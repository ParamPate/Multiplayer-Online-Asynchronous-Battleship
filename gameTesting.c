#include <stdio.h>
#include <assert.h>
#include "game.h" 

int main() {
    printf("Testing...\n");

    assert(valid_name("albert") == 1);
    assert(valid_name("player-123") == 1);
    assert(valid_name("invalid_name!") == 0); // Contains '_' and '!'
    assert(valid_name("") == 0);

    int ox[5], oy[5];
    assert(ship_cells(3, 4, ox, oy, '-') == 1); 
    assert(ox[0] == 1 && ox[4] == 5); // Should span x=1 to x=5

    assert(ship_cells(0, 1, ox, oy, '-') == 0);

    init_players();
    int p1 = player_add(5); 
    assert(p1 == 0);
    assert(players[p1].fd == 5);

    player_remove(p1);
    assert(players[p1].fd == -1);

    printf("All unit tests passed!\n");
    return 0;
}