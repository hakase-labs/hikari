#ifndef HIKARI_CLIENT_GAME_GAMESTATEFACTORY
#define HIKARI_CLIENT_GAME_GAMESTATEFACTORY

#include <string>
#include <boost/smart_ptr.hpp>
#include <hikari/core/game/GameState.hpp>
#include <json/value.h>

#pragma warning(push)
#pragma warning(disable:4251)

namespace hikari {
namespace client {
namespace game {

	typedef boost::shared_ptr<hikari::core::game::GameState> StatePtr;

	class GameStateFactory {
	public:
		GameStateFactory();

		/**
			Builds a game state. Parameters are given in the form of a JSON
			object. The JSON object must contain a property called "type" with
			a value that is a string.

			@param buildParams a JSON object with a "type" property and other
			properties specific to the creation of a particular game state.

			@return a smart pointer to an instance of a GameState
		*/
		StatePtr build(const Json::Value &buildParams);
	};

} // hikari.client.game
} // hikari.client
} // hikari

#endif // HIKARI_CLIENT_GAME_GAMESTATEFACTORY