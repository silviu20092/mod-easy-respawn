# ![logo](https://raw.githubusercontent.com/azerothcore/azerothcore.github.io/master/images/logo-github.png) AzerothCore

# Easy respawn module for AzerothCore

## Overview

Adds the possibility for players to be resurrected and teleported to the start of dungeons/raids instead of the closest graveyard. Configuration options makes possible for players to instantly respawn in open world aswell. This module also works for playerbots and affects how they are resurrected.


## How to install

1. Clone this repository to your AzerothCore repo modules folder. You should now have mod-easy-respawn there.
2. Re-run cmake to generate the solution.
3. Re-build your project.
4. You should have mod_easy_respawn.conf.dist copied in configs/modules after building, copy this to configs/modules in your server's base directory (remove the .dist extension).
5. Start the server & enjoy.

## How to use

Open **mod_easy_respawn.conf** and check **EasyRespawn.Active** option.

Easily disable specific maps by using **EasyRespawn.DisabledMapIds** option, just add the map ids separated by a comma.

If you want to override the respawn location for certain maps, you just need to add it to config file (just add another config line): **EasyRespawn.OverrideRespawnLocation.YOUR_MAP_ID=new_map_id,new_pos_x,new_pos_y,new_pos_z,new_orientation**. An override is already provided there for Icecrown Citadel (map id 631).

## Credits
- silviu20092
