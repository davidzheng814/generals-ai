#ifndef ENGINE_H
#define ENGINE_H

#include <stdint.h>
#include <vector>

using namespace std;

#define NUM_PLAYERS 2
#define BOARD_WIDTH 15
#define BOARD_HEIGHT 15
#define BOARD_SIZE BOARD_WIDTH*BOARD_HEIGHT
#define MPT NUM_PLAYERS*2  // Moves per Turn
int ORDER[MPT];

#define player(a) ORDER[a % MPT]  // Gives the index of the player who needs to move this turn
#define SPAWNS(a) a == GENERAL || a == CITY  // Whether a type spawns a unit

enum type {
  EMPTY = 0,
  GENERAL = 1,
  ARMY = 2,
  CITY = 3,
  MOUNTAIN = 4,
};

enum direction {
  NORTH = 0,
  EAST = 1,
  SOUTH = 2,
  WEST = 3,
};

struct piece {
  type type;
  int8_t owner;
  uint32_t size;
};

class Game {
  public:
    int move;
    piece board[BOARD_SIZE];
    bool vision[NUM_PLAYERS][BOARD_SIZE];
    bool is_alive[NUM_PLAYERS];

    int num_land[NUM_PLAYERS];
    int num_units[NUM_PLAYERS];

    Game();
    ~Game();
    void start(vector<int> &mountains, vector<int> &cities, vector<int> &generals);
    void make_move(int player, int position, direction dir, bool half_move);
    bool is_gameover();

  private:
    void generate();
    void next_move();
    void create_vision(int player, int position);
};

#endif
