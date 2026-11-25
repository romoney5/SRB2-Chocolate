<p align = "center">
<img width="132" height="132" alt="srb2chocolate" src="https://github.com/user-attachments/assets/94fc79e3-f048-4238-acee-37b427dc3f90" />
</p>
<h1 align = "center">Sonic Robo Blast 2 Chocolate</h1>

SRB2 Chocolate is an experimental custom build, forked from [SRB2 Banpyura](https://www.github.com/GLideKS/SRB2-Banpyura), that aims to provide a more feature-complete experience similar to [SRB2-edit](https://www.github.com/luigi-budd/SRB2-edit).

Icon made by romoney5.

## Chocolate Features

- Use the `runlua` command to run any line of Lua code locally! Great for debugging.
  - As runlua scripts are treated as loading Lua scripts, hooks and freeslots can also be created.
- Resize the window to any resolution you like (up to 8192x8192)!
  - Ported the `any-resolution` branch with various fixes.

- Snappier level loading, with `wipes` both on and off.
- More immediate startup.
- Losing focus from the game window now fully pauses the game.

## Chocolate Branches

Since I've used this repository as a testing workbench in the past (and the present), there are numerous feature branches with experimental features; some are intended to be merged into the main branch. Also note that most branches that relate to compilation won't work outside of MSVC (or even at all!) out of the box, as that is the main compiler I use.

- chocolate-main: Main branch, most other branches are built on top of this (or Banpyura vanilla-master).
- chocolate-next: Next branch; same as chocolate-main but synced with SRB2's v2.2.16 next branch.
- chocolate-luajit: WIP port of LuaJIT to SRB2. You need the includes and a DLL of LuaJIT's source code in order to run this.
- vm-luamenu: Source code for "Luamenu," WIP recreation of SRB2's menus in Lua. See romoney5/luamenu.
- lo: Heavy WIP infrastructure for a port of SRB2 to the LOVE game framework.
- openlump: io.readlump Lua function, returns the content of a lump as a string.
- twopointthree: Port of numerous SRB2 2.3 (dev-master branch) C++ features, including the RHI and new audio subsystem. Also includes a full port of Ring Racers' WebM video recorder. Only compilable using CMake.
- twopointsevenpluswalllighting: twopointthree branch, including visual features like wall lighting, lighter drop shadows, and more accurate underwater rippling.
- vm-avrecorder: Experimental port of Ring Racers' WebM video recorder to base vanilla-master (less C++slop features). Does not have audio and video is reported to be buggier, so be mindful of that when porting this branch.
- (lib)avrecorder: Port of xdf (aka Alice)'s port of Lactozilla's old video encoder powered by libav. This is more heavyweight in libraries, though, and recording performance is poorer compared to avrecorder (this is not multithreaded). On the plus side, though, it supports more video formats.
- kart-discord-rpc: Experimental port of SRB2Kart's Discord Rich Presence features to vanilla-master (plus source code included within the game). For a complete implementation of this, see luigi-budd/SRB2-edit.
- stuff: Has various features mixed-in: a fixed-up delfile, any-resolution with fractional scaling, and a "Network Options" menu.
- only-coraonas: WIP port of Final Demo's Corona functionality to vanilla-master. Assisted by Eden's abandoned merge request and existing leftover functionality in 2.2. Note that gr_batching must be disabled.
- woofmidi: Port of MIDIMan's port of Woof (Doom source port)'s Native MIDI player. Crashes often.
- moviesfreetwo: Experimental port of SRB2 Legacy's GitHub Actions workflows, MacOS support was confirmed to not work by WhisperVII. It has funny commit names, though.
- twopointone: Was intended to be an SRB2 Legacy backward-compatibility branch, only includes serverinfo as of now.
- tungworld

Any other branches not mentioned here are either already merged in or not noteworthy enough to make it into the list.

## Banpyura Features

### HUD/Menus

- Toggle screen fades using `wipes`. Not effective in Marathon Run.
- Lots of chat tweaks! Window positioning, snapping, copy-paste, selection, customizable opacity, and more.
- Menu tweaks! background color, text case switching, selection color.
- Extra camera options: Exact aiming, Clipping style, Gamepad Camera sensitivity
- Addons menu has it's background translucent and their text on thin font.
- A progress bar now displays when checking server files. (SRB2 v2.2.16 Nightly)
- Inverted crosshairs option (SRB2-edit)
- Player names on the rankings hud always use a thin font
- Servers on the Master Server list has a background (SRB2-Edit)
- Server information display! shows players, new download confirmation, addon listing, current map. (Lugent)
- - Also plays a music when showing the server information (`SRVMS1`) and loading files (`SRVMS2`). You can override the music loading an addon containing `SRVMS1` and `SRVMS2` as the music.
- - This feature is toggleable with `serverviewmusic` command.
- "Quit Game" and "Abort" red colored
- OpenGL's black console color to be a proper black and gray is an color option (SRB2 v2.2.16 Nightly)
- `cecho` messages now prints on console.

### Visuals

- `renderhitbox` usable in multiplayer.
- OpenGL Light Dithering, smooth light ramps even for Palette rendering! (SRB2-Classic)
- Splat interpolation (SRB2-edit)
- Added 1360x768 resolution (personal use)
- `r_secbright` Configurable minimum sector brightness (SRB2-Legacy)
- Readded 2.1 sprite shadows! Enabled using `shadow Sprite` in OpenGL. (SRB2-Legacy)

### Gameplay / Netplay

- `addfilelocal` allows you to load addons locally. Can be accessed via Addons Menu and pressing Right Alt as well. (SRB2 Kart Saturn)
- Fixed SRB2's loading time. (SRB2-Classic)
- Minimum input delay and Gentleman's delay (Ring Racers)
- Lifted netxcmd limits (SRB2-Classic)
- `cam_centertoggle` and `cam2_centertoggle` are not exclusive to the Automatic playstyle.
- Rejoin server menu with timestamps (Ring Racers)
- Added total progress bar while downloading addons from a server.
- `clearscores` command now blames the admin that used it rather than blaming the host.
- `addfile`/`addfolder` requests from administrators are logged.
- `allowfilerequests`: If off, addfile/addfolder requests from admins will be silently ignored (they are still logged). On by default.

### Lua

- Added `CameraThinker` hook to alter the camera's behavior.
- - `player` and `camera` are passed in as arguments. Return `true` to override vanilla camera movement.
- Added HTTP library from SRB2 AX.
- - Check the wiki (https://github.com/GLideKS/SRB2-Banpyura/wiki/Lua-stuff) for more details.

### Miscellaneous

- Improved GIF/movie recording. (Movie HUD, default size limit of 20 MB, fixed aPNG, etc.)
- The game now goes to the title or drops a warning instead of crashing on the following situations: `Invalid sector number from server`, `Invalid line number from server`, `Savegame corrupted`, `polyobj count inconsistency`, SOCK_Send errors.
- `saveaddons` saves the current addon order to be loaded as a console script. Useful for quickly testing addon lists locally for servers.
- Don't reset chasecam and do not run special stage wipe on resync (Jisk, Lugent)
- Fixed chats not being saved on the logs in Linux systems
- Removed 5-second delay upon disconnecting from servers
- Fixed nameless servers causing other servers to not display.
- The `help` command is now sorted by origin (Vanilla, Banpyura, Addon). You can use `-v`, `-c`, and `-a` respectively to only print out only those sections. (You can use more than one flag at once).
- Doing merely `<cvar>` has the same effect as `help <cvar>`.
- Console variable information is more in-depth.
- - You can choose to hide certain sections via `cvarinfo` ("Show All" by default).
- See the amount of used freeslots using `freeslots`.
- Force console input with the `-forceconsole` parameter.
- Banpyura options are saved in a separated config file. `bnpconfig.cfg`

**Most of these options can be found in the menu under Banpyura Options....**

# Installation:

- Github Actions build (go to the Actions tab on the top, then select the one that corresponds to your platform; x64 Windows builds require different DLLs) (this requires being signed in to GitHub!)
- Compile from the source code.
- [Download the latest build from nightly.link (Windows x86)](https://nightly.link/romoney5/SRB2-Chocolate/workflows/windows-x86-makefile/chocolate-main?preview)

# Screenshots / GIFs

<img width="640" height="512" alt="image" src="https://github.com/user-attachments/assets/de0b15e8-b711-4aae-92bf-809005a2b70a" />
<img width="640" height="400" alt="srb20114" src="https://github.com/user-attachments/assets/ee678a31-d8b2-4743-9474-9fb6d1b1f0f8" />
<img width="640" height="400" alt="srb20113" src="https://github.com/user-attachments/assets/c0ed8a7a-bd52-49b0-90fd-425d982f1ae1" />
<img width="320" height="200" alt="srb20230" src="https://github.com/user-attachments/assets/43fdb009-b300-4e98-ac41-c60644c2f648" />
<img width="320" height="200" alt="srb20229" src="https://github.com/user-attachments/assets/ac1ff16b-54de-4803-822a-f866f575c1a9" />
<img width="320" height="200" alt="srb20228" src="https://github.com/user-attachments/assets/b2b46a4d-93d2-431c-822f-f6e1c6dc1872" />
<img width="320" height="200" alt="srb20226" src="https://github.com/user-attachments/assets/d49162f1-b5d9-4339-b92c-f1cfc147a304" />
<img width="320" height="256" alt="srb20225" src="https://github.com/user-attachments/assets/13dfa657-d72a-463f-87b5-c832e663fdad" />
<img width="320" height="256" alt="srb20224" src="https://github.com/user-attachments/assets/c1b8e0cf-6a57-40b8-b5a9-fa04c5d953e9" />

## Dependencies
- SDL2 (Linux/OS X only)
- SDL2-Mixer (Linux/OS X only)
- libupnp (Linux/OS X only)
- libgme (Linux/OS X only)
- libopenmpt (Linux/OS X only)

## Compiling

See [SRB2 Wiki/Source code compiling](http://wiki.srb2.org/wiki/Source_code_compiling)

## Disclaimer
Sonic Team Junior is in no way affiliated with SEGA or Sonic Team. We do not claim ownership of any of SEGA's intellectual property used in SRB2.
