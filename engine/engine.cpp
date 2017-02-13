#include "./engine.h"

Game::Game() {
}

Game::~Game() {
}

// Generates a new unit on generals and cities at the end of every turn
// Only works if the current move number is divisible by the (moves per turn)
void Game::generate() {
  if (this->move % MPT != 0) return;

  for (int i = 0; i < BOARD_SIZE; ++i) {
    if (SPAWNS(this->board[i].type)) {
      this->board[i].size++;
    }
  }
}

// Gives the player vision around a certain position
void Game::create_vision(int player, int position) {
  for (int dx = -1; dx <= 1; ++dx) {
    for (int dy = -1; dy <= 1; ++dy) {
      int new_pos = position + dx * BOARD_WIDTH + dy;
      if (0 <= new_pos && new_pos < BOARD_SIZE) {
        this->vision[player][new_pos] = true;
      }
    }
  }
}

// Initializes all relevant fields at the start of a new game
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
    this->create_vision(i, generals[i]);
  }

  this->move = 0;

  for (int i = 0; i < NUM_PLAYERS; ++i) {
    this->is_alive[i] = true;
    this->num_land[i] = 1;
    ORDER[i] = i;
    ORDER[MPT - i - 1] = i;
  }
}

// Finds the next player who needs to make a move
void Game::next_move() {
  while (true) {
    this->move++;
    this->generate();
    if (is_alive[player(this->move)]) {
      break;
    }
  }
}

// Makes the given move
void make_move(int player, int position, direction dir, bool half_move) {
}

// Determines whether the game is over
bool Game::is_gameover() {
  for (int i = 0; i < NUM_PLAYERS; ++i) {
    if (is_alive[i]) return false;
  }
  return true;
}

