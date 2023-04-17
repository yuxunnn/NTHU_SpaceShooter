#include "game.h"
#include "scene_game_over.h"
#include "scene_menu.h"
#include "utility.h"
#include "scene_start.h"
#include "shared.h"
static ALLEGRO_BITMAP* img_gameover_backgroung;
static ALLEGRO_BITMAP* img_money;
int score, total_bullets;
float time_used;
FILE* MONEY;
int money, earn_money;
FILE* RECORD;
static int record_score[15];
static float record_time[15];

static void init(void) {
    img_gameover_backgroung = load_bitmap_resized("1.jpg", SCREEN_W, SCREEN_H);
    img_money = load_bitmap_resized("money.png", 50, 50);
}
static void on_mouse_down(int btn, int x, int y, int dz) {
    if (btn == 1) {
        if (pnt_in_rect(x, y, 0, 0, SCREEN_W, SCREEN_H))
            game_change_scene(scene_menu_create());
    }
}
static void draw(void) {
    al_draw_bitmap(img_gameover_backgroung, 0, 0, 0);
    al_draw_text(font_pirulen_50, al_map_rgb(255, 255, 0), SCREEN_W / 2, 30, ALLEGRO_ALIGN_CENTER, "GAME OVER");

    al_draw_textf(font_pirulen_32, al_map_rgb(255, 255, 255), SCREEN_W / 2 - 140, SCREEN_H / 5 * 2 -50, 0, "Score : %d", score);
    al_draw_textf(font_pirulen_32, al_map_rgb(255, 255, 255), SCREEN_W / 2 - 140, SCREEN_H / 5 * 2 + 50 - 50, 0, "Time : %.1f", time_used);
    al_draw_textf(font_pirulen_32, al_map_rgb(255, 255, 255), SCREEN_W / 2 - 140, SCREEN_H / 5 * 2 + 100 -50, 0, "Bullets : %d", total_bullets);

    al_draw_bitmap(img_money, SCREEN_W / 2 - 70, SCREEN_H / 5 , 0);
    al_draw_text(font_pirulen_18, al_map_rgb(255, 255, 255), SCREEN_W / 2 - 70+60, SCREEN_H / 5 +13, 0, "x");
    al_draw_textf(font_pirulen_32, al_map_rgb(255, 0, 255), SCREEN_W / 2 - 70 + 85, SCREEN_H / 5 +5, 0, "%d", earn_money);

    al_draw_text(font_pirulen_24, al_map_rgb(0, 100, 100), 20, SCREEN_H - 50, 0, "Press enter key to return");
}
static void on_key_down(int keycode) {
    // [HACKATHON 3-10]
    // TODO: If active_scene is SCENE_SETTINGS and Backspace is pressed,
    // return to SCENE_MENU.
    if (keycode == ALLEGRO_KEY_ENTER)
        game_change_scene(scene_menu_create());
}
void sort() {
    record_score[10] = score;
    record_time[10] = time_used;
    for (int i = 0; i < 10; i++) {
        for (int j = i + 1; j <= 10; j++) {
            if (record_score[j] > record_score[i]) {
                int temp = record_score[i];
                record_score[i] = record_score[j];
                record_score[j] = temp;
                float temp2 = record_time[i];
                record_time[i] = record_time[j];
                record_time[j] = temp2;
            }
            else if (record_score[j] == record_score[i] && record_time[j] < record_time[i]) {
                int temp = record_score[i];
                record_score[i] = record_score[j];
                record_score[j] = temp;
                float temp2 = record_time[i];
                record_time[i] = record_time[j];
                record_time[j] = temp2;
            }
        }
    }
}
Scene scene_game_over_create(void) {
    Scene scene;
    MONEY = fopen("money.txt", "w");
    fprintf(MONEY, "%d", money);
    fclose(MONEY);
    RECORD = fopen("record.txt", "r");
    for (int i = 0; i < 10; i++) {
        fscanf(RECORD, "%d %f", &record_score[i], &record_time[i]);
    }
    fclose(RECORD);
    RECORD = fopen("record.txt", "w");
    sort();
    for (int i = 0; i < 10; i++) {
        fprintf(RECORD, "%d %f\n", record_score[i], record_time[i]);
    }
    fclose(RECORD);
    memset(&scene, 0, sizeof(Scene));
    scene.name = "Game over";
    scene.initialize = &init;
    scene.draw = &draw;
    scene.on_key_down = &on_key_down;
    scene.on_mouse_down = &on_mouse_down;
    // TODO: Register more event callback functions such as keyboard, mouse, ...
    game_log("Game over scene created");
    return scene;
}