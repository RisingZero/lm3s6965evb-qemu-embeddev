//
// Created by Andrea Raineri on 24/12/23.
//

#ifndef CAEOS_PROJECT_PONG_H
#define CAEOS_PROJECT_PONG_H

#define pongFIELD_WIDTH 128
#define pongFIELD_HEIGHT 64
#define pongUSER_X 123
#define pongCOMPUTER_X 5

typedef struct pong_s *Pong;
typedef struct player_s Player;
typedef struct ball_s Ball;
typedef enum {
    UP = -1,
    DOWN = 1
} PlayerDirection;

Pong pong_init();
void pong_start_game_loop(Pong game);
Player* get_user_player(Pong game);
void move_player(Player *p, PlayerDirection dir);
Ball* get_ball_info(Pong game);

#endif //CAEOS_PROJECT_PONG_H
