#ifndef PLAYER_H
#define PLAYER_H

#include "./engine.h"

#include <string>
#include <map>

class Player {
  public:
    virtual move_t act(Game *g) = 0;
};

class RandomPlayer : public Player {
  public:
    move_t act(Game *g);
};

struct player_pair {
  Player *player0;
  Player *player1;
};

extern const map<string, player_pair> STR_TO_PLAYER;

#endif
