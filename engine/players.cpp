#include "./players.h"

const map<string, player_pair> STR_TO_PLAYER = {
  {"random", {new RandomPlayer(), new RandomPlayer()}}
};
