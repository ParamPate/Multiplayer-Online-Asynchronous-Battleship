#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <signal.h>
#include <poll.h>
#include <errno.h>

#include "network.h"
#include "game.h"

void send_msg(int fd, const char *msg) {
    if(fd != -1) write(fd, msg, strlen(msg));
}

void broadcast_msg(const char *msg) {
    for(int i = 0; i < MAX_PLAYERS; i++) {
        if(players[i].fd != -1) send_msg(players[i].fd, msg);
    }
}

void disconnect_player(int index) {
    if(players[index].registered) {
        char msg[MSG_OUT];
        snprintf(msg, sizeof(msg), "GG %s\n", players[index].name);
        broadcast_msg(msg);
    }
    player_remove(index);
}

void reg_command(int index, const char *msg) {
    Player *p = &players[index];
    char name[NAME_SIZE + 1];
    int x; 
    int y; 
    char dir; 

    if(sscanf(msg, "REG %20s %d %d %c", name, &x, &y, &dir) != 4) {
        send_msg(p->fd, "Invalid REG command format.\n");
        return;
    }
    if(!valid_name(name)) {
        send_msg(p->fd, "INVALID\n");
        return;
    }
    for(int i = 0; i < MAX_PLAYERS; i++) {
        if(i != index && players[i].registered && strcmp(players[i].name, name) == 0) {
            send_msg(p->fd, "TAKEN\n");
            return;
        }
    }
    int ox[SHIPS_LEN];
    int oy[SHIPS_LEN];
    if(ship_cells(x, y, ox, oy, dir) != 1) {
        send_msg(p->fd, "INVALID\n");
        return;
    }
    p->registered = 1;
    strncpy(p->name, name, NAME_SIZE);
    memcpy(p->sx, ox, sizeof(ox));
    memcpy(p->sy, oy, sizeof(oy));
    memset(p->hit, 0, sizeof(p->hit));

    char join_msg[MSG_OUT];
    snprintf(join_msg, sizeof(join_msg), "JOIN %s\n", p->name);
    broadcast_msg(join_msg);
}

void bomb(int index, const char *msg) {
    Player *p = &players[index];
    if(p->registered == 0) {
        return;
    }
    int x; 
    int y;
    int hit = 0;
    if(sscanf(msg, "BOMB %d %d", &x, &y) != 2) {
        send_msg(p->fd, "INVALID\n");
        return;
    }
    for(int i = 0; i < MAX_PLAYERS; i++) {
        if(i != index && players[i].registered) {
            if(ship_check_hit(i, x, y)) {
                char hit_msg[MSG_OUT];
                 hit = 1;
                snprintf(hit_msg, sizeof(hit_msg), "HIT %s %d %d %s\n", p->name, x, y, players[i].name);
                broadcast_msg(hit_msg);
                if(ship_sunk(i)) {
                    disconnect_player(i);
                }
            }
        }
    }
    if(hit == 0) {
        char miss_msg[MSG_OUT];
        snprintf(miss_msg, sizeof(miss_msg), "MISS %s %d %d\n", p->name, x, y);
        broadcast_msg(miss_msg);
    }
}

void process_msg(int index, char *msg) {
    if(strncmp(msg, "REG ", 4) == 0) {
        reg_command(index, msg);
    } else if(strncmp(msg, "BOMB ", 5) == 0) {
        bomb(index, msg);
    } else {
        send_msg(players[index].fd, "INVALID\n");
    }
}


int main(){

}
