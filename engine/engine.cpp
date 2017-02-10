#include "./engine.h"

Game::Game() {
  this->board = new piece[BOARD_SIZE];

  for (int i = 0; i < NUM_PLAYERS; ++i) {
    this->vision[i] = new bool[BOARD_SIZE];
  }
}

Game::~Game() {
  delete [] this->board;

  for (int i = 0; i < NUM_PLAYERS; ++i) {
    delete [] this->vision[i];
  }
}

void Game::generate() {
  if (this->move % MPT != 0) return;

  for (int i = 0; i < BOARD_SIZE; ++i) {
    if (SPAWNS(this->board[i].type)) {
      this->board[i].size++;
    }
  }
}

void Game::start(vector<int> &mountains,
    vector<int> &cities, vector<int> &generals) {
  for (size_t i = 0; i < BOARD_SIZE; ++i) {
    this->board[i] = {EMPTY, -1, 0};
  }

  for (size_t i = 0; i < mountains.size(); ++i) {
    this->board[mountains[i]] = {MOUNTAIN, -1, 0};
  }

  for (size_t i = 0; i < cities.size(); ++i) {
    this->board[cities[i]] = {CITY, -1, 40};
  }

  for (size_t i = 0; i < generals.size(); ++i) {
    this->board[generals[i]] = {GENERAL, (int8_t)i, 0};
  }

  this->move = 0;

  for (int i = 0; i < NUM_PLAYERS; ++i) {
    is_alive[i] = true;
    ORDER[i] = i;
    ORDER[MPT - i - 1] = i;
  }
}

void Game::next_move() {
  while (true) {
    this->move = (this->move + 1) % NUM_PLAYERS;
    if (is_alive[player(this->move)]) {
      break;
    }
  }
}

void Game::make_move() {
}

bool Game::is_gameover() {
  for (int i = 0; i < NUM_PLAYERS; ++i) {
    if (is_alive[i]) return false;
  }
  return true;
}
