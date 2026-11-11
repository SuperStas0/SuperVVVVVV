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

    void open(void);
    void create(void);
    void input(void);

    void render_options(int r, int g, int b);
    void render(int r, int g, int b);
}

#endif /* SUPERMENU_H */
