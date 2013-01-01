#include <hikari/client/game/GameStateFactory.hpp>
#include <hikari/client/game/GuiTestState.hpp>
#include <hikari/client/game/MapTestState.hpp>
#include <hikari/client/game/SpriteTestState.hpp>
#include <sstream>
#include <stdexcept>

namespace hikari {
namespace client {
namespace game {

	GameStateFactory::GameStateFactory() {

	}

	/*
		All states have to have these at a minimum:
		
		{
			"type":"string_type_name",
			"id":"string_id"
		}
	*/
	StatePtr GameStateFactory::build(const Json::Value &buildParams) {
		if(!buildParams["type"].isNull()) {
			if(buildParams["type"].isString()) {
				std::string type = buildParams["type"].asString();

				if(type == "type1") {

				} else if(type == "type2") {

				} else {
					std::stringstream ss;
					ss << "Cannot instantiate game state; unrecognized type '";
					ss << type;
					ss << "'.";
					throw std::runtime_error(ss.str());
				}
			} else {
				throw std::runtime_error("Cannot instantiate game state; type is not a string.");
			}
		} else {
			throw std::runtime_error("Cannot instantiate game state; no type given.");
		}
	}

} // hikari.client.game
} // hikari.client
} // hikari