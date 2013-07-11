#include "hikari/client/game/DamageTable.hpp"

namespace hikari {

    DamageTable::DamageTable() 
        : Service()
        , damageValues()
        // , damageBuffs()
    {
        
    }

    DamageTable::~DamageTable() {
        
    }

    void DamageTable::addEntry(int damageId, float ammount) {
        damageValues[damageId] = ammount;
    }

    bool DamageTable::hasEntryFor(int damageId) const {
        return damageValues.count(damageId) > 0;
    }

    float DamageTable::getDamageFor(int damageId) const {
        auto it = damageValues.find(damageId);

        if(it != std::end(damageValues)) {
            return it->second;
        }

        return 0.0f;
    }

} // hikari