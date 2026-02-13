# ![logo](https://raw.githubusercontent.com/azerothcore/azerothcore.github.io/master/images/logo-github.png) AzerothCore

# Easy respawn module for AzerothCore

## Overview

Adds the possibility for players to be resurrected and teleported to the start of dungeons/raids instead of the closest graveyard. Configuration options makes possible for players to instantly respawn in open world aswell.


## How to install

1. Clone this repository to your AzerothCore repo modules folder. You should now have mod-easy-respawn there.
2. Re-run cmake to generate the solution.
3. Re-build your project.
4. You should have mod_easy_respawn.conf.dist copied in configs/modules after building, copy this to configs/modules in your server's base directory (remove the .dist extension).
5. Start the server & enjoy.

## How to use

Open **mod_easy_respawn.conf** and check **EasyRespawn.ResurrectMapMask** option. Simply use bitwise or (you can use an online calculator) to toggle between the 3 possible options (use 0 if you want to disable the module altogether).
Easily disable specific maps by using **EasyRespawn.DisabledMapIds** option, just add the map ids separated by a comma.

## Credits
- silviu20092