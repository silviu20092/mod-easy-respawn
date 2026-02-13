/*
 * Credits: silviu20092
 */

#include "Player.h"
#include "Tokenize.h"
#include "StringConvert.h"
#include "easy_respawn_mgr.h"

EasyRespawnMgr::EasyRespawnMgr()
{
    resurrectMapMask = MAP_MASK_ALL;
    resurrectHealthPct = 1.0f;
}

EasyRespawnMgr::~EasyRespawnMgr()
{
}

EasyRespawnMgr* EasyRespawnMgr::instance()
{
    static EasyRespawnMgr instance;
    return &instance;
}

bool EasyRespawnMgr::IsValidResurrectMapMask(const Player* player) const
{
    Map* map = player->GetMap();

    if (map->IsBattlegroundOrArena())
        return false;

    if (IsDisabledMapId(map->GetId()))
        return false;

    if ((resurrectMapMask & MAP_MASK_DUNGEON) && map->IsNonRaidDungeon())
        return true;

    if ((resurrectMapMask & MAP_MASK_RAID) && map->IsRaid())
        return true;

    if ((resurrectMapMask & MAP_MASK_OPEN_WORLD) && map->IsWorldMap())
        return true;

    return false;
}

void EasyRespawnMgr::Resurrect(Player* player) const
{
    player->CombatStop();
    player->getHostileRefMgr().deleteReferences();

    player->ResurrectPlayer(resurrectHealthPct);
    player->SpawnCorpseBones();
}

bool EasyRespawnMgr::IsDisabledMapId(uint32 mapId) const
{
    return disabledMapIds.find(mapId) != disabledMapIds.end();
}

bool EasyRespawnMgr::RespawnAndTeleport(Player* player) const
{
    if (!player || !player->GetMap() || !player->IsInWorld())
        return false;

    if (player->IsAlive())
        return false;

    if (!IsValidResurrectMapMask(player))
        return false;

    Map* map = player->GetMap();
    if (map->IsDungeon())
    {
        if (AreaTriggerTeleport const* at = sObjectMgr->GetMapEntranceTrigger(map->GetId()))
        {
            player->TeleportTo(at->target_mapId, at->target_X, at->target_Y, at->target_Z, at->target_Orientation);
            Resurrect(player);
            return true;
        }
        return false;
    }

    Resurrect(player);

    return true;
}

void EasyRespawnMgr::HandleConfigSettings(int32 resurrectMapMask, float resurrectHealthPct, const std::string& disabledMapIdsStr)
{
    if ((resurrectMapMask & ~MAP_MASK_ALL) == 0)
        this->resurrectMapMask = resurrectMapMask;
    else
        this->resurrectMapMask = MAP_MASK_ALL;

    if (resurrectHealthPct >= 0.1f && resurrectHealthPct <= 1.0f)
        this->resurrectHealthPct = resurrectHealthPct;
    else
        this->resurrectHealthPct = 1.0f;

    CreateDisabledMapIdSet(disabledMapIdsStr);
}

void EasyRespawnMgr::CreateDisabledMapIdSet(const std::string& disabledMapIdsStr)
{
    disabledMapIds.clear();
    std::vector<std::string_view> tokenized = Acore::Tokenize(disabledMapIdsStr, ',', false);
    std::transform(
        tokenized.begin(),
        tokenized.end(),
        std::inserter(disabledMapIds, disabledMapIds.end()),
        [](const std::string_view& str) { return *Acore::StringTo<int32>(str); }
    );
}
