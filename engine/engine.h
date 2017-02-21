#ifndef ENGINE_H
#define ENGINE_H

#include <stdint.h>
#include <vector>
#include <stdlib.h>
#include <time.h>

using namespace std;

extern int NUM_PLAYERS;
extern int START_PLAYER;
extern int BOARD_WIDTH;
extern int BOARD_HEIGHT;
#define BOARD_SIZE BOARD_WIDTH*BOARD_HEIGHT
#define MPT NUM_PLAYERS*2  // Moves per Turn
extern int ORDER[]; // size chosen arbitrarily. 

#define player(a) ORDER[(a) % MPT]  // Gives the index of the player who needs to move this turn
#define SPAWNS(a) (a) == GENERAL || (a) == CITY  // Whether a type spawns a unit
#define ROW_OF(a) (a) / BOARD_WIDTH
#define COL_OF(a) (a) % BOARD_WIDTH
#define IND_OF(a, b) (a) * BOARD_WIDTH + (b)
#define NO_MOVE {-1, -1, false}

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

// Let (-1, -1, false) be no move.
struct move_t {
  int loc1;
  int loc2;
  bool half_move;
};


class Player;
class Game {
  public:
    int winner;
    bool verbose;
    bool use_input;

    int move;
    piece *board;
    bool **vision;
    bool *is_alive;

    int *num_land;
    int *num_army;
    vector<move_t> moves;

    Game(bool use_input, bool verbose);
    ~Game();
    void start(Player **players);
    void init(vector<int> &mountains, vector<int> &cities, vector<int> &generals);
    bool make_move(move_t m);
    bool is_gameover();

  private:
    void generate();
    void next_move();
    void set_all_moves();
    void create_vision(int player, int position);
    bool is_valid_move(move_t m);
};

#endif
