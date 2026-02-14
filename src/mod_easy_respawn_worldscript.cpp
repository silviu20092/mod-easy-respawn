/*
 * Credits: silviu20092
 */

#include "ScriptMgr.h"
#include "Config.h"
#include "easy_respawn_mgr.h"

class mod_easy_respawn_worldscript : public WorldScript
{
public:
    mod_easy_respawn_worldscript() : WorldScript("mod_easy_respawn_worldscript",
        {
            WORLDHOOK_ON_AFTER_CONFIG_LOAD
        }) {
    }

    void OnAfterConfigLoad(bool reload) override
    {
        sEasyRespawnMgr->HandleConfigSettings(
            sConfigMgr->GetOption<int32>("EasyRespawn.ResurrectMapMask", MAP_MASK_ALL),
            sConfigMgr->GetOption<float>("EasyRespawn.ResurrectPct", 1.0f),
            sConfigMgr->GetOption<std::string>("EasyRespawn.DisabledMapIds", ""),
            sConfigMgr->GetOption<int32>("EasyRespawn.InstanceRespawnLocation", RESPAWN_OUTSIDE),
            sConfigMgr->GetOption<int32>("EasyRespawn.OpenWorldRespawnLocation", RESPAWN_AT_GRAVEYARD),
            sConfigMgr->GetKeysByString("EasyRespawn.OverrideRespawnLocation.")
        );
    }
};

void AddSC_mod_easy_respawn_worldscript()
{
    new mod_easy_respawn_worldscript();
}
