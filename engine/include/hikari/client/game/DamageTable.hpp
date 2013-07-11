#ifndef HIKARI_CLIENT_GAME_DAMAGETABLE
#define HIKARI_CLIENT_GAME_DAMAGETABLE

#include "hikari/core/util/Service.hpp"
#include "hikari/client/game/DamageKey.hpp"

#include <unordered_map>

namespace hikari {

  class DamageTable : public Service {
  private:
    std::unordered_map<int, float> damageValues;
    // std::unordered_map<DamageKey, float> damageBuffs;

  public:
    DamageTable();
    virtual ~DamageTable();

    /**
     * Adds an entry to the table for a given damageId the amount of damage it
     * should cause.
     *
     * @param damageId a unique damageId
     * @param ammount  the amount of damage it causes normally
     */
    void addEntry(int damageId, float ammount);

    /**
     * Returns whether a given damageId has an entry or not.
     * 
     * @param  damageId the damageId to look for
     * @return          true if an netry was found for the specified damageId,
     *                  otherwise false
     */
    bool hasEntryFor(int damageId) const;

    /**
     * Gets the amount of damage caused by the specified damageId. If there is
     * no entry for the damageId then 0 is returned.
     *
     * @param  damageId [description]
     * @return          [description]
     */
    float getDamageFor(int damageId) const;
  };

} // hikari

#endif // HIKARI_CLIENT_GAME_DAMAGETABLE