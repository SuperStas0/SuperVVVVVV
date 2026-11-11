# SuperVVVVVV

Altered version of [VVVVVV](https://github.com/TerryCavanagh/VVVVVV), not the original game. VVVVVV is copyright of Terry Cavanagh and this is based on the VVVVVV 2.4.4 source code. Practice patch for Windows (Steam version) that adds savestates for time trials, intermission replays and No Death Mode, and a Super mode with fast restarts.

# Installation
Requires your own copy of VVVVVV on Steam. The game's `data.zip` is not included.

1. In the game folder (`...\steamapps\common\VVVVVV`), copy the original `VVVVVV.exe` to a backup, e.g. `VVVVVV-vanilla.exe`.
2. Extract the release archive into the game folder, overwriting `VVVVVV.exe`.
3. Launch the game from Steam.

To go back to vanilla, put the backup back as `VVVVVV.exe`, or use Steam > Properties > Installed Files > Verify integrity of game files. A Steam update also restores it.

The build is 64-bit and statically linked, so it needs no `SDL2.dll`. It uses the same save and settings folder as the original game.

## Steam achievements
- Needs the 64-bit `steam_api64.dll` next to `VVVVVV.exe`. It is not included: copy it from any 64-bit Steam game's folder or from the Steamworks SDK (`redistributable_bin\win64`).
- Steam must be running and the game must be launched from the Steam library with `VVVVVV.exe` replaced as above. A Non-Steam Game shortcut has another app ID and gets no achievements.
- Without the DLL or Steam the game runs normally, without achievements.
- Check: each launch writes `steam_status.txt` to the game folder. It says `Steam API on`, or `Steam API off: <reason>`. If the file is missing, the exe is an older build or its folder is not writable.
- An invalid run (see Valid runs) never unlocks achievements, same as its record. At startup the game re-sends the achievements your save data already implies, as the original does.

# Usage
## Savestates
| Key | Action |
| --- | --- |
| `Shift` + `` ` `` (`~`) | Save state |
| `` ` `` (`~`) | Load state |

The key is the one left of `1`. Ctrl or Alt held does nothing.

- Off by default. Turn them on with `Allow savestates` in the Super menu.
- Saves the full in-game state: trinkets, room, position, checkpoint, gravity, entities, script progress and music. The timer is not saved or loaded, it keeps running.
- Active in the main game, time trials, intermission replays and No Death Mode.
- One slot per mode: the main game (new game, continue and quicksave share it), each time trial, each intermission replay, No Death Mode with cutscenes and without. Time trials and intermissions with Super mode on have their own slots.
- Slots are kept while the game runs, also across leaving and entering levels. Nothing is saved to disk.
- A state loads only in the mode it was saved in. Saving is ignored during the death animation.
- Save, load and toggles show a message in place of the room name.

## Super menu
`Esc` > `options` > `super`, shown only during a main game, time trial, intermission replay or No Death Mode run. Everything is off when the game starts.

| Item | Action |
| --- | --- |
| `super mode` | Toggle Super mode. In a time trial or intermission it restarts the level. |
| `allow savestates` | Toggle the save and load keys and the game over buttons. |
| `delete savestates` | Delete all states in all modes. |

## Super mode
Affects only what happens after dying and which screens appear.
- Main game: no effect, and it does not make a run invalid.
- No Death Mode: 0.5 s after dying the game leaves the run without the fade and the game over delay. Gameplay is not affected.
  - Without a state: opens the cutscene choice screen of No Death Mode.
  - With a state: opens the game over screen with its buttons at once.
- Time trials and intermission replays: No Death Mode rules (no checkpoints, no cutscenes). Dying restarts the level at its start 0.5 s later, without the fade. Time trials use a 1 s countdown. The timer restarts. States are kept.

## Game over screen
Shown after dying in No Death Mode.

| Button | Action |
| --- | --- |
| `load savestate` | Starts the mode of the last saved No Death Mode state and loads it. Selected by default. |
| `retry` | Starts the same mode again. |
| `return to play menu` | Original button. |

`load savestate` and `retry` appear only if savestates are allowed and a No Death Mode state exists. Without Super mode the screen keeps its original delay.

## Valid runs
A run is invalid if any of these is true:
- a state was loaded in it
- a state exists for its mode (main game: any state of the main game; No Death Mode: for either cutscene choice)
- it is a time trial or intermission with Super mode on

Invalid runs show `GG!` in place of `Level Complete!` and `Game Complete!` (main game and No Death Mode), `Results` (time trials) and `WOW` (No Death Mode). From the moment a run is invalid it earns no achievements, trophies, unlocks, trinket stats, Super Gravitron records, best death count or time trial records. What was earned before the first state was saved stays. Use `delete savestates` before a real run.

Super mode in No Death Mode does not make a run invalid.

# Translations
Texts added by SuperVVVVVV are translated with `lang\<code>\super.xml`, in the same format as the game's `strings.xml`. Missing or empty translations stay English. Official texts stay in the game's own `strings.xml`.

To add a language:
1. Copy `desktop_version\lang\en\super.xml` to `desktop_version\lang\<code>\super.xml`. `<code>` is the folder name the game uses for that language. The language must already exist in the game.
2. Fill the `translation` attributes. Keep the `english` attributes unchanged.
3. Run `build.bat -r`, or copy the file into the game's `lang\<code>\` folder.

# Autosplitter
`LiveSplit.VVVVVV.asl` is the VVVVVV autosplitter with SuperVVVVVV support. Start, split, reset, game time and settings are unchanged. Its source is `autosplitter\LiveSplit.VVVVVV.asl`; `build.bat` copies it to `build\` and `-r` adds it to the release archive.

## Setup
1. LiveSplit: right-click > Edit Layout > add Control > Scriptable Auto Splitter.
2. Layout Settings > Scriptable Auto Splitter > Script Path: select `LiveSplit.VVVVVV.asl`.
3. Choose the splits in the same settings page.

## Detection
| Game | How it is detected |
| --- | --- |
| Vanilla VVVVVV 2.0-2.4.4 | Original module size detection and offsets |
| Modded build (any other `VVVVVV.exe`) | Scans the exe for the `SUPERVVVVVV_SPLIT` marker and reads the `SplitData` struct. Works for any exe size. |
| Anything else | Inactive |

## Changing the game
- `SplitData.cpp` mirrors the game variables each frame in `loop_end()`. The struct is append-only: never move, resize or remove fields.
- New field: add it at the end of the struct, then read it in the `update` block of the `.asl`.
- Incompatible layout: change the marker in `SplitData.cpp` and in the `.asl` `init` block, so older scripts stay inactive instead of reading wrong data.
- Other exe name: duplicate the `VVVVVV` `split` and `unknown` states in the `.asl` with that process name.

# What changed from upstream
Based on the VVVVVV 2.4.4 tag (`ea811e1`). Useful when rebasing onto a newer version:

| File | Change |
| --- | --- |
| `desktop_version\src\Savestate.h`, `desktop_version\src\Savestate.cpp` | New: savestate logic |
| `desktop_version\src\SplitData.h`, `desktop_version\src\SplitData.cpp` | New: autosplitter data struct |
| `desktop_version\src\Input.cpp` | `gameinput()` calls `savestate::input()`; `fadetomode` and `gotomode` made non-static; game over buttons; Super menu |
| `desktop_version\src\Script.cpp` | `startgamemode()` calls `savestate::note_mode()`, `savestate::apply_trial_mode()`, `savestate::apply_trial_countdown()` and `savestate::after_start()` |
| `desktop_version\src\LocalizationStorage.cpp` | `loadtext()` also loads `lang\<code>\super.xml` |
| `desktop_version\src\Logic.cpp` | Death logic calls `savestate::fast_death_exit()` |
| `desktop_version\src\Game.h` | New menu `supermode` |
| `desktop_version\src\Render.cpp` | Message in place of the room name; `GG!` on the NDM complete and time trial results screens; Super menu |
| `desktop_version\src\Graphics.cpp` | `GG!` replaces the Level Complete and Game Complete images |
| `desktop_version\src\main.cpp` | `loop_end()` calls `splitdata::update()` |
| `desktop_version\src\Game.cpp` | Records, achievements and trophies skipped when `savestate::invalid_run()`; game over and Super menus |
| `autosplitter\LiveSplit.VVVVVV.asl` | Autosplitter with `SplitData` support, signature scan added to `init` and a `split` mode in `update` |
| `desktop_version\src\SteamNetwork.c` | Loads `steam_api64.dll` on 64-bit Windows |
| `desktop_version\CMakeLists.txt` | Adds `Savestate.cpp` and `SplitData.cpp` to the sources; embeds `icon.rc` on Windows |

# Building
Requirements:
- Windows 10/11 x64
- [w64devkit](https://github.com/skeeto/w64devkit) (GCC/G++, built with 15.2), found automatically in `Program Files`, `Program Files (x86)` or `C:\w64devkit`; otherwise set `W64DEVKIT` to its folder or add its `bin` to `PATH`
- CMake and Ninja: `pip install cmake ninja`, with Python's `Scripts` folder in `PATH`
- SDL2 2.24.0+ MinGW dev package (x86_64), `SDL2-devel-<version>-mingw.zip` from the [SDL releases](https://github.com/libsdl-org/SDL/releases) (built with 2.32.8), linked statically (`lib\libSDL2.a`); extract it into `SDL32` in the repo root, either as `SDL32\x86_64-w64-mingw32\...` or with `bin`, `include` and `lib` directly inside `SDL32`
- Repo path without spaces, e.g. `C:\vsrc` (spaces break icon embedding)
- `steam_api64.dll` in the repo root (optional, for Steam achievements): copied to `build\`, never added to the release

Run `build.bat [-c] [-i] [-r] [-n] [-s path] [-h]`. Flags combine in any order, e.g. `build.bat -cir`; the path of `-s` follows the flag group, e.g. `build.bat -cs C:\SDL2\x86_64-w64-mingw32`.

| Flag | Action |
| --- | --- |
| `-c` | Clean: delete previous build output first |
| `-i` | Install: copy the build to the Steam game folder, replacing `VVVVVV.exe`, and every `lang\<code>\super.xml` into the game's `lang\<code>\` |
| `-r` | Release: package the build into `release\SuperVVVVVV.zip` (`VVVVVV.exe`, `LiveSplit.VVVVVV.asl`, `SuperVVVVVV-README.md`, `LICENSE.md`, `lang\<code>\super.xml`) and write `release\SHA256.txt` (SHA-256 of the archive and of `VVVVVV.exe`) |
| `-n` | Nobuild: skip the build, reuse existing output (needs `-i` and/or `-r`, not with `-c`) |
| `-s path` | SDL2: `x86_64-w64-mingw32` folder of the SDL2 MinGW package, default `SDL32` in the repo |
| `-h` | Show help, no build |

Build output: `build\VVVVVV.exe`, `build\LiveSplit.VVVVVV.asl`, `build\steam_api64.dll` (if present)

# License
- [MIT](LICENSE.md) (changes by SuperStas0 only)
- VVVVVV (VVVVVV Source Code License v1.0, [full text with third-party licenses](licenses/LICENSE.md))
- SDL2 (zlib)
- FAudio (zlib)
- PhysicsFS (zlib)
- TinyXML-2 (zlib)
- LodePNG (zlib)
- SheenBidi (Apache-2.0)
- C-HashMap (BSD-3-Clause)
