#ifndef HIKARI_CLIENT_GAMEQUITEVENTDATA
#define HIKARI_CLIENT_GAMEQUITEVENTDATA

#include "hikari/client/game/events/BaseEventData.hpp"

namespace hikari {

    class GameQuitEventData : public BaseEventData {
    public:
        static const EventType Type;

        enum QuitType {
            PROMPT_USER,
            QUIT_NOW
        };

    private:
        QuitType quitType;
        
    public:
        explicit GameQuitEventData(QuitType quitType = PROMPT_USER);
        virtual ~GameQuitEventData();

        QuitType getQuitType() const;

        virtual const EventType & getEventType() const;

        virtual EventDataPtr copy() const;
        virtual const char * getName() const;
    };

} // hikari

#endif // HIKARI_CLIENT_GAMEQUITEVENTDATA