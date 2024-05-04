//
// Created by johan on 2024-05-04.
//

#include <pd_api.h>
#include "game.h"

#define SCREEN_WIDTH LCD_COLUMNS
#define SCREEN_HEIGHT LCD_ROWS
#define PLAYER_SPEED 5

typedef struct Player {
    int x;
    int y;
    uint8_t w;
    uint8_t h;
} Player;

const char *fontpath = "/System/Fonts/Asheville-Sans-14-Bold.pft";
Player player = {50, 50, 32, 32};
PlaydateAPI *pd;
LCDFont *font = NULL;

#define draw_text(text, x, y) pd->graphics->drawText((text), strlen(text), kASCIIEncoding, x, y)

void player_draw(void) {
    pd->graphics->fillRect(player.x, player.y, player.w, player.h, kColorBlack);
}

void player_update(void) {
    PDButtons current, pushed, released;
    pd->system->getButtonState(&current, &pushed, &released);
    if(current & kButtonLeft) {
        player.x -= PLAYER_SPEED;
    } if(current & kButtonRight) {
        player.x += PLAYER_SPEED;
    } if(current & kButtonUp) {
        player.y -= PLAYER_SPEED;
    } if(current & kButtonDown) {
        player.y += PLAYER_SPEED;
    }

    // Check collision with walls
    if(player.x < 0) {
        player.x = 0;
    } else if((player.x + player.w) > SCREEN_WIDTH) {
        player.x = (int16_t)(SCREEN_WIDTH - (player.w));
    }

    if(player.y < 0) {
        player.y = 0;
    } else if((player.y + player.h) > SCREEN_HEIGHT) {
        player.y = (int16_t)(SCREEN_HEIGHT - (player.h));
    }
}

void game_init(PlaydateAPI *playdate_api) {
    pd = playdate_api;
    const char *err;
    font = pd->graphics->loadFont(fontpath, &err);
    if (font == NULL) {
        pd->system->error("%s:%i Couldn't load font %s: %s", __FILE__, __LINE__, fontpath, err);
    }
}

int game_update(__attribute__((unused)) void *user_data) {
    int x = 20;
    int y = 20;

    pd->graphics->clear(kColorWhite);
    pd->graphics->setFont(font);
    draw_text("Hello Moving Rect!", x, y);
    pd->system->drawFPS(0, 0);
    player_draw();
    player_update();

    return 1;

}