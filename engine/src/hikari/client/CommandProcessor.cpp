#include "hikari/client/CommandProcessor.hpp"
#include "hikari/core/util/StringUtils.hpp"
#include "hikari/core/util/exception/HikariException.hpp"
#include <sstream>
#include <algorithm>
#include <iterator>

namespace hikari {

    const CommandProcessor::Command CommandProcessor::parseCommand(const std::string& commandString) const {
        std::istringstream iss(commandString);
        std::vector<std::string> tokens;

        // Tokenize the input
        std::copy(
            std::istream_iterator<std::string>(iss),
            std::istream_iterator<std::string>(),
            std::back_inserter<std::vector<std::string>>(tokens)
        );

        // Check boundary conditions...
        if(tokens.size() > 0) {
            ArgumentList args(tokens.size() - 1); // token[0] is the command name, everything else is an arg
            std::string commandName = tokens.at(0);

            std::copy_if(
                tokens.begin() + 1, 
                tokens.end(), 
                args.begin(),
                [](const std::string& arg) { return !arg.empty(); } // only copy non-empty args
            );

            return std::make_tuple(commandName, args);
        } else {
            // Poorly formed command.
            throw HikariException("Command string could not be parsed.");
        }
    }

    void CommandProcessor::processCommand(const std::string& commandString) {
        std::string cleanString = StringUtils::toLowerCase(StringUtils::trim(commandString));

        Command command = parseCommand(cleanString);

        processCommand(std::get<0>(command), std::get<1>(command));
    }

    void CommandProcessor::processCommand(const std::string& commandName, const ArgumentList& args) {
        auto commandSearch = handlers.find(commandName);

        if(commandSearch != handlers.end()) {
            CommandHandler handler = commandSearch->second;
            handler(args);
        } else {
            throw HikariException("No handlers for the command \"" + commandName + "\" are registered.");
        }
    }

    void CommandProcessor::registerHandler(const std::string& commandName, const CommandHandler handler) {
        auto commandSearch = handlers.find(commandName);

        if(commandSearch == handlers.end()) {
            handlers.insert(
                std::make_pair(commandName, handler));
        } else {
            throw HikariException("A handler is already registered for the command \"" + commandName + "\". Unregister the existing handler before registering another.");
        }
    }

    void CommandProcessor::unregisterHandler(const std::string& commandName) {
        auto commandSearch = handlers.find(commandName);

        if(commandSearch != handlers.end()) {
            handlers.erase(commandSearch);
        } else {
            throw HikariException("No handlers for the command \"" + commandName + "\" are registered.");
        }
    }

} // hikari