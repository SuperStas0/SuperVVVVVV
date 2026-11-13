# SuperVVVVVV

Altered version of [VVVVVV](https://github.com/TerryCavanagh/VVVVVV) (copyright Terry Cavanagh, based on the 2.4.4 source code), not the original game: practice patch for Windows (Steam version) with savestates for time trials, intermission replays and No Death Mode, and a Super mode with fast restarts.

# Installation
Requires your own copy of VVVVVV on Steam.

1. In the game folder (`...\steamapps\common\VVVVVV`), copy the original `VVVVVV.exe` to a backup, e.g. `VVVVVV-vanilla.exe`.
2. Extract the release archive into the game folder, overwriting `VVVVVV.exe`.
3. Launch the game from Steam.

If you use LiveSplit, use `LiveSplit.VVVVVV.asl` from the release archive as the autosplitter, see Autosplitter.

To go back to vanilla, put the backup back as `VVVVVV.exe`, or use Steam > Properties > Installed Files > Verify integrity of game files. A Steam update also restores it.

It uses the same save and settings folder as the original game.

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
| `Shift` + `1` `2` `3` | Save state 1, 2 or 3 |
| `1` `2` `3` | Load state 1, 2 or 3 |

The keys are the number row. Ctrl or Alt held does nothing.

- Off by default. Turn them on with `allow savestates` in the Super menu.
- Saves the full in-game state: trinkets, room, position, checkpoint, gravity, entities, script progress and music. The timer is not saved or loaded, it keeps running.
- Active in the main game, time trials, intermission replays and No Death Mode.
- Three slots per mode: the main game (new game, continue and quicksave share it), each time trial, each intermission replay, No Death Mode with cutscenes and without. Time trials and intermissions with Super mode on have their own slots.
- Slots are kept while the game runs, also across leaving and entering levels. Nothing is saved to disk.
- A state loads only in the mode it was saved in. Saving is ignored during the death animation and during the countdown after a load.
- Save and load show a message in place of the room name, for example `State 1 saved`, `State 2 loaded`, `State 3 empty`.

## Super menu
The first entry of the options menu, in the main menu and in the game (`Esc` > `options`). Everything is off when the game starts.

| Item | Action |
| --- | --- |
| `super mode` | Toggle Super mode. In a time trial or intermission it restarts the level. |
| `allow savestates` | Toggle the save and load keys and loading a state on death. |
| `delete savestates` | Delete all states in all modes. |

## Dying with a state
Applies where dying would end the run: No Death Mode, and time trials and intermission replays with Super mode on. Needs `allow savestates` and a state for the current mode. It loads the state that was saved or loaded last, of any slot. The main game and time trials without Super mode keep the original respawn.

| Super mode | Delay | After the load |
| --- | --- | --- |
| off | original death delay (1 s) | 1 s countdown without control, then `Go!` |
| on | 0.5 s | none |

With a state the music is not faded out on death. Loading a state restarts the music only if its song differs from the one playing.

Without a state (or with savestates off), No Death Mode ends the run as in the original game, except with Super mode on: see below.

## Super mode
- Main game: adds `reset run` to the pause menu (see below). It does not make a run invalid.
- No Death Mode: dying with a state loads it (see above). Without a state, 0.5 s after dying the game leaves the run without the fade and the game over screen, and opens the cutscene choice screen of No Death Mode. Gameplay is not affected.
- Time trials and intermission replays: No Death Mode rules (no checkpoints, no cutscenes). Without a state, dying restarts the level at its start 0.5 s later, without the fade. Time trials use a 1 s countdown. The timer restarts.

## Reset run
In the main game with Super mode on, the pause menu (`Esc`) has `reset run` below `quit to menu`. It is there only in a main game run and only if the game was completed once (the Game Complete trophy). It has no confirmation.

It deletes the teleporter and quick save files, unlocks everything of `accessibility` > `unlock play modes` (all time trials, intermission replays, No Death Mode, flip mode, ship jukebox, secret lab) and goes to the menu without the fade out. That is the play menu, one step before a new run. Savestates are kept, so a new run with states is invalid as described below.

## Valid runs
A run is invalid if any of these is true:
- a state was loaded in it
- a state exists for its mode (main game: any state of the main game; No Death Mode: for either cutscene choice)
- it is a time trial or intermission replay with Super mode on

Invalid runs show `GG!` in place of `Level Complete!` and `Game Complete!` (main game and No Death Mode), `Results` (time trials) and `WOW` (No Death Mode). From the moment a run is invalid it earns no achievements, trophies, unlocks, trinket stats, Super Gravitron records, best death count or time trial records. What was earned before the first state was saved stays. Use `delete savestates` before a real run.

Super mode in No Death Mode does not make a run invalid.

# Translations
Texts added by SuperVVVVVV are translated with `lang\<code>\super.xml`, in the same format as the game's `strings.xml`. Missing or empty translations stay English. Official texts stay in the game's own `strings.xml`.

`desktop_version\lang` has one folder per game language, each with only `super.xml`. All of them are copies of the English file except `uk`.

To translate a language:
1. Open `desktop_version\lang\<code>\super.xml`.
2. Fill the `translation` attributes. Keep the `english` attributes unchanged.
3. Run `build.bat -r`, or copy the file into the game's `lang\<code>\` folder.

For a language without a folder, create `desktop_version\lang\<code>\` and copy `super.xml` from `en`. `<code>` is the folder name the game uses for that language, and the language must already exist in the game.

# Autosplitter
`LiveSplit.VVVVVV.asl` is the VVVVVV autosplitter with SuperVVVVVV support. Game time and settings are unchanged. Start, split and reset work as in the original, except in invalid runs (see Invalid runs). Its source is `autosplitter\LiveSplit.VVVVVV.asl`; `build.bat` copies it to `build\` and `-r` adds it to the release archive.

Based on [tzann/Autosplitters](https://github.com/tzann/Autosplitters).

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

## Invalid runs
With a SuperVVVVVV build, an invalid run (see Valid runs) is handled like this:
- Invalid from the start: the timer does not start.
- Invalid after the start: the timer keeps running and is never split or reset automatically. Split and reset it manually. This lasts until the timer is started again.
- Use `VVVVVV.exe` and `LiveSplit.VVVVVV.asl` from the same release.

## Changing the game
- `SplitData.cpp` mirrors the game variables each frame in `loop_end()`. The struct is append-only: never move, resize or remove fields.
- New field: add it at the end of the struct, then read it in the `update` block of the `.asl`.
- Incompatible layout: change the marker in `SplitData.cpp` and in the `.asl` `init` block, so older scripts stay inactive instead of reading wrong data.
- Other exe name: duplicate the `VVVVVV` `split` and `unknown` states in the `.asl` with that process name.

# Changes from vanilla
Based on VVVVVV 2.4.4. The own code is in `Savestate`, `Supermode`, `Supermenu`, `Supertext` and `SplitData` in `desktop_version\src`. Other files in that folder only get small hooks into it.

`changes.bat` shows what differs from the vanilla source in `desktop_version\src\`. It needs git, adds the remote `vanilla` (`https://github.com/TerryCavanagh/VVVVVV.git`) if it is missing and fetches the tag. It compares the last commit, so commit first.

| Command | Action |
| --- | --- |
| `changes.bat` | Write `changesFromVanilla.txt` (file list and full diff, ignored by git) |
| `changes.bat -b` | Create the branch `compare-vanilla` to browse the changes in GitHub Desktop |
| `changes.bat -d` | Delete the branch `compare-vanilla` |
| `changes.bat -f` | Full: all files, not only `desktop_version\src\`. Works with the file and with `-b`. |
| `changes.bat -t <tag>` | Compare with another vanilla tag, default `2.4.4` |
| `changes.bat -h` | Show help |

How `-b` works: git stores a full snapshot of the files in every commit, and the changes shown for a commit are its snapshot minus its parent's snapshot. The script makes two commits on the branch `compare-vanilla`. The first is the vanilla snapshot of the chosen tag: only `desktop_version\src\` comes from vanilla and everything else is your files, or with `-f` the whole vanilla tag. The second has exactly the files of your last commit. Its changes are therefore what turns vanilla into your version. Your own branch is not touched.

In GitHub Desktop:
1. Commit your changes and run `changes.bat -b`.
2. Current Branch > `compare-vanilla`. The branch has the same files as your last commit, so nothing changes on disk.
3. Open the History tab and select the top commit. Its changed files and diffs are the differences from vanilla.
4. Switch back to your branch and run `changes.bat -d`.

Flags combine, for example `changes.bat -fb`.

Same steps by hand: `git remote add vanilla https://github.com/TerryCavanagh/VVVVVV.git`, `git fetch vanilla tag 2.4.4`, `git diff --stat 2.4.4 HEAD -- desktop_version/src/`.

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
- [MIT](LICENSE.md)
- VVVVVV (VVVVVV Source Code License v1.0, [full text](licenses/LICENSE.md))
- SDL2 (zlib)
- FAudio (zlib)
- PhysicsFS (zlib)
- TinyXML-2 (zlib)
- LodePNG (zlib)
- SheenBidi (Apache-2.0)
- C-HashMap (BSD-3-Clause)
- LiveSplit.VVVVVV.asl (https://github.com/tzann/Autosplitters)
