#ifndef ENGINE_H
#define ENGINE_H

#include <stdint.h>
#include <vector>

using namespace std;

#define NUM_PLAYERS 2
#define START_PLAYER 0
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

struct piece {
  type type;
  int8_t owner;
  uint32_t size;
};

struct move_t {
  int loc1;
  int loc2;
  bool half_move;
};

class Game {
  public:
    bool verbose;

    int move;
    piece board[BOARD_SIZE];
    bool vision[NUM_PLAYERS][BOARD_SIZE];
    bool is_alive[NUM_PLAYERS];

    int num_land[NUM_PLAYERS];
    int num_units[NUM_PLAYERS];

    Game(bool verbose);
    ~Game();
    void start(vector<int> &mountains, vector<int> &cities, vector<int> &generals);
    void make_move(move_t m);
    bool is_gameover();

  private:
    void generate();
    void next_move();
    void create_vision(int player, int position);
    bool is_valid_move(move_t m);
};

#endif
