//
// Created by Andrea Raineri on 24/12/23.
//

#include "pong.h"
#include "stdio.h"
#include "osram128x64x4.h"

#define pongBALL_WIDTH 2
#define pongBALL_HEIGHT 2
#define pongBLOCK_WIDTH 2
#define pongBLOCK_HEIGHT 10

struct player_s {
    unsigned long y;
    int score;
    int isUser;
};

struct ball_s {
    unsigned long pos[2];
    unsigned long vec2[2];
};

struct pong_s {
    Player user;
    Player computer;
    Ball ball;
} game_global;

unsigned char BALL[pongBALL_HEIGHT];
unsigned char BLOCK[pongBLOCK_HEIGHT];
unsigned char BLANK_SPACE[pongBLOCK_HEIGHT] = {0};

static void draw_ball(Ball ball);
static void clear_ball(Ball ball);
static void draw_block(Player player);
static void clear_block(Player player);
static void display_scores(Pong game);

Pong pong_init() {
    Pong game = (Pong)&game_global;

    register int i;
    for (i = 0; i < pongBALL_HEIGHT; i++)
        BALL[i] = 0xFF;
    for (i = 0; i < pongBLOCK_HEIGHT; i++)
        BLOCK[i] = 0xFF;

    game->user.isUser = 1;
    game->user.score = 0;
    game->user.y = (pongFIELD_HEIGHT - pongBLOCK_HEIGHT) / 2;
    game->computer.isUser = 0;
    game->computer.score = 0;
    game->computer.y = (pongFIELD_HEIGHT - pongBLOCK_HEIGHT) / 2;
    game->ball.pos[0] = (pongFIELD_WIDTH - pongBALL_WIDTH) / 2;
    game->ball.pos[1] = (pongFIELD_HEIGHT - pongBALL_HEIGHT) / 4;
    game->ball.vec2[0] = 1;
    game->ball.vec2[1] = -1;

    draw_ball(game->ball);
    draw_block(game->user);
    draw_block(game->computer);

    OSRAM128x64x4ImageDraw(BLOCK,0,  pongFIELD_HEIGHT, 10, 2);

    return game;
}

Player* get_user_player(Pong game) {
    return &game->user;
}

void pong_start_game_loop(Pong game) {
    // game loop, no return
    // to be called inside independent rtos task

    while (1) {
        Ball old_ball = game->ball;

        // x
        if (game->ball.pos[0] == pongCOMPUTER_X + pongBLOCK_WIDTH + 1) {
            // check position with computer block, in case it's a hit
            if (game->ball.pos[1] >= game->computer.y && game->ball.pos[1] <= game->computer.y)
                game->ball.vec2[0] = -game->ball.vec2[0];           // bounce on x
        } else if (game->ball.pos[0] == pongUSER_X - 1) {
            // check position with user block, in case it's a hit
            if (game->ball.pos[1] >= game->user.y && game->ball.pos[1] <= game->user.y)
                game->ball.vec2[0] = -game->ball.vec2[0];           // bounce on x
        }

        // y
        if (game->ball.pos[1] == 0 || game->ball.pos[1] == pongFIELD_HEIGHT - 1)
            game->ball.vec2[1] = -game->ball.vec2[1];

        game->ball.pos[0] += game->ball.vec2[0];
        game->ball.pos[1] += game->ball.vec2[1];
        clear_ball(old_ball);
        draw_ball(game->ball);

        // delay
        for (int i = 0; i < 10000000; i++){}
    }
}

void move_player(Player *p, PlayerDirection dir) {
    int x_pos = (p->isUser) ? pongUSER_X : pongCOMPUTER_X;
    Player old = *p;
    switch (dir) {
        case UP:
            if (p->y == 0)
                return;
            p->y--;
            break;
        case DOWN:
            if (p->y + pongBLOCK_HEIGHT == pongFIELD_HEIGHT)
                return;
            p->y++;
            break;
        default:
            return;
    }

    clear_block(old);
    draw_block(*p);
}

Ball* get_ball_info(Pong game) {
    return &game->ball;
}

static void draw_ball(Ball ball) {
    OSRAM128x64x4ImageDraw(BALL, ball.pos[0], ball.pos[1], pongBALL_WIDTH, pongBALL_HEIGHT);
}

static void clear_ball(Ball ball) {
    OSRAM128x64x4ImageDraw(BLANK_SPACE, ball.pos[0], ball.pos[1], pongBALL_WIDTH, pongBLOCK_HEIGHT);
}

static void draw_block(Player player) {
    OSRAM128x64x4ImageDraw(BLOCK, player.isUser ? pongUSER_X : pongCOMPUTER_X, player.y, pongBLOCK_WIDTH, pongBLOCK_HEIGHT);
}

static void clear_block(Player player) {
    OSRAM128x64x4ImageDraw(BLANK_SPACE, player.isUser ? pongUSER_X : pongCOMPUTER_X, player.y, pongBLOCK_WIDTH, pongBLOCK_HEIGHT);
}

static void display_scores(Pong game) {

}