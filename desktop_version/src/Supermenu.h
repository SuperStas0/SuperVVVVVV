#ifndef SUPERMENU_H
#define SUPERMENU_H

/* The Super entry of the in-game options menu and the menu behind it. */
namespace supermenu
{
    enum { OPTION_SUPER = -1 };

    /* Index of the selected options menu entry, OPTION_SUPER for the Super entry. */
    int options_selected(void);

    /* Adds the Super entry to the options menu. */
    void options_item(void);

    /* Adds the switch to vanilla entry to the main menu. */
    void main_item(void);

    /* The switch to vanilla entry was chosen. Returns true when the game must
     * quit now: the vanilla exe starts after the game has saved and closed.
     * Returns false after opening a page that says what is missing. */
    bool switch_to_vanilla(void);

    void open(void);
    void create(void);
    void input(void);

    void render_options(int r, int g, int b);
    void render(int r, int g, int b);
}

#endif /* SUPERMENU_H */
