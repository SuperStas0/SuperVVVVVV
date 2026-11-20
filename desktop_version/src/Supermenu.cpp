#include "Supermenu.h"

#include <SDL.h>
#include <algorithm>
#include <stdlib.h>
#include <string>
#include <vector>

#include "Constants.h"
#include "Enums.h"
#include "Font.h"
#include "Game.h"
#include "Localization.h"
#include "Map.h"
#include "Music.h"
#include "Savestate.h"
#include "Supermode.h"
#include "Supertext.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

namespace supermenu
{
    /* The delete page is the same menu as the Super menu, created again. */
    enum Page
    {
        PAGE_MAIN,
        PAGE_DELETE,
        PAGE_VANILLA
    };

    enum VanillaStatus
    {
        VANILLA_OK,
        VANILLA_MISSING,
        VANILLA_MODDED
    };

    static Page page = PAGE_MAIN;
    static VanillaStatus vanilla_status = VANILLA_MISSING;

    /* The original exe, next to the running exe. */
    static const char VANILLA_EXE[] = "vanilla.exe";
    static const char SPLIT_MARKER[] = "SUPERVVVVVV_SPLIT";

    static std::string vanilla_path;

    static const int DELETE_ALL = supermode::CATEGORIES;
    static const int DELETE_RETURN = supermode::CATEGORIES + 1;

    static const char* const DELETE_LABELS[supermode::CATEGORIES] = {
        "delete: main game",
        "delete: time trials",
        "delete: intermissions",
        "delete: No Death Mode"
    };

    static void show(const Page next, const int option)
    {
        page = next;
        game.createmenu(Menu::supermode, true);
        game.currentmenuoption = option;
    }

    static VanillaStatus check_vanilla(std::string& path)
    {
        char* base = SDL_GetBasePath();
        if (base == NULL)
        {
            return VANILLA_MISSING;
        }
        path = base;
        path += VANILLA_EXE;
        SDL_free(base);

        SDL_RWops* file = SDL_RWFromFile(path.c_str(), "rb");
        if (file == NULL)
        {
            return VANILLA_MISSING;
        }

        const Sint64 size = SDL_RWsize(file);
        if (size <= 0)
        {
            SDL_RWclose(file);
            return VANILLA_MISSING;
        }

        std::vector<char> content(static_cast<size_t>(size));
        const size_t got = SDL_RWread(file, &content[0], 1, content.size());
        SDL_RWclose(file);
        if (got != content.size())
        {
            return VANILLA_MISSING;
        }

        const char* marker_end = SPLIT_MARKER + sizeof(SPLIT_MARKER) - 1;
        if (std::search(content.begin(), content.end(), SPLIT_MARKER, marker_end) != content.end())
        {
            return VANILLA_MODDED;
        }
        return VANILLA_OK;
    }

#ifdef _WIN32
    static std::wstring widen(const std::string& text)
    {
        const int length = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, NULL, 0);
        if (length <= 0)
        {
            return std::wstring();
        }
        std::vector<wchar_t> buffer(length);
        MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, &buffer[0], length);
        return std::wstring(&buffer[0]);
    }
#endif

    /* Runs at exit, after the game saved and released its files and Steam. The
     * new process keeps the environment, so Steam still sees the same game. */
    static void start_vanilla(void)
    {
#ifdef _WIN32
        const std::wstring exe = widen(vanilla_path);
        const std::string::size_type slash = vanilla_path.find_last_of("\\/");
        if (exe.empty() || slash == std::string::npos)
        {
            return;
        }
        const std::wstring folder = widen(vanilla_path.substr(0, slash));

        std::wstring command = L"\"" + exe + L"\"";
        std::vector<wchar_t> buffer(command.begin(), command.end());
        buffer.push_back(L'\0');

        STARTUPINFOW startup;
        PROCESS_INFORMATION info;
        ZeroMemory(&startup, sizeof(startup));
        ZeroMemory(&info, sizeof(info));
        startup.cb = sizeof(startup);

        if (CreateProcessW(exe.c_str(), &buffer[0], NULL, NULL, FALSE, 0, NULL, folder.c_str(), &startup, &info))
        {
            CloseHandle(info.hThread);
            CloseHandle(info.hProcess);
        }
#endif
    }

    void main_item(void)
    {
        game.option(supertext::get("switch to vanilla"));
    }

    bool switch_to_vanilla(void)
    {
#ifdef _WIN32
        vanilla_status = check_vanilla(vanilla_path);
#else
        vanilla_status = VANILLA_MISSING;
#endif

        if (vanilla_status != VANILLA_OK)
        {
            music.playef(Sound_VIRIDIAN);
            page = PAGE_VANILLA;
            game.createmenu(Menu::supermode);
            map.nexttowercolour();
            return false;
        }

        atexit(start_vanilla);
        return true;
    }

    int options_selected(void)
    {
        /* The Super entry is the first one. */
        return game.currentmenuoption - 1;
    }

    void options_item(void)
    {
        game.option(supertext::get("super"));
    }

    void open(void)
    {
        music.playef(Sound_VIRIDIAN);
        page = PAGE_MAIN;
        game.createmenu(Menu::supermode);
        map.nexttowercolour();
    }

    void create(void)
    {
        if (page == PAGE_VANILLA)
        {
            game.option(loc::gettext("return"));
            game.menuyoff = 0;
            return;
        }

        if (page == PAGE_DELETE)
        {
            for (int category = 0; category < supermode::CATEGORIES; category++)
            {
                char label[SCREEN_WIDTH_CHARS + 1];
                SDL_snprintf(
                    label, sizeof(label), "%s (%d)",
                    supertext::get(DELETE_LABELS[category]),
                    supermode::category_states(category)
                );
                game.option(label);
            }
            game.option(supertext::get("delete all savestates"));
            game.option(loc::gettext("return"));
            game.menuyoff = 0;
            return;
        }

        game.option(supertext::get("super mode"));
        game.option(supertext::get("allow savestates"));
        game.option(supertext::get("delete savestates"));
        game.option(supertext::get("load state: 1-9"), false);
        game.option(supertext::get("save state: shift + 1-9"), false);
        game.option(loc::gettext("return"));
        game.menuyoff = 0;
    }

    static void input_delete(void)
    {
        const int option = game.currentmenuoption;

        if (option == DELETE_RETURN)
        {
            music.playef(Sound_VIRIDIAN);
            show(PAGE_MAIN, 2);
            return;
        }

        const int states = option == DELETE_ALL
            ? savestate::count()
            : supermode::category_states(option);
        if (states == 0)
        {
            music.playef(Sound_CRY);
            return;
        }

        music.playef(Sound_VIRIDIAN);
        if (option == DELETE_ALL)
        {
            supermode::delete_states();
        }
        else
        {
            supermode::delete_category(option);
        }
        show(PAGE_DELETE, option);
    }

    void input(void)
    {
        if (page == PAGE_VANILLA)
        {
            music.playef(Sound_VIRIDIAN);
            game.returnmenu();
            map.nexttowercolour();
            return;
        }

        if (page == PAGE_DELETE)
        {
            input_delete();
            return;
        }

        switch (game.currentmenuoption)
        {
        case 0:
            music.playef(Sound_VIRIDIAN);
            supermode::toggle_super();
            break;
        case 1:
            music.playef(Sound_VIRIDIAN);
            supermode::toggle_allow();
            break;
        case 2:
            music.playef(Sound_VIRIDIAN);
            show(PAGE_DELETE, 0);
            break;
        case 3:
        case 4:
            music.playef(Sound_CRY);
            break;
        default:
            music.playef(Sound_VIRIDIAN);
            game.returnmenu();
            map.nexttowercolour();
            break;
        }
    }

    void render_options(const int r, const int g, const int b)
    {
        font::print(PR_2X | PR_CEN, -1, 30, supertext::get("Super"), r, g, b);
        font::print_wrap(PR_CEN, -1, 65, supertext::get("Super mode and savestates."), r, g, b);
    }

    void render(const int r, const int g, const int b)
    {
        char buffer[SCREEN_WIDTH_CHARS + 1];

        if (page == PAGE_VANILLA)
        {
            font::print(PR_2X | PR_CEN, -1, 30, supertext::get("Switch to vanilla"), r, g, b);
            font::print_wrap(
                PR_CEN, -1, 65,
                supertext::get(vanilla_status == VANILLA_MODDED
                    ? "vanilla.exe is not the original game. Copy the original VVVVVV.exe there under that name."
                    : "vanilla.exe was not found next to VVVVVV.exe. Copy the original VVVVVV.exe there under that name."),
                r, g, b
            );
            return;
        }

        if (page == PAGE_DELETE)
        {
            font::print(PR_2X | PR_CEN, -1, 30, supertext::get("Delete savestates"), r, g, b);
            SDL_snprintf(buffer, sizeof(buffer), "%s (%d)", supertext::get("Savestates:"), savestate::count());
            font::print_wrap(PR_CEN, -1, 56, buffer, r, g, b);
            return;
        }

        font::print(PR_2X | PR_CEN, -1, 30, supertext::get("Super"), r, g, b);

        SDL_snprintf(
            buffer, sizeof(buffer), "%s %s   %s %s (%d)",
            supertext::get("Super mode:"),
            supertext::get(supermode::super_mode() ? "ON" : "OFF"),
            supertext::get("Savestates:"),
            supertext::get(supermode::allow_states() ? "ON" : "OFF"),
            savestate::count()
        );

        font::print_wrap(PR_CEN, -1, 56, buffer, r, g, b);
    }
}
