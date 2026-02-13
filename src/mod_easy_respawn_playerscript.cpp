/*
 * Credits: silviu20092
 */

#include "ScriptMgr.h"
#include "easy_respawn_mgr.h"

class mod_easy_respawn_playerscript : public PlayerScript
{
public:
    mod_easy_respawn_playerscript() : PlayerScript("mod_easy_respawn_playerscript",
        {
            PLAYERHOOK_CAN_REPOP_AT_GRAVEYARD
        }) {
    }

    bool OnPlayerCanRepopAtGraveyard(Player* player) override
    {
        if (sEasyRespawnMgr->RespawnAndTeleport(player))
            return false;

        return true;
    }
};

void AddSC_mod_easy_respawn_playerscript()
{
    new mod_easy_respawn_playerscript();
}
