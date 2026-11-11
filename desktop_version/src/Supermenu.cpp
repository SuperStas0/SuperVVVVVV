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
    int options_selected(void)
    {
        int selected = game.currentmenuoption;
        if (supermode::in_options())
        {
            selected--;
        }
        return selected;
    }

    void options_item(void)
    {
        if (supermode::in_options())
        {
            game.option(supertext::get("super"));
        }
    }

    void open(void)
    {
        music.playef(Sound_VIRIDIAN);
        game.createmenu(Menu::supermode);
        map.nexttowercolour();
    }

    void create(void)
    {
        game.option(supertext::get("super mode"));
        game.option(supertext::get("allow savestates"));
        game.option(supertext::get("delete savestates"));
        game.option(supertext::get("load state: `"), false);
        game.option(supertext::get("save state: shift + `"), false);
        game.option(loc::gettext("return"));
        game.menuyoff = -20;
    }

    void input(void)
    {
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
            supermode::delete_states();
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
        font::print(PR_2X | PR_CEN, -1, 30, supertext::get("Super"), r, g, b);

        char buffer[SCREEN_WIDTH_CHARS + 1];
        SDL_snprintf(
            buffer, sizeof(buffer), "%s %s   %s %s (%d)",
            supertext::get("Super mode:"),
            supertext::get(supermode::super_mode() ? "ON" : "OFF"),
            supertext::get("Savestates:"),
            supertext::get(supermode::allow_states() ? "ON" : "OFF"),
            savestate::count()
        );
        font::print_wrap(PR_CEN, -1, 60, buffer, r, g, b);
        font::print_wrap(PR_CEN, -1, 76, supertext::get("NDM + super mode: valid"), r, g, b);
        font::print_wrap(PR_CEN, -1, 88, supertext::get("Time trial + super mode: not valid"), r, g, b);
        font::print_wrap(PR_CEN, -1, 100, supertext::get("Any savestate: not valid"), r, g, b);
    }
}
