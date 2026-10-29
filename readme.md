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

4. Output: `build\VVVVVV.exe` and `build\SDL2.dll`. Compiler warnings are expected.

From a w64devkit shell, `sh build.sh <absolute path to x86_64-w64-mingw32>` does the same.

## Releasing

After building, run `release.bat`. It creates `release\SuperVVVVVV.zip` containing:

- `SuperVVVVVV.exe` and `SDL2.dll`
- `SuperVVVVVV-README.md`
- `SuperVVVVVV-licenses\` with `LICENSE.md` and `THIRD_PARTY_LICENSES.txt`

The license files must stay in the archive: the VVVVVV license requires its notice to be kept in binary distributions, and two statically linked libraries (SheenBidi, c-hashmap) require theirs.

## What changed from upstream

Based on the VVVVVV 2.4.4 tag (`ea811e1`). Useful when rebasing onto a newer version:

| File | Change |
|---|---|
| `desktop_version/src/Savestate.h`, `Savestate.cpp` | New: savestate logic |
| `desktop_version/src/Input.cpp` | `gameinput()` calls `savestate::input()` |
| `desktop_version/src/Script.cpp` | `startgamemode()` calls `savestate::note_mode()` |
| `desktop_version/src/Game.cpp` | Time trial result recording skipped when `savestate::used()` |
| `desktop_version/CMakeLists.txt` | Adds `Savestate.cpp` to the sources |

## Credits and license

SuperVVVVVV is an **altered version** of VVVVVV, based on the VVVVVV source code, copyright Terry Cavanagh. It is not the original game and is not affiliated with or endorsed by its author.

Provided free of charge for non-commercial use under the terms of the [VVVVVV Source Code License](https://github.com/TerryCavanagh/VVVVVV/blob/master/LICENSE.md) (copy in `SuperVVVVVV-licenses\`). It contains none of the game's assets, which must come from your own copy of the game. VVVVVV is a commercial game; if you enjoy it, please buy it at [thelettervsixtim.es](https://thelettervsixtim.es).

Third-party licenses: `SuperVVVVVV-licenses\THIRD_PARTY_LICENSES.txt`.
