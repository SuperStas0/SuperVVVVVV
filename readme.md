# SuperVVVVVV

Practice patch for VVVVVV (Windows, Steam version) built on the VVVVVV 2.4.4 source, adding in-memory savestates for time trials and intermission replays.

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
- A time trial run that loaded a state never unlocks achievements, same as its record. At startup the game re-sends the achievements your save data already implies, as the original does.

# Usage
## Savestates
| Key | Action |
| --- | --- |
| `Shift` + `` ` `` (`~`) | Save state |
| `` ` `` (`~`) | Load state |

The key is the one left of `1`.

- Saves the full in-game state: time, trinkets, room, position, checkpoint, gravity, entities and script progress.
- One slot, kept in memory only (lost when the game closes). No on-screen indicators.
- Active only in Time Trials and Intermission replays. Everywhere else, including No Death Mode, the keys do nothing.
- A state loads only in the same trial or intermission it was saved in.
- Saving is ignored during the death animation.
- A trial run that loaded a state does not record its result (best time, rank, trinkets, deaths), so practice can't overwrite real records.

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
| `desktop_version\src\Input.cpp` | `gameinput()` calls `savestate::input()`; `fadetomode` and `gotomode` made non-static |
| `desktop_version\src\Script.cpp` | `startgamemode()` calls `savestate::note_mode()` |
| `desktop_version\src\main.cpp` | `loop_end()` calls `splitdata::update()` |
| `desktop_version\src\Game.cpp` | Time trial result recording skipped when `savestate::used()` |
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
- `steam_api64.dll` in the repo root (optional, for Steam achievements): copied to `build\`, added to the release by `-r`

Run `build.bat [-c] [-i] [-r] [-n] [-s path] [-h]`. Flags combine in any order, e.g. `build.bat -cir`; the path of `-s` follows the flag group, e.g. `build.bat -cs C:\SDL2\x86_64-w64-mingw32`.

| Flag | Action |
| --- | --- |
| `-c` | Clean: delete previous build output first |
| `-i` | Install: copy the build to the Steam game folder, replacing `VVVVVV.exe` |
| `-r` | Release: package the build into `release\SuperVVVVVV.zip` (`VVVVVV.exe`, `steam_api64.dll` if present, `LiveSplit.VVVVVV.asl`, `SuperVVVVVV-README.md`) |
| `-n` | Nobuild: skip the build, reuse existing output (needs `-i` and/or `-r`, not with `-c`) |
| `-s path` | SDL2: `x86_64-w64-mingw32` folder of the SDL2 MinGW package, default `SDL32` in the repo |
| `-h` | Show help, no build |

Build output: `build\VVVVVV.exe`, `build\LiveSplit.VVVVVV.asl`, `build\steam_api64.dll` (if present)

# License
- [MIT](LICENSE.md)
- VVVVVV (VVVVVV Source Code License v1.0)
- SDL2 (zlib)
- FAudio (zlib)
- PhysicsFS (zlib)
- TinyXML-2 (zlib)
- LodePNG (zlib)
- SheenBidi (Apache-2.0)
- C-HashMap (BSD-3-Clause)
