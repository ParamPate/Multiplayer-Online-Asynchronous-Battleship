#ifndef GAME_H
#define GAME_H

#define MAX_PLAYERS 64
#define BOARD_SIZE 10
#define SHIPS_LEN 5
#define MSG_BUF_SIZE 256
#define MSG_OUT 256
#define NAME_SIZE 32

typedef struct player {
    int fd; 
    int registered; 
    int name [NAME_SIZE];
    int sx[SHIPS_LEN], sy[SHIPS_LEN],;
    int hit[SHIPS_LEN];
    char inbuf[MSG_BUF_SIZE];
    int inlen; 
}Player; 

extern Player players[MAX_PLAYERS];
void init_players();
void player_add(int fd);
void player_remove(int index);
int ship_cells(int cx, int cy, int outx[SHIPS_LEN], int outy[SHIPS_LEN], char dir);
int name_valid(char *name);
int ship_check_hit(int index, int x, int y);
int sunk(int index);

#endif
