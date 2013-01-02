#ifndef HIKARI_CLIENT_COMMANDPROCESSOR
#define HIKARI_CLIENT_COMMANDPROCESSOR

#include <functional>
#include <map>
#include <string>
#include <tuple>
#include <vector>

namespace hikari {

    class CommandProcessor {
    public:
        typedef std::vector<std::string> ArgumentList;
        typedef std::tuple<const std::string, const ArgumentList> Command;
        typedef std::function<void (ArgumentList)> CommandHandler;

    private:
        std::vector<Command> history;
        std::map<std::string, CommandHandler> handlers;

        const Command parseCommand(const std::string& commandString) const;
        // const ArgumentList parseCommandArguments(const std::string& commandString) const;

    public:
        void processCommand(const std::string& commandString);
        void processCommand(const std::string& command, const ArgumentList& args);

        void registerHandler(const std::string& commandName, const CommandHandler handler);
        void unregisterHandler(const std::string& commandName);
    };

} // hikari

#endif // HIKARI_CLIENT_COMMANDPROCESSOR