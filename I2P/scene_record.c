#include "game.h"
#include "scene_game_over.h"
#include "scene_menu.h"
#include "utility.h"
#include "scene_start.h"
#include "shared.h"
#include <stdio.h>
FILE* RECORD;
static int record_score[15];
static float record_time[15];
static ALLEGRO_BITMAP* img_universe;
static void init(void) {
    img_universe = load_bitmap_resized("universe.png", SCREEN_W, SCREEN_H);
}
static void draw(void) {
    al_draw_bitmap(img_universe, 0, 0, 0);
    al_draw_text(font_pirulen_50, al_map_rgb(255, 255, 0), SCREEN_W / 2, 30, ALLEGRO_ALIGN_CENTER, "Record");
    al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 255), SCREEN_W / 2 - 300 + 100, 140, ALLEGRO_ALIGN_CENTER, "Rate");
    al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 255), SCREEN_W / 2 - 100 + 100, 140, ALLEGRO_ALIGN_CENTER, "Score");
    al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 255), SCREEN_W / 2 + 100 + 100, 140, ALLEGRO_ALIGN_CENTER, "Time");
    for (int i = 0; i < 10; i++) {
        al_draw_textf(font_pirulen_32, al_map_rgb(255, 255, 255), SCREEN_W / 2 - 300 + 100, i * 50 + 200, ALLEGRO_ALIGN_CENTER, "%2d", i + 1);
        al_draw_textf(font_pirulen_24, al_map_rgb(255, 255, 255), SCREEN_W / 2 - 100 + 100, i * 50 + 200, ALLEGRO_ALIGN_CENTER, "%2d", record_score[i]);
        al_draw_textf(font_pirulen_24, al_map_rgb(255, 255, 255), SCREEN_W / 2 + 100 + 100, i * 50 + 200, ALLEGRO_ALIGN_CENTER, "%.1f", record_time[i]);
    }
}
static void on_key_down(int keycode) {
    // [HACKATHON 3-10]
    // TODO: If active_scene is SCENE_SETTINGS and Backspace is pressed,
    // return to SCENE_MENU.
    if (keycode == ALLEGRO_KEY_BACKSPACE)
        game_change_scene(scene_menu_create());
}
Scene scene_record_create(void) {
    Scene scene;
    RECORD = fopen("record.txt", "r");
    for (int i = 0; i < 10; i++) {
        fscanf(RECORD, "%d %f", &record_score[i], &record_time[i]);
    }
    fclose(RECORD);
    memset(&scene, 0, sizeof(Scene));
    scene.name = "Record";
    scene.initialize = &init;
    scene.draw = &draw;
    scene.on_key_down = &on_key_down;
    // TODO: Register more event callback functions such as keyboard, mouse, ...
    game_log("Record scene created");
    return scene;
}