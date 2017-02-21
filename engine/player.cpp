#include "./players.h"

move_t RandomPlayer::act(Game *g) {
  int num_moves = g->moves.size();
  return g->moves[rand() % num_moves];
}
