/*
 * Credits: silviu20092
 */

#ifndef _EASY_RESPAWN_MGR_H_
#define _EASY_RESPAWN_MGR_H_

class Player;

enum ResurrectMapMask
{
    MAP_MASK_DISABLED   = 0,
    MAP_MASK_DUNGEON    = 1 << 0,
    MAP_MASK_RAID       = 1 << 1,
    MAP_MASK_OPEN_WORLD = 1 << 2,
    MAP_MASK_ALL        = MAP_MASK_DUNGEON | MAP_MASK_RAID | MAP_MASK_OPEN_WORLD
};

enum InstanceRespawnLocation
{
    RESPAWN_INSIDE = 1,
    RESPAWN_OUTSIDE
};

enum OpenWorldRespawnLocation
{
    RESPAWN_IMMEDIATE = 1,
    RESPAWN_AT_GRAVEYARD,
    RESPAWN_AT_CORPSE
};

class EasyRespawnMgr
{
private:
    EasyRespawnMgr();
    ~EasyRespawnMgr();

    int32 resurrectMapMask;
    float resurrectHealthPct;
    std::set<int32> disabledMapIds;
    InstanceRespawnLocation instanceRespawnLocation;
    OpenWorldRespawnLocation openWorldRespawnLocation;
    std::unordered_map<uint32, WorldLocation> overrideLocations;

    void CreateResurrectMapMask(const std::string& activeStr);

    bool IsValidResurrectMapMask(const Player* player) const;
    void Resurrect(Player* player) const;

    bool IsDisabledMapId(uint32 mapId) const;
    void CreateDisabledMapIdSet(const std::string& disabledMapIdsStr);

    void CreateOverrideLocations(const std::vector<std::string>& overrides);
    const WorldLocation* ChooseOverrideLocation(uint32 mapId) const;
public:
    static EasyRespawnMgr* instance();

    bool RespawnAndTeleport(Player* player) const;
    void HandleConfigSettings(
        const std::string& activeStr,
        float resurrectHealthPct,
        const std::string& disabledMapIdsStr,
        int32 instanceRespawnLocation,
        int32 openWorldRespawnLocation,
        const std::vector<std::string>& overrides
    );
};

#define sEasyRespawnMgr EasyRespawnMgr::instance()

#endif
