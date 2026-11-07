# SuperVVVVVV

Practice patch for VVVVVV (Windows, Steam version) built on the VVVVVV 2.4.4 source. It adds in-memory savestates for time trials and intermission replays.

## Savestates

| Key | Action |
|---|---|
| `Shift` + `` ` `` (`~`) | Save state |
| `` ` `` (`~`) | Load state |

The key is the one left of `1`.

- Saves the full in-game state: time, trinkets, room, position, checkpoint, gravity, entities and script progress.
- One slot, kept in memory only (lost when the game closes). No on-screen indicators.
- Active only in **Time Trials** and **Intermission replays**. Everywhere else, including No Death Mode, the keys do nothing.
- A state loads only in the same trial or intermission it was saved in.
- Saving is ignored during the death animation.
- A trial run that loaded a state does not record its result (best time, rank, trinkets, deaths), so practice can't overwrite real records.

## Install

Requires your own copy of VVVVVV. The game's `data.zip` is not included.

1. Extract the archive into the game folder (`...\steamapps\common\VVVVVV`), next to the original `VVVVVV.exe` and `data.zip`. The original game is not overwritten.
2. Run `SuperVVVVVV.exe`.

It uses the same save and settings folder as the original game.

### Steam

- **Separate entry:** Steam > Games > Add a Non-Steam Game to My Library > Browse > select `SuperVVVVVV.exe`. Counts hours, and the overlay should work.
- **Hours on the real game:** rename the original `VVVVVV.exe` (backup), then rename `SuperVVVVVV.exe` to `VVVVVV.exe`. A Steam update or "Verify integrity" restores the original.
- Steam achievements and cloud saves do not work, because this build has Steam API support off.

## Building

### Requirements

| Tool | Notes |
|---|---|
| Windows 10/11 x64 | |
| [w64devkit](https://github.com/skeeto/w64devkit) | GCC/G++ (built with 15.2). Found automatically in `Program Files`, `Program Files (x86)` or `C:\w64devkit`; otherwise set `W64DEVKIT` to its folder or add its `bin` to `PATH` |
| CMake + Ninja | `pip install cmake ninja`; Python's `Scripts` folder must be in `PATH` |
| SDL2 2.24.0+ MinGW dev package | `SDL2-devel-<version>-mingw.zip` from the [SDL releases](https://github.com/libsdl-org/SDL/releases) (built with 2.32.8) |

### Steps

1. Put the repo in a path **without spaces** (e.g. `C:\vsrc`). Paths with spaces break icon embedding.
2. Extract the SDL2 MinGW package into `SDL32` in the repo root, either as `SDL32\x86_64-w64-mingw32\...` or with `bin`, `include` and `lib` directly inside `SDL32` (or pass its location to the script).
3. Run in `cmd`:

   ```
   build.bat
   ```

   or with a custom SDL2 location:

   ```
   build.bat C:\path\to\SDL2\x86_64-w64-mingw32
   ```

4. Output: `build\VVVVVV.exe`, `build\SDL2.dll` and `build\LiveSplit.VVVVVV.asl`. Compiler warnings are expected.

From a w64devkit shell, `sh build.sh <absolute path to x86_64-w64-mingw32>` does the same.

## Releasing

After building, run `release.bat`. It creates `release\SuperVVVVVV.zip` containing:

- `SuperVVVVVV.exe` and `SDL2.dll`
- `LiveSplit.VVVVVV.asl`
- `SuperVVVVVV-README.md`

## Autosplitter

`LiveSplit.VVVVVV.asl` is the VVVVVV autosplitter with SuperVVVVVV support. Start, split, reset, game time and settings are unchanged. Its source is `autosplitter\LiveSplit.VVVVVV.asl`; `build.bat` copies it to `build\` and `release.bat` adds it to the release zip.

### Setup

1. LiveSplit: right-click > Edit Layout > add **Control > Scriptable Auto Splitter**.
2. Layout Settings > Scriptable Auto Splitter > **Script Path**: select `LiveSplit.VVVVVV.asl`.
3. Choose the splits in the same settings page.

### Detection

| Game | How it is detected |
|---|---|
| SuperVVVVVV or any modded build (process `SuperVVVVVV.exe` or `VVVVVV.exe`) | Scans the exe for the `SUPERVVVVVV_SPLIT` marker and reads the `SplitData` struct. Works for any exe size. |
| Vanilla VVVVVV 2.0-2.4.4 | Original module size detection and offsets |
| Anything else | Inactive |

### Changing the game

- `SplitData.cpp` mirrors the game variables each frame in `loop_end()`. The struct is append-only: never move, resize or remove fields.
- New field: add it at the end of the struct, then read it in the `update` block of the `.asl`.
- Incompatible layout: change the marker in `SplitData.cpp` and in the `.asl` `init` block, so older scripts stay inactive instead of reading wrong data.
- Other exe name: add `state("<name>", "split") {}` and `state("<name>", "unknown") {}` to the `.asl`.

## What changed from upstream

Based on the VVVVVV 2.4.4 tag (`ea811e1`). Useful when rebasing onto a newer version:

| File | Change |
|---|---|
| `desktop_version/src/Savestate.h`, `Savestate.cpp` | New: savestate logic |
| `desktop_version/src/SplitData.h`, `SplitData.cpp` | New: autosplitter data struct |
| `desktop_version/src/Input.cpp` | `gameinput()` calls `savestate::input()`; `fadetomode` and `gotomode` made non-static |
| `desktop_version/src/Script.cpp` | `startgamemode()` calls `savestate::note_mode()` |
| `desktop_version/src/main.cpp` | `loop_end()` calls `splitdata::update()` |
| `desktop_version/src/Game.cpp` | Time trial result recording skipped when `savestate::used()` |
| `autosplitter/LiveSplit.VVVVVV.asl` | Autosplitter with `SplitData` support, signature scan added to `init` and a `split` mode in `update` |
| `desktop_version/CMakeLists.txt` | Adds `Savestate.cpp` and `SplitData.cpp` to the sources |
