//
// Created by johan on 2024-05-04.
//

#include <pd_api.h>
#include "game.h"

#define SCREEN_WIDTH LCD_COLUMNS
#define SCREEN_HEIGHT LCD_ROWS
#define PLAYER_SPEED 7.56f

#define draw_text(text, x, y) pd->graphics->drawText((text), strlen(text), kASCIIEncoding, x, y)

typedef struct Player {
    float x;
    float y;
    LCDSprite *sprite;
    PDRect bounds;
} Player;

const char *fontpath = "/System/Fonts/Asheville-Sans-14-Bold.pft";
Player player;

PlaydateAPI *pd;
LCDFont *font = NULL;


void player_init(void) {
    player.bounds.x = 0;
    player.bounds.y = 0;
    player.bounds.width = 64;
    player.bounds.height = 64;

    LCDBitmap *bitmap = pd->graphics->loadBitmap("data/img/capmv-8.pdi", NULL);
    const struct playdate_sprite* sprite = pd->sprite;
    player.sprite = sprite->newSprite();
    sprite->setImage(player.sprite, bitmap, 0);
    pd->sprite->addSprite(player.sprite);
    pd->sprite->setVisible(player.sprite, 1);
    pd->sprite->setUpdatesEnabled(player.sprite, 1);
    pd->sprite->setBounds(player.sprite, player.bounds);
    pd->sprite->setCollideRect(player.sprite, player.bounds);
    pd->sprite->setCenter(player.sprite, 0, 0);
}

void player_draw(void) {
    pd->sprite->drawSprites();
}

void player_update(void) {
    PDButtons current;

    pd->system->getButtonState(&current, NULL, NULL);
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
    } else if((player.x + player.bounds.width) > SCREEN_WIDTH) {
        player.x = (int16_t)(SCREEN_WIDTH - (player.bounds.width));
    }

    if(player.y < 0) {
        player.y = 0;
    } else if((player.y + player.bounds.height) > SCREEN_HEIGHT) {
        player.y = (int16_t)(SCREEN_HEIGHT - (player.bounds.height));
    }

    pd->sprite->moveTo(player.sprite, player.x, player.y);
    player.bounds.x = player.x;
    player.bounds.y = player.y;
}

void game_init(PlaydateAPI *playdate_api) {
    pd = playdate_api;
    const char *err;
    font = pd->graphics->loadFont(fontpath, &err);
    if (font == NULL) {
        pd->system->error("%s:%i Couldn't load font %s: %s", __FILE__, __LINE__, fontpath, err);
    }
    player_init();
}

int game_update(__attribute__((unused)) void *user_data) {
    int x = 20;
    int y = 20;

    pd->graphics->clear(kColorWhite);
    pd->graphics->setFont(font);
    draw_text("Hello Moving Rect!", x, y);
    pd->system->drawFPS(0, 0);
    player_update();
    player_draw();

    return 1;

}