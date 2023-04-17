#include "scene_start.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include "game.h"
#include "utility.h"
#include <math.h>
#include "shared.h"
#include "scene_menu.h"
#include "scene_settings.h"
#include "scene_game_over.h"
// Variables and functions with 'static' prefix at the top level of a
// source file is only accessible in that file ("file scope", also
// known as "internal linkage"). If other files has the same variable
// name, they'll be different variables.

/* Define your static vars / function prototypes below. */

// TODO: More variables and functions that will only be accessed
// inside this scene. They should all have the 'static' prefix.

static ALLEGRO_BITMAP* img_background;
static ALLEGRO_BITMAP* img_plane;
static ALLEGRO_BITMAP* img_enemy;
// [HACKATHON 2-1]
// TODO: Declare a variable to store your bullet's image.
// Uncomment and fill in the code below.
static ALLEGRO_BITMAP* img_bullet;
static ALLEGRO_BITMAP* img_ufo;
static ALLEGRO_BITMAP* img_ufo_bullet;
static ALLEGRO_BITMAP* img_heart;
static ALLEGRO_BITMAP* img_boss;
static ALLEGRO_BITMAP* img_bulletused;
static ALLEGRO_BITMAP* img_money;
static ALLEGRO_BITMAP* img_money_game;
static ALLEGRO_BITMAP* img_rocket;

typedef struct {
    // The center coordinate of the image.
    float x, y;
    // The width and height of the object.
    float w, h;
    // The velocity in x, y axes.
    float vx, vy;
    // Should we draw this object on the screen.
    bool hidden;
    // The pointer to the object’s image.
    ALLEGRO_BITMAP* img;
    int hp, index;
} MovableObject;

static void init(void);
static void update(void);
static void draw_movable_object(MovableObject obj);
static void draw(void);
static void destroy(void);

#define MAX_ENEMY 3
#define MAX_UFO 4
#define MAX_UFO_BULLET 3
#define MAX_BOSS_HP 50
// [HACKATHON 2-2]
// TODO: Declare the max bullet count that will show on screen.
// You can try max 4 bullets here and see if you needed more.
// Uncomment and fill in the code below.
#define MAX_BULLET 20
#define UFO_HP 3
#define MAX_ULT 2000
#define MAX_ROCKET 10

static MovableObject plane;
static MovableObject heart;
static MovableObject enemies[MAX_ENEMY];
static MovableObject ufos[MAX_UFO];
// [HACKATHON 2-3]
// TODO: Declare an array to store bullets with size of max bullet count.
// Uncomment and fill in the code below.
static MovableObject bullets[MAX_BULLET];
static MovableObject enemies_bullets[MAX_UFO*MAX_UFO_BULLET];
static MovableObject boss;
static MovableObject MO_money;
static MovableObject rocket[MAX_ROCKET];

// [HACKATHON 2-4]
// TODO: Set up bullet shooting cool-down variables.
// 1) Declare your shooting cool-down time as constant. (0.2f will be nice)
// 2) Declare your last shoot timestamp.
// Uncomment and fill in the code below.
static const float MAX_COOLDOWN = 0.2f;
static const float MAX_HEART = 0.8f * 20;
static const float MAX_COOLDOWN_ENEMY = 0.8f * 5;
static const float MAX_MONEY_COOLDOWN = 0.5f * 10;
static const float MAX_ROCKET_COOL = 0.005f;
static double last_heart_timestamp;
static double last_shoot_timestamp;
static double last_enemyshoot_timestamp;
static double last_money_timestamp;
static double last_rocket_timestamp;
static ALLEGRO_SAMPLE* bgm;
static ALLEGRO_SAMPLE_ID bgm_id;
static ALLEGRO_SAMPLE* rocketgo;
static ALLEGRO_SAMPLE_ID rocketgo_id;
static bool draw_gizmos;
static int bullet_used;
int score, character, earn_money;
float time_used, start_time;
int PLANE_HP, ultnow, total_bullets, money;
static ALLEGRO_SAMPLE* shoot;
static ALLEGRO_SAMPLE_ID shoot_id;
static ALLEGRO_SAMPLE* collect;
static ALLEGRO_SAMPLE_ID collect_id;

static int hit(MovableObject a, MovableObject b) {
    if (a.x - a.w / 2 > b.x - b.w / 2 && a.x - a.w / 2 < b.x + b.w / 2 &&
        a.y - a.h / 2 < b.y + b.h / 2 && a.y - a.h / 2 > b.y - b.h / 2) return 1;
    if (a.x + a.w / 2 < b.x + b.w / 2 && a.x + a.w / 2 > b.x - b.w / 2 &&
        a.y - a.h / 2 < b.y + b.h / 2 && a.y - a.h / 2 > b.y - b.h / 2) return 1;
    if (a.x - a.w / 2 > b.x - b.w / 2 && a.x - a.w / 2 < b.x + b.w / 2 &&
        a.y + a.h / 2 < b.y + b.h / 2 && a.y + a.h / 2 > b.y - b.h / 2) return 1;
    if (a.x + a.w / 2 < b.x + b.w / 2 && a.x + a.w / 2 > b.x - b.w / 2 &&
        a.y + a.h / 2 < b.y + b.h / 2 && a.y + a.h / 2 > b.y - b.h / 2) return 1;
    MovableObject temp = a;
    a = b;
    b = temp;
    if (a.x - a.w / 2 > b.x - b.w / 2 && a.x - a.w / 2 < b.x + b.w / 2 &&
        a.y - a.h / 2 < b.y + b.h / 2 && a.y - a.h / 2 > b.y - b.h / 2) return 1;
    if (a.x + a.w / 2 < b.x + b.w / 2 && a.x + a.w / 2 > b.x - b.w / 2 &&
        a.y - a.h / 2 < b.y + b.h / 2 && a.y - a.h / 2 > b.y - b.h / 2) return 1;
    if (a.x - a.w / 2 > b.x - b.w / 2 && a.x - a.w / 2 < b.x + b.w / 2 &&
        a.y + a.h / 2 < b.y + b.h / 2 && a.y + a.h / 2 > b.y - b.h / 2) return 1;
    if (a.x + a.w / 2 < b.x + b.w / 2 && a.x + a.w / 2 > b.x - b.w / 2 &&
        a.y + a.h / 2 < b.y + b.h / 2 && a.y + a.h / 2 > b.y - b.h / 2) return 1;
    return 0;
}
static void draw_healbar(MovableObject a, int max, int thick) {
    al_draw_rectangle(a.x - a.w / 2, a.y + a.h / 2 + 4, a.x + a.w / 2 - (max - a.hp) * (a.w / max), a.y + a.h / 2 + 4 + thick, al_map_rgb(255, 0, 0), thick);
}
static void draw_ultbar(void) {
    if(ultnow < MAX_ULT) al_draw_rectangle(10, SCREEN_H - 20, (ultnow)*(SCREEN_W-10)/MAX_ULT, SCREEN_H-10, al_map_rgb(50, 120, 255), 10);
    else al_draw_rectangle(10, SCREEN_H - 20, (ultnow) * (SCREEN_W - 10) / MAX_ULT, SCREEN_H - 10, al_map_rgb(0, 255, 0), 10);
}
static void init(void) {
    int i, j;
    img_background = load_bitmap_resized("12.jpg", SCREEN_W, SCREEN_H);
    img_bulletused = load_bitmap_resized("bulletused.png", 40, 40);
    img_money = load_bitmap_resized("money.png", 50, 50);
    img_money_game = load_bitmap_resized("money.png", 30, 30);
    shoot = load_audio("shoot.mp3");
    collect = load_audio("collect.mp3");
    rocketgo = load_audio("rocket.mp3");
    MO_money.img = img_money_game;
    MO_money.w = 30;
    MO_money.h = 30;
    MO_money.x = MO_money.w / 2 + (float)rand() / RAND_MAX * (SCREEN_W - MO_money.w) + 0.5;
    MO_money.y = -MO_money.h / 2;
    MO_money.vx = 0;
    MO_money.vy = 3;
    MO_money.hidden = true;

    if (character == 0) img_plane = plane.img = load_bitmap_resized("plane.png", 55, 55);
    else if (character == 1) img_plane = plane.img = load_bitmap_resized("plane2.jfif", 55, 55);
    plane.x = SCREEN_W / 2 - 55 / 2;
    plane.y = 500;
    plane.w = 55;
    plane.h = 55;
    plane.hp = PLANE_HP;
    img_enemy = load_bitmap("smallfighter0006.png");

    for (i = 0; i < MAX_ENEMY; i++) {
        enemies[i].img = img_enemy;
        enemies[i].w = al_get_bitmap_width(img_enemy);
        enemies[i].h = al_get_bitmap_height(img_enemy);
        enemies[i].x = enemies[i].w / 2 + (float)rand() / RAND_MAX * (SCREEN_W - enemies[i].w) + 0.5;
        enemies[i].y = -enemies[i].h/2;
        enemies[i].vx = 0;
        enemies[i].vy = (int)rand() % 4 + 2;
        enemies[i].hidden = true;
    }
    // [HACKATHON 2-5]
    // TODO: Initialize bullets.
    // 1) Search for a bullet image online and put it in your project.
    //    You can use the image we provided.
    // 2) Load it in by 'load_bitmap' or 'load_bitmap_resized'.
    // 3) For each bullets in array, set their w and h to the size of
    //    the image, and set their img to bullet image, hidden to true,
    //    (vx, vy) to (0, -3).
    // Uncomment and fill in the code below.
    if(character == 0) img_bullet = load_bitmap("image12.png");
    else if(character == 1) img_bullet = load_bitmap("image13.png");
    for (int i = 0; i < MAX_BULLET; i++) {
        bullets[i].img = img_bullet;
        bullets[i].w = al_get_bitmap_width(img_bullet);
        bullets[i].h = al_get_bitmap_height(img_bullet);
        bullets[i].vx = 0;
        bullets[i].vy = -3;
        bullets[i].hidden = true;
    }

    img_ufo = load_bitmap("ufo-1.png");
    for (i = 0; i < MAX_UFO; i++) {
        ufos[i].img = img_ufo;
        ufos[i].w = al_get_bitmap_width(img_ufo);
        ufos[i].h = al_get_bitmap_height(img_ufo);
        ufos[i].x = ufos[i].w / 2 + (float)rand() / RAND_MAX * (SCREEN_W - ufos[i].w);
        ufos[i].y = ufos[i].h / 2;
        ufos[i].vx = (int)rand() % 2 + 0.5;
        if (i % 2) ufos[i].vx *= -1;
        ufos[i].vy = (int)rand() % 2 + 0.5;
        ufos[i].hidden = true;
        ufos[i].hp = 3;
    }

    img_ufo_bullet = load_bitmap_resized("enemybullet2.png", 20, 20);
    for (i = 0; i < MAX_UFO; i++) {
        for (j = 0; j < MAX_UFO_BULLET; j++) {
            enemies_bullets[i * 3 + j].img = img_ufo_bullet;
            enemies_bullets[i * 3 + j].x = ufos[i].x;
            enemies_bullets[i * 3 + j].y = ufos[i].y + ufos[i].h / 2 - enemies_bullets[i * 3 + j].h/2;
            enemies_bullets[i * 3 + j].w = 20;
            enemies_bullets[i * 3 + j].h = 20;
            enemies_bullets[i * 3 + j].vx = (int)rand() % 3;
            if ((i * 3 + j) % 2) enemies_bullets[i * 3 + j].vx *= -1;
            enemies_bullets[i * 3 + j].vy = (int)rand() % 5 + 2;
            if (ufos[i].vy < 0) enemies_bullets[i * 3 + j].vy += -ufos[i].vy;
            else enemies_bullets[i * 3 + j].vy += ufos[i].vy;
            enemies_bullets[i * 3 + j].hidden = true;
            enemies_bullets[i * 3 + j].index = i;
        }
    }

    img_heart = load_bitmap_resized("heart2.png", 30, 30);
    heart.img = img_heart;
    heart.w = 30;
    heart.h = 30;
    heart.x = heart.w / 2 + (float)rand() / RAND_MAX * (SCREEN_W - heart.w) + 0.5;
    heart.y = 0;
    heart.vx = (int)rand() % 4;
    heart.vy = 3;
    heart.hidden = true;

    img_boss = load_bitmap_resized("tank-1.png", 150, 150);
    boss.img = img_boss;
    boss.w = 150;
    boss.h = 150;
    boss.x = SCREEN_W/2-boss.w/2;
    boss.y = 150;
    boss.vx = 5;
    boss.vy = 0;
    boss.hp = MAX_BOSS_HP;
    boss.hidden = true;

    img_rocket = load_bitmap_resized("rocket-1.png", 70, 70);
    for (i = 0; i < MAX_ROCKET; i++) {
        rocket[i].img = img_rocket;
        rocket[i].w = 70;
        rocket[i].h = 70;
        rocket[i].x = boss.x;
        rocket[i].y = boss.y + boss.h / 2 - rocket[i].h/2;
        rocket[i].vx = 0;
        rocket[i].vy = 4;
        rocket[i].hp = 3;
        rocket[i].hidden = true;
    }
    // Can be moved to shared_init to decrease loading time.
    bgm = load_audio("mythica.ogg");
    game_log("Start scene initialized");
    bgm_id = play_bgm(bgm, 1);
}


static void update(void) {
    if(ultnow < MAX_ULT) ultnow++;
    if (plane.hp <= 0)
        game_change_scene(scene_game_over_create());
    if ((int)time_used % 20 == 0 && time_used >= 20 && boss.hidden) {
        boss.hidden = false;
        boss.vx = 5;
    }
    plane.vx = plane.vy = 0;
    if (key_state[ALLEGRO_KEY_UP] || key_state[ALLEGRO_KEY_W])
        plane.vy -= 1;
    if (key_state[ALLEGRO_KEY_DOWN] || key_state[ALLEGRO_KEY_S])
        plane.vy += 1;
    if (key_state[ALLEGRO_KEY_LEFT] || key_state[ALLEGRO_KEY_A])
        plane.vx -= 1;
    if (key_state[ALLEGRO_KEY_RIGHT] || key_state[ALLEGRO_KEY_D])
        plane.vx += 1;
    // 0.71 is (1/sqrt(2)).
    plane.y += plane.vy * 4 * (plane.vx ? 0.71f : 1);
    plane.x += plane.vx * 4 * (plane.vy ? 0.71f : 1);
    // [HACKATHON 1-1]
    // TODO: Limit the plane's collision box inside the frame.
    //       (x, y axes can be separated.)
    // Uncomment and fill in the code below.
    if (plane.x - plane.w / 2 < 0)
        plane.x = plane.w / 2;
    else if (plane.x + plane.w / 2 > SCREEN_W)
        plane.x = SCREEN_W - plane.w / 2;
    if (plane.y - plane.h / 2 < 0)
        plane.y = plane.h / 2;
    else if (plane.y + plane.h / 2 > SCREEN_H)
        plane.y = SCREEN_H - plane.h / 2;
    // [HACKATHON 2-6]
    // TODO: Update bullet coordinates.
    // 1) For each bullets, if it's not hidden, update x, y
    // according to vx, vy.
    // 2) If the bullet is out of the screen, hide it.
    // Uncomment and fill in the code below.
    if (!MO_money.hidden) {
        if (hit(plane, MO_money)) {
            money++;
            earn_money++;
            collect_id = play_audio(collect, 3);
            MO_money.hidden = true;
        }
        else {
            MO_money.x += MO_money.vx;
            MO_money.y += MO_money.vy;
            if (MO_money.y - MO_money.h / 2 >= SCREEN_H)
                MO_money.hidden = true;
        }
    }
    if (!heart.hidden) {
        if (hit(plane, heart)) {
            heart.hidden = true;
            if (plane.hp + 3 > PLANE_HP) plane.hp = PLANE_HP;
            else plane.hp += 3;
        }
        else {
            heart.x += heart.vx;
            heart.y += heart.vy;
            if (heart.x + heart.w / 2 <= 0 || heart.x - heart.w / 2 >= SCREEN_W) heart.vx *= -1;
            if (heart.y + heart.h / 2 <= 0 || heart.y - heart.h / 2 >= SCREEN_H)
                heart.hidden = true;
        }
    }
    int i, j, stop;
    for (i = 0; i < MAX_BULLET; i++) {
        if (bullets[i].hidden)
            continue;
        stop = 0;  
        for (j = 0; j < MAX_ENEMY; j++) {
            if (hit(bullets[i], enemies[j]) && !enemies[j].hidden) {
                bullets[i].hidden = true;
                bullet_used--;
                enemies[j].hidden = true;
                score++;
                stop = 1;
                if (ultnow < MAX_ULT) ultnow++;
                break;
            }
        }
        if (stop) continue;
        for (j = 0; j < MAX_UFO; j++) {
            if (hit(bullets[i], ufos[j]) && !ufos[j].hidden) {
                bullets[i].hidden = true;
                ufos[j].hp--;
                bullet_used--;
                stop = 1;
                if (ufos[j].hp <= 0) {
                    if (ultnow < MAX_ULT) ultnow++;
                    ufos[j].hidden = true;
                    score += 5;
                }
                break;
            }
        }
        if (stop) continue;
        for (j = 0; j < MAX_ROCKET; j++) {
            if (hit(bullets[i], rocket[j]) && !rocket[j].hidden) {
                rocket[j].hp--;
                bullets[i].hidden = true;
                bullet_used--;
                stop = 1;
                if (rocket[j].hp <= 0) {
                    rocket[j].hidden = true;
                }
                break;
            }
        }
        if (stop) continue;
        if (hit(bullets[i], boss) && !boss.hidden) {
            bullet_used--;
            bullets[i].hidden = true;
            boss.hp--;
            stop = 1;
            if (boss.hp <= 0) {
                boss.hidden = true;
                if (ultnow < MAX_ULT) ultnow += 100;
                score += 1000;
            }
        }
        if (stop) continue;
        bullets[i].x += bullets[i].vx;
        bullets[i].y += bullets[i].vy;
        if (bullets[i].y + bullets[i].h / 2 <= 0 || bullets[i].y - bullets[i].h / 2 >= SCREEN_H ||
            bullets[i].x + bullets[i].w / 2 <= 0 || bullets[i].x - bullets[i].w / 2 >= SCREEN_W) {
            bullets[i].hidden = true;
            bullet_used--;
        }
            
    }
    for (i = 0; i < MAX_ENEMY; i++) {
        if (enemies[i].hidden)
            continue;
        if (hit(plane, enemies[i])) {
            plane.hp--;
            enemies[i].hidden = true;
            continue;
        }
        enemies[i].y += enemies[i].vy;
        if (enemies[i].y-enemies[i].h/2 > SCREEN_H || enemies[i].y+enemies[i].h/2 < 0) enemies[i].hidden = true;
    }
    for (i = 0; i < MAX_UFO; i++) {
        if (ufos[i].hidden)
            continue;
        if (hit(plane, ufos[i])) {
            plane.hp -= 2;
            ufos[i].hidden = true;
            continue;
        }
        ufos[i].x += ufos[i].vx;
        ufos[i].y += ufos[i].vy;
        if (ufos[i].x - ufos[i].w / 2 <= 0 || ufos[i].x + ufos[i].w / 2 >= SCREEN_W) ufos[i].vx *= -1;
        if (ufos[i].y + ufos[i].h / 2 >= SCREEN_H / 3 || ufos[i].y - ufos[i].h / 2 <= 0) ufos[i].vy *= -1;
    }
    for (i = 0; i < MAX_UFO * MAX_UFO_BULLET; i++) {
        if (enemies_bullets[i].hidden)
            continue;
        stop = 0;
        if (hit(enemies_bullets[i], plane)) {
            plane.hp--;
            enemies_bullets[i].hidden = true;
            continue;
        }
        for (j = 0; j < MAX_BULLET; j++) {
            if (hit(enemies_bullets[i], bullets[j]) && !bullets[j].hidden) {
                enemies_bullets[i].hidden = true;
                bullets[j].hidden = true;
                bullet_used--;
                stop = 1;
                break;
            }
        }
        if (stop) continue;
        enemies_bullets[i].x += enemies_bullets[i].vx;
        enemies_bullets[i].y += enemies_bullets[i].vy;
        if (enemies_bullets[i].x + enemies_bullets[i].w / 2 <= 0 || enemies_bullets[i].x - enemies_bullets[i].w / 2 >= SCREEN_W ||
            enemies_bullets[i].y-enemies_bullets[i].h/2 >= SCREEN_H)
            enemies_bullets[i].hidden = true;
    }
    if (!boss.hidden) {
        boss.x += boss.vx;
        if (boss.x - boss.w / 2 <= 0 || boss.x + boss.w / 2 >= SCREEN_W) boss.vx *= -1;
    }
    else {
        SCREEN_W / 2 - boss.w / 2;
        boss.hp = MAX_BOSS_HP;
    }
    for (i = 0; i < MAX_ROCKET; i++) {
        if (rocket[i].hidden)
            continue;
        if (hit(plane, rocket[i]) && !rocket[i].hidden) {
            plane.hp -= 3;
            rocket[i].hidden = true;     
            continue;
        }
        rocket[i].x += rocket[i].vx;
        rocket[i].y += rocket[i].vy;
        if (rocket[i].y - rocket[i].h / 2 >= SCREEN_H) rocket[i].hidden = true;
    }
    // [HACKATHON 2-7]
    // TODO: Shoot if key is down and cool-down is over.
    // 1) Get the time now using 'al_get_time'.
    // 2) If Space key is down in 'key_state' and the time
    //    between now and last shoot is not less that cool
    //    down time.
    // 3) Loop through the bullet array and find one that is hidden.
    //    (This part can be optimized.)
    // 4) The bullet will be found if your array is large enough.
    // 5) Set the last shoot time to now.
    // 6) Set hidden to false (recycle the bullet) and set its x, y to the
    //    front part of your plane.
    // Uncomment and fill in the code below.
    double now2 = al_get_time();
    if (now2 - last_heart_timestamp >= MAX_HEART) {
        if (heart.hidden) {
            last_heart_timestamp = now2;
            heart.hidden = false;
            heart.x = heart.w / 2 + (float)rand() / RAND_MAX * (SCREEN_W - heart.w) + 0.5;
            heart.y = 0;
            heart.vx = (int)rand() % 4;
            heart.vy = 3;
        }
    }
    double now = al_get_time();
    if (key_state[ALLEGRO_KEY_SPACE] && now - last_shoot_timestamp >= MAX_COOLDOWN) {
        for (i = 0; i < MAX_BULLET; i++) {
            if (bullets[i].hidden) {
                bullet_used++;
                total_bullets++;
                shoot_id = play_audio(shoot, 1);
                last_shoot_timestamp = now;
                bullets[i].hidden = false;
                bullets[i].x = plane.x;
                bullets[i].y = plane.y - plane.w / 2 + bullets[i].h/2;
                break;
            }
        }
    }
    for (i = 0; i < MAX_ENEMY; i++) {
        if (enemies[i].hidden) {
            enemies[i].hidden = false;
            enemies[i].x = enemies[i].w / 2 + (float)rand() / RAND_MAX * (SCREEN_W - enemies[i].w);
            enemies[i].y = -enemies[i].h/2;
            enemies[i].vy = (int)rand() % 4 + 2;
        }
    }
    for (i = 0; i < MAX_UFO; i++) {
        if (ufos[i].hidden) {
            ufos[i].hidden = false;
            ufos[i].x = ufos[i].w / 2 + (float)rand() / RAND_MAX * (SCREEN_W - ufos[i].w);
            ufos[i].y = ufos[i].h/2;
            ufos[i].vx = (int)rand() % 2 + 0.5;
            if (i % 2) ufos[i].vx *= -1;
            ufos[i].vy = (int)rand() % 2 + 1;
            ufos[i].hp = 3;
        }
    }
    double now3 = al_get_time();
    if(now3 - last_enemyshoot_timestamp >= MAX_COOLDOWN_ENEMY){
        last_enemyshoot_timestamp = now3;
        for (i = 0; i < MAX_UFO*MAX_UFO_BULLET; i++) {
            if (enemies_bullets[i].hidden) {
                enemies_bullets[i].hidden = false;
                enemies_bullets[i].x = ufos[enemies_bullets[i].index].x;
                enemies_bullets[i].y = ufos[enemies_bullets[i].index].y + ufos[enemies_bullets[i].index].h / 2 - enemies_bullets[i].h/2;
                enemies_bullets[i].vx = (int)rand() % 4;
                enemies_bullets[i].vy = (int)rand() % 3 + ufos[enemies_bullets[i].index].vy;
                if (enemies_bullets[i].vy == 0) enemies_bullets[i].vy = 2;
                if (enemies_bullets[i].vy < 0) enemies_bullets[i].vy *= -1;
            }
        }
    }
    double now4 = al_get_time();
    if (now4 - last_money_timestamp >= MAX_MONEY_COOLDOWN) {
        if (MO_money.hidden) {
            last_money_timestamp = now4;
            MO_money.hidden = false;
            MO_money.x = MO_money.w / 2 + (float)rand() / RAND_MAX * (SCREEN_W - MO_money.w) + 0.5;
            MO_money.y = -MO_money.h / 2;
            MO_money.vx = 0;
            MO_money.vy = 3;
        }
    }

    double now5 = al_get_time();    
    for (i = 0; i < MAX_ROCKET; i++) {
        if (now5 - last_rocket_timestamp >= MAX_ROCKET_COOL && !boss.hidden) {
            last_rocket_timestamp = now5;
            if (rocket[i].hidden) { 
                rocketgo_id = play_audio(rocketgo, 2);
                rocket[i].hidden = false;
                rocket[i].x = boss.x;
                rocket[i].y = boss.y + boss.h / 2 - rocket[i].h/2;
                rocket[i].vy = 4;
                rocket[i].hp = 3;
            }
        }
    }
}
static void draw_movable_object(MovableObject obj) {
    if (obj.hidden)
        return;
    al_draw_bitmap(obj.img, round(obj.x - obj.w / 2), round(obj.y - obj.h / 2), 0);
    if (draw_gizmos) {
        al_draw_rectangle(round(obj.x - obj.w / 2), round(obj.y - obj.h / 2),
            round(obj.x + obj.w / 2) + 1, round(obj.y + obj.h / 2) + 1, al_map_rgb(255, 0, 0), 0);
    }
}
static void draw(void) {
    int i;
    al_draw_bitmap(img_background, 0, 0, 0);
    // [HACKATHON 2-8]
    // TODO: Draw all bullets in your bullet array.
    // Uncomment and fill in the code below.
    for (i = 0; i < MAX_BULLET; i++)
        draw_movable_object(bullets[i]);
    draw_movable_object(plane);
    for (i = 0; i < MAX_ENEMY; i++)
        draw_movable_object(enemies[i]);
    for (i = 0; i < MAX_UFO * MAX_UFO_BULLET; i++)
        draw_movable_object(enemies_bullets[i]);
    for (i = 0; i < MAX_UFO; i++) {
        draw_movable_object(ufos[i]);
        draw_healbar(ufos[i], UFO_HP, 2);
    }
    draw_movable_object(heart);
    for (i = 0; i < MAX_ROCKET; i++)
        draw_movable_object(rocket[i]);
    draw_movable_object(boss);
    if(!boss.hidden) draw_healbar(boss, MAX_BOSS_HP, 3);
    al_draw_textf(font_pirulen_18, al_map_rgb(20, 255, 20), 20, 90, 0, "Score : %d", score);
    al_draw_textf(font_pirulen_18, al_map_rgb(20, 255, 20), 20, 120, 0, "Time : %.1f", time_used = al_get_time() - start_time);
    al_draw_textf(font_pirulen_18, al_map_rgb(255, 0, 0), 20, SCREEN_H - 60, 0, "HEALTH : %d", plane.hp);
    al_draw_textf(font_pirulen_24, al_map_rgb(0, 50, 200), SCREEN_W - 150, SCREEN_H - 70, 0, "%d/%d", MAX_BULLET - bullet_used, MAX_BULLET);
    al_draw_bitmap(img_bulletused, SCREEN_W - 50, SCREEN_H - 80, 0);
    draw_movable_object(MO_money);
    draw_healbar(plane, PLANE_HP, 2);
    draw_ultbar();
    al_draw_bitmap(img_money, 20, 22, 0);
    al_draw_text(font_pirulen_18, al_map_rgb(255, 255, 255), 80, 35, 0, "x");
    al_draw_textf(font_pirulen_24, al_map_rgb(255, 0, 255), 105, 30, 0, "%d", earn_money);
}

static void destroy(void) {
    al_destroy_bitmap(img_background);
    al_destroy_bitmap(img_plane);
    al_destroy_bitmap(img_enemy);
    al_destroy_sample(bgm);
    al_destroy_sample(shoot);
    al_destroy_sample(collect);
    al_destroy_sample(rocketgo);
    // [HACKATHON 2-9]
    // TODO: Destroy your bullet image.
    // Uncomment and fill in the code below.
    al_destroy_bitmap(img_bullet);
    al_destroy_bitmap(img_ufo);
    al_destroy_bitmap(img_ufo_bullet);
    al_destroy_bitmap(img_heart);
    al_destroy_bitmap(img_boss);
    al_destroy_bitmap(img_bulletused);
    al_destroy_bitmap(img_money);
    al_destroy_bitmap(img_rocket);
    stop_bgm(bgm_id);
    game_log("Start scene destroyed");
}

static void on_key_down(int keycode) {
    if (keycode == ALLEGRO_KEY_TAB)
        draw_gizmos = !draw_gizmos;
    if (keycode == ALLEGRO_KEY_BACKSPACE)
        game_change_scene(scene_menu_create());
    if (keycode == ALLEGRO_KEY_ENTER)
        game_change_scene(scene_game_over_create());
    if (keycode == ALLEGRO_KEY_F && ultnow == MAX_ULT) {
        ultnow = 0;
        for (int i = 0; i < MAX_UFO * MAX_UFO_BULLET; i++)
            enemies_bullets[i].hidden = true;
        for (int i = 0; i < MAX_ENEMY; i++)
            enemies[i].hidden = true;
        for (int i = 0; i < MAX_UFO; i++) {
            ufos[i].vx /= 2;
            ufos[i].vy /= 2;
        }
        for (int i = 0; i < MAX_ROCKET; i++)
            rocket[i].hidden = true;
        if (!boss.hidden) {
            boss.vx /= 2;
            boss.hp -= 10;
        }
    }
}

// TODO: Add more event callback functions such as keyboard, mouse, ...

// Functions without 'static', 'extern' prefixes is just a normal
// function, they can be accessed by other files using 'extern'.
// Define your normal function prototypes below.

// The only function that is shared across files.
Scene scene_start_create(void) {
    Scene scene;
    if (PLANE_HP == 0) PLANE_HP = 20;
    score = 0;
    start_time = al_get_time();
    ultnow = 0;
    bullet_used = 0;
    total_bullets = 0;
    earn_money = 0;
    memset(&scene, 0, sizeof(Scene));
    scene.name = "Start";
    scene.initialize = &init;
    scene.update = &update;
    scene.draw = &draw;
    scene.destroy = &destroy;
    scene.on_key_down = &on_key_down;
    // TODO: Register more event callback functions such as keyboard, mouse, ...
    game_log("Start scene created");
    return scene;
}