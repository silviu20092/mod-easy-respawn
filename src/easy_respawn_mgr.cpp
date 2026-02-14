/*
 * Credits: silviu20092
 */

#include "Player.h"
#include "Tokenize.h"
#include "StringConvert.h"
#include "BattlefieldMgr.h"
#include "GameGraveyard.h"
#include "easy_respawn_mgr.h"

EasyRespawnMgr::EasyRespawnMgr()
{
    resurrectMapMask = MAP_MASK_ALL;
    resurrectHealthPct = 1.0f;
    instanceRespawnLocation = RESPAWN_OUTSIDE;
    openWorldRespawnLocation = RESPAWN_AT_GRAVEYARD;
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

    if (sBattlefieldMgr->GetBattlefieldToZoneId(player->GetZoneId()))
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

    player->RemovePlayerFlag(PLAYER_FLAGS_IS_OUT_OF_BOUNDS);
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
    bool resurrect = false;
    if (map->IsDungeon())
    {
        AreaTriggerTeleport const* chosenTrigger = nullptr;
        if (instanceRespawnLocation == RESPAWN_INSIDE)
            chosenTrigger = sObjectMgr->GetMapEntranceTrigger(map->GetId());
        else
        {
            chosenTrigger = sObjectMgr->GetGoBackTrigger(map->GetId());
            if (chosenTrigger == nullptr)
                chosenTrigger = sObjectMgr->GetMapEntranceTrigger(map->GetId());
        }

        if (chosenTrigger != nullptr)
        {
            player->TeleportTo(chosenTrigger->target_mapId, chosenTrigger->target_X, chosenTrigger->target_Y, chosenTrigger->target_Z, chosenTrigger->target_Orientation);
            resurrect = true;
        }
    }
    else
    {
        if (player->GetPositionZ() >= map->GetMinHeight(player->GetPositionX(), player->GetPositionY()))
        {
            if (openWorldRespawnLocation == RESPAWN_AT_GRAVEYARD)
            {
                GraveyardStruct const* closestGrave = sGraveyard->GetClosestGraveyard(player, player->GetTeamId());
                if (closestGrave != nullptr)
                    player->TeleportTo(closestGrave->Map, closestGrave->x, closestGrave->y, closestGrave->z, player->GetOrientation());
                else
                    player->TeleportTo(player->m_homebindMapId, player->m_homebindX, player->m_homebindY, player->m_homebindZ, player->GetOrientation());
            }
            resurrect = true;
        }
    }

    if (resurrect)
        Resurrect(player);

    return resurrect;
}

void EasyRespawnMgr::HandleConfigSettings(int32 resurrectMapMask, float resurrectHealthPct, const std::string& disabledMapIdsStr, int32 instanceRespawnLocation, int32 openWorldRespawnLocation)
{
    if ((resurrectMapMask & ~MAP_MASK_ALL) == 0)
        this->resurrectMapMask = resurrectMapMask;
    else
        this->resurrectMapMask = MAP_MASK_ALL;

    if (resurrectHealthPct >= 0.1f && resurrectHealthPct <= 1.0f)
        this->resurrectHealthPct = resurrectHealthPct;
    else
        this->resurrectHealthPct = 1.0f;

    if (instanceRespawnLocation >= RESPAWN_INSIDE && instanceRespawnLocation <= RESPAWN_OUTSIDE)
        this->instanceRespawnLocation = (InstanceRespawnLocation)instanceRespawnLocation;
    else
        this->instanceRespawnLocation = RESPAWN_OUTSIDE;

    if (openWorldRespawnLocation >= RESPAWN_IMMEDIATE && openWorldRespawnLocation <= RESPAWN_AT_GRAVEYARD)
        this->openWorldRespawnLocation = (OpenWorldRespawnLocation)openWorldRespawnLocation;
    else
        this->openWorldRespawnLocation = RESPAWN_AT_GRAVEYARD;

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
