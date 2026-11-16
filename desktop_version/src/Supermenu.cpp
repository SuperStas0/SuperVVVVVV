#include "Supermenu.h"

#include <SDL.h>

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

namespace supermenu
{
    /* The delete page is the same menu as the Super menu, created again. */
    enum Page
    {
        PAGE_MAIN,
        PAGE_DELETE
    };

    static Page page = PAGE_MAIN;

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

        /* Long translations wrap, so each line starts where the last one ended. */
        int y = 56;
        y = font::print_wrap(PR_CEN, -1, y, buffer, r, g, b) + 6;
        y = font::print_wrap(PR_CEN, -1, y, supertext::get("NDM + super mode: valid"), r, g, b) + 2;
        y = font::print_wrap(PR_CEN, -1, y, supertext::get("Time trial + super mode: not valid"), r, g, b) + 2;
        font::print_wrap(PR_CEN, -1, y, supertext::get("Savestates on in a run: not valid"), r, g, b);
    }
}
