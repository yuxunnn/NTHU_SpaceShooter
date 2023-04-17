// [HACKATHON 3-9]
// TODO: Create scene_settings.h and scene_settings.c.
// No need to do anything for this part. We've already done it for
// you, so this 2 files is like the default scene template.
#include "game.h"
#include "scene_settings.h"
#include "scene_menu.h"
#include "utility.h"
#include "shared.h"
// Variables and functions with 'static' prefix at the top level of a
// source file is only accessible in that file ("file scope", also
// known as "internal linkage"). If other files has the same variable
// name, they'll be different variables.

/* Define your static vars / function prototypes below. */
static ALLEGRO_BITMAP* img_plane;
static ALLEGRO_BITMAP* img_plane2;
static ALLEGRO_BITMAP* img_choose;
static ALLEGRO_BITMAP* img_plane_choose;
static ALLEGRO_BITMAP* img_plane2_choose;
static ALLEGRO_BITMAP* img_add;
static ALLEGRO_BITMAP* img_minus;

int PLANE_HP;
// TODO: More variables and functions that will only be accessed
// inside this scene. They should all have the 'static' prefix.
static void init(void) {
    img_plane = load_bitmap_resized("plane.png", SCREEN_W / 4, SCREEN_W / 4);
    img_plane_choose = load_bitmap_resized("plane_choose.png", SCREEN_W / 4, SCREEN_W / 4);
    img_plane2 = load_bitmap_resized("plane2.jfif", SCREEN_W / 4, SCREEN_W / 4);
    img_plane2_choose = load_bitmap_resized("plane2_choose.png", SCREEN_W / 4, SCREEN_W / 4);
    img_choose = load_bitmap_resized("choose.png", SCREEN_W / 8, SCREEN_W / 8);
    img_add = load_bitmap_resized("add.png", SCREEN_W / 16, SCREEN_W / 16);
    img_minus = load_bitmap_resized("minus.png", SCREEN_W / 16 , SCREEN_W / 16);
}
static void on_mouse_down(int btn, int x, int y, int dz) {
    if (btn == 1) {
        if (pnt_in_rect(x, y, SCREEN_W / 4 - SCREEN_W / 8, SCREEN_H / 5, SCREEN_W / 4, SCREEN_W / 4)) character = 0;
        else if (pnt_in_rect(x, y, SCREEN_W / 4 * 3 - SCREEN_W / 8, SCREEN_H / 5, SCREEN_W / 4, SCREEN_W / 4)) character = 1;
        if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W / 2 - SCREEN_W / 16 * 2, SCREEN_H * 9 / 10, SCREEN_W / 16, SCREEN_W / 16) && PLANE_HP > 1)
            PLANE_HP--;
        if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W / 2 + SCREEN_W / 16, SCREEN_H * 9 / 10, SCREEN_W / 16, SCREEN_W / 16) && PLANE_HP < 50)
            PLANE_HP++;
    }
}
static void draw(void) {
    al_clear_to_color(al_map_rgb(80, 100, 215));
    al_draw_text(font_pirulen_50, al_map_rgb(255, 255, 0), SCREEN_W / 2, 30, ALLEGRO_ALIGN_CENTER, "Choose plane");
    al_draw_textf(font_pirulen_50, al_map_rgb(225, 80, 0), SCREEN_W / 2, SCREEN_H * 9 / 10, ALLEGRO_ALIGN_CENTER, "%d", PLANE_HP);
    al_draw_bitmap(img_minus, SCREEN_W / 2 - SCREEN_W / 16*2, SCREEN_H * 9 / 10-10, 0);
    al_draw_bitmap(img_add, SCREEN_W / 2 + SCREEN_W / 16, SCREEN_H * 9 / 10-10, 0);
    if(character == 0) al_draw_bitmap(img_choose, SCREEN_W / 4 - SCREEN_W / 16, SCREEN_H * 3 / 5, 0);
    else if(character == 1) al_draw_bitmap(img_choose, SCREEN_W / 4 * 3- SCREEN_W / 16, SCREEN_H * 3 / 5, 0);
    if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W / 4 - SCREEN_W / 8, SCREEN_H / 5, SCREEN_W / 4, SCREEN_W / 4)) al_draw_bitmap(img_plane_choose, SCREEN_W / 4 - SCREEN_W / 8, SCREEN_H / 5, 0);
    else al_draw_bitmap(img_plane, SCREEN_W / 4 - SCREEN_W / 8, SCREEN_H / 5, 0);
    if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W / 4 * 3- SCREEN_W / 8, SCREEN_H / 5, SCREEN_W / 4, SCREEN_W / 4)) al_draw_bitmap(img_plane2_choose, SCREEN_W / 4 * 3- SCREEN_W / 8, SCREEN_H / 5, 0);
    else al_draw_bitmap(img_plane2, SCREEN_W / 4 * 3- SCREEN_W / 8, SCREEN_H / 5, 0);
}

static void on_key_down(int keycode) {
    // [HACKATHON 3-10]
    // TODO: If active_scene is SCENE_SETTINGS and Backspace is pressed,
    // return to SCENE_MENU.
    if (keycode == ALLEGRO_KEY_BACKSPACE)
       game_change_scene(scene_menu_create());
}

// The only function that is shared across files.
Scene scene_settings_create(void) {
    Scene scene;
    if (PLANE_HP == 0) PLANE_HP = 20;
    memset(&scene, 0, sizeof(Scene));
    scene.name = "Start";
    scene.initialize = &init;
    scene.draw = &draw;
    scene.on_key_down = &on_key_down;
    scene.on_mouse_down = &on_mouse_down;
    // TODO: Register more event callback functions such as keyboard, mouse, ...
    game_log("Settings scene created");
    return scene;
}