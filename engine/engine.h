#ifndef ENGINE_H
#define ENGINE_H

#include <stdint.h>
#include <vector>

using namespace std;

#define NUM_PLAYERS 2
#define BOARD_WIDTH 15
#define BOARD_SIZE BOARD_WIDTH*BOARD_WIDTH
#define MPT NUM_PLAYERS*2
int ORDER[MPT];

#define player(a) ORDER[a % MPT]
#define SPAWNS(a) a == GENERAL || a == ARMY || a == CITY

enum type {
  EMPTY = 0,
  GENERAL = 1,
  ARMY = 2,
  CITY = 3,
  MOUNTAIN = 4,
};

struct piece {
  type type;
  int8_t owner;
  uint32_t size;
};

class Game {
  public:
    int move;
    piece *board;
    bool *vision[NUM_PLAYERS];
    bool is_alive[NUM_PLAYERS];

    Game();
    ~Game();
    void start(vector<int> &mountains, vector<int> &cities, vector<int> &generals);
    void make_move();
    bool is_gameover();

  private:
    void generate();
    void next_move();
};

#endif
