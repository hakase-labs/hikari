#ifndef HIKARI_CLIENT_DOOREVENTDATA
#define HIKARI_CLIENT_DOOREVENTDATA

#include "hikari/client/game/events/BaseEventData.hpp"

#include <memory>

namespace hikari {

    class Door;

    class DoorEventData : public BaseEventData {
    public:
        static const EventType Type;

    private:
        std::weak_ptr<Door> door;
        
    public:
        explicit DoorEventData(const std::weak_ptr<Door> & door);
        virtual ~DoorEventData();

        const std::weak_ptr<Door> & getDoor() const;

        virtual const EventType & getEventType() const;

        virtual EventDataPtr copy() const;
        virtual const char * getName() const;
    };

} // hikari

#endif // HIKARI_CLIENT_DOOREVENTDATA