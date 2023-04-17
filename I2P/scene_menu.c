#include "scene_menu.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_acodec.h>
#include "game.h"
#include "utility.h"
#include "shared.h"
#include "scene_start.h"
#include "scene_settings.h"
#include "scene_record.h"
#include <stdio.h>

// Variables and functions with 'static' prefix at the top level of a
// source file is only accessible in that file ("file scope", also
// known as "internal linkage"). If other files has the same variable
// name, they'll be different variables.

/* Define your static vars / function prototypes below. */

// TODO: More variables and functions that will only be accessed
// inside this scene. They should all have the 'static' prefix.

static const char* txt_title = "Space Shooter";
static const char* txt_info = "Press enter key to start";
static ALLEGRO_BITMAP* img_background;
// [HACKATHON 3-1]
// TODO: Declare 2 variables for storing settings images.
// Uncomment and fill in the code below.
static ALLEGRO_BITMAP* img_settings;
static ALLEGRO_BITMAP* img_settings2;
static ALLEGRO_BITMAP* img_record;
static ALLEGRO_BITMAP* img_record2;
static ALLEGRO_BITMAP* img_money;
static ALLEGRO_SAMPLE* bgm;
static ALLEGRO_SAMPLE_ID bgm_id;
static void init(void);
static void draw(void);
static void destroy(void);
static void on_key_down(int keycode);
int character, money;
FILE* MONEY;

static void init(void) {
    img_background = load_bitmap_resized("main-bg.jpg", SCREEN_W, SCREEN_H);
    // [HACKATHON 3-4]
    // TODO: Load settings images.
    // Uncomment and fill in the code below.
    img_settings = al_load_bitmap("settings.png");
    img_settings2 = al_load_bitmap("settings2.png");
    img_record = load_bitmap_resized("record.png", 38, 38);
    img_record2 = load_bitmap_resized("record2.png", 38, 38);
    img_money = load_bitmap_resized("money.png", 50, 50);
    // Can be moved to shared_init to decrease loading time.
    bgm = load_audio("S31-Night Prowler.ogg");
    bgm_id = play_bgm(bgm, 1);
    game_log("Menu scene initialized");
}

static void draw(void) {
    al_draw_bitmap(img_background, 0, 0, 0);
    // [HACKATHON 3-5]
    // TODO: Draw settings images.
    // The settings icon should be located at (x, y, w, h) =
    // (SCREEN_W - 48, 10, 38, 38).
    // Change its image according to your mouse position.
    // Uncomment and fill in the code below.
    if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W - 48, 58, 38, 38))
        al_draw_bitmap(img_record2, SCREEN_W - 48, 58, 0);
    else
        al_draw_bitmap(img_record, SCREEN_W - 48, 58, 0);
    if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W-48, 10, 38, 38))
        al_draw_bitmap(img_settings2, SCREEN_W-48, 10, 0);
    else
        al_draw_bitmap(img_settings, SCREEN_W-48, 10, 0);
    al_draw_text(font_pirulen_50, al_map_rgb(255, 255, 0), SCREEN_W / 2, 30, ALLEGRO_ALIGN_CENTER, txt_title);
    al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), 20, SCREEN_H - 50, 0, txt_info);
    al_draw_bitmap(img_money, 20, 22, 0);
    al_draw_text(font_pirulen_18, al_map_rgb(255, 255, 255), 80, 35, 0, "x");
    al_draw_textf(font_pirulen_24, al_map_rgb(255, 255, 0), 105, 30, 0, "%d", money);
}

static void destroy(void) {
    al_destroy_sample(bgm);
    al_destroy_bitmap(img_background);
    // [HACKATHON 3-6]
    // TODO: Destroy the 2 settings images.
    // Uncomment and fill in the code below.
    al_destroy_bitmap(img_settings);
    al_destroy_bitmap(img_settings2);
    al_destroy_bitmap(img_money);
    al_destroy_bitmap(img_record);
    al_destroy_bitmap(img_record2);
    stop_bgm(bgm_id);
    game_log("Menu scene destroyed");
}

static void on_key_down(int keycode) {
    if (keycode == ALLEGRO_KEY_ENTER)
        game_change_scene(scene_start_create());
}

// [HACKATHON 3-7]
// TODO: When settings clicked, switch to settings scene.
// Uncomment and fill in the code below.
static void on_mouse_down(int btn, int x, int y, int dz) {
    if (btn == 1) {
        if (pnt_in_rect(x, y, SCREEN_W-48, 10, 38, 38))
            game_change_scene(scene_settings_create());
        if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W - 48, 58, 38, 38))
            game_change_scene(scene_record_create());
    }
}

// TODO: Add more event callback functions such as update, ...

// Functions without 'static', 'extern' prefixes is just a normal
// function, they can be accessed by other files using 'extern'.
// Define your normal function prototypes below.

// The only function that is shared across files.
Scene scene_menu_create(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));
    MONEY = fopen("money.txt", "r");
    fscanf(MONEY, "%d", &money);
    fclose(MONEY);
    scene.name = "Menu";
    scene.initialize = &init;
    scene.draw = &draw;
    scene.destroy = &destroy;
    scene.on_key_down = &on_key_down;
    // [HACKATHON 3-8]
    // TODO: Register on_mouse_down.
    // Uncomment the code below.
    scene.on_mouse_down = &on_mouse_down;
    // TODO: Register more event callback functions such as update, mouse, ...
    game_log("Menu scene created");
    return scene;
}
