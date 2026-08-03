#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "game.h"

Player players[MAX_PLAYERS];

void init_players(){ //set to empty state
    for(int i = 0; i < MAX_PLAYERS; i++){
        players[i].fd = -1;
        players[i].registered = 0;
        players[i].inlen = 0;
    }
}

int player_add(int fd){
    for(int i = 0; i < MAX_PLAYERS; i++){
        if(players[i].fd == -1){
            players[i].fd = fd;
            players[i].registered = 0;
            players[i].inlen = 0;
            memset(players[i].name, 0, sizeof(players[i].name));
            return i;
        }
    }
    return -1;
}

int player_remove(int index){
    if(index < 0 || index >= MAX_PLAYERS) return -1;
    
    players[index].fd = -1;
    players[index].registered = 0;
    players[index].inlen = 0;
    close(players[index].fd);
    return 0;       
}

int valid_name(char *name){
    if(strlen(name) == 0 || strlen(name) > NAME_SIZE) return 0; 
    for(size_t i = 0; i < strlen(name); i++){
        if(!isalnum(name[i]) && name[i] != '-') return 0; 
    }
    return 1;
}

int ship_cells(int cx, int cy, int outx[SHIPS_LEN], int outy[SHIPS_LEN], char dir){
    if(dir != '-' && dir != '|') return -1;
    for(int i = 0; i < SHIPS_LEN; i++){
        if(dir == '-'){
            outx[i] = cx + i - 2;
            outy[i] = cy;
        } else {
            outx[i] = cx;
            outy[i] = cy + i -2;
        }
        if(outx[i] < 0 || outx[i] >= BOARD_SIZE || outy[i] < 0 || outy[i] >= BOARD_SIZE) return 0;
    }
    return 1;
}

int ship_check_hit(int index, int x, int y){
    Player *p = &players[index];
    for(int i = 0; i < SHIPS_LEN; i++){
        if(p->sx[i] == x && p->sy[i] == y){
            p->hit[i] = 1;
            return 1;
        }
    }
    return 0;
}

int ship_sunk(int index){
    Player *p = &players[index];
    for(int i = 0; i < SHIPS_LEN; i++){
        if(p->hit[i] == 0) return 0;
    }
    return 1;
}


