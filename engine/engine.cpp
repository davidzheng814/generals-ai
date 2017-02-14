#include "./engine.h"
#include <stdio.h>
#include <iostream>
#include <assert.h>

Game::Game(bool verbose) {
  this->verbose = verbose;

  if (this->verbose) {
    cout << "setting num_players " << NUM_PLAYERS << "\n";
    cout << "setting start_player " << START_PLAYER << "\n";
    cout << "setting board_width " << BOARD_WIDTH << "\n";
    cout << "setting board_height " << BOARD_HEIGHT << "\n";
    cout << "setting usernames " << "why_do_we_need_usernames" << "\n";
  }
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

bool Game::is_valid_move(move_t m) {
  if (player(this->move) != this->board[m.loc1].owner) {
    return false;
  }

  if (m.loc1 < 0 || BOARD_SIZE <= m.loc1 || m.loc2 < 0 || BOARD_SIZE <= m.loc2) {
    return false;
  }

  if (abs(m.loc1 - m.loc2) != 1 && abs(m.loc1 - m.loc2) != BOARD_WIDTH) {
    return false;
  }

  if (this->board[m.loc1].type == MOUNTAIN || this->board[m.loc2].type == MOUNTAIN) {
    return false;
  }

  return true;
}

string Types[] =
{
    "EMPTY",
    "GENERAL",
    "ARMY",
    "CITY",
    "MOUNTAIN"
};

// Initializes all relevant fields at the start of a new game
void Game::start(vector<int> &mountains,
    vector<int> &cities, vector<int> &generals) {
  for (size_t i = 0; i < BOARD_SIZE; ++i) {
    this->board[i] = {EMPTY, -1, 0};
  }

  for (size_t i = 0; i < mountains.size(); ++i) {
    this->board[mountains[i]] = {MOUNTAIN, -1, 0};
    if (this->verbose) {
      cout << "action new_piece type=MOUNTAIN " << mountains[i] << "\n";
    }
  }

  for (size_t i = 0; i < cities.size(); ++i) {
    this->board[cities[i]] = {CITY, -1, 40};
    if (this->verbose) {
      cout << "action new_piece type=CITY,size=40 " << cities[i] << "\n";
    }
  }

  for (size_t i = 0; i < generals.size(); ++i) {
    this->board[generals[i]] = {GENERAL, (int8_t)i, 0};
    this->create_vision(i, generals[i]);
    if (this->verbose) {
      cout << "action new_piece type=GENERAL,owner=" << i << " " << generals[i] << "\n";
    }
  }

  this->move = 0;

  for (int i = 0; i < NUM_PLAYERS; ++i) {
    this->is_alive[i] = true;
    this->num_land[i] = 1;
    ORDER[i] = (i + START_PLAYER) % NUM_PLAYERS;
    ORDER[MPT - i - 1] = (i + START_PLAYER) % NUM_PLAYERS;
  }
}

// Finds the next player who needs to make a move
void Game::next_move() {
  while (true) {
    this->move++;
    this->generate();
    if (this->is_alive[player(this->move)]) {
      if (this->verbose) {
        cout << "action next_move " << player(this->move) << "\n";
      }
      break;
    }
  }
}

// Makes the given move
void Game::make_move(move_t m) {
  if (!this->is_valid_move(m)) {
    cout << "Invalid move\n";
    assert("Invalid move");
  }

  piece start = this->board[m.loc1];
  piece end = this->board[m.loc2];

  int num_to_move = m.half_move ? (start.size - 1) : (start.size - 1) / 2;
  start.size = start.size - num_to_move;

  if (start.owner == end.owner) {
    // Reinforcing your own tile
    end.size = end.size + num_to_move;
  } else {
    // Attacking enemy tile
    int end_size = end.size - num_to_move;
    if (end_size < 0) {
      // Successfully captured
      if (end.type == GENERAL) {
        for (size_t i = 0; i < BOARD_SIZE; ++i) {
          if (this->board[i].owner == end.owner) {
            this->board[i].owner = start.owner;
          }
        }
        end.type = CITY;
      }

      end.size = -1 * end_size;
      end.owner = start.owner;
    } else {
      end.size = end_size;
    }
  }

  cout << "action move type=" << Types[start.type] << ",owner=" << start.owner << ",size=" << start.size 
    << " type=" << Types[end.type] << ",owner=" << end.owner << ",size=" << end.size << " " << m.loc1 << " "
    << m.loc2 << "\n";
}

// Determines whether the game is over
bool Game::is_gameover() {
  for (int i = 0; i < NUM_PLAYERS; ++i) {
    if (this->is_alive[i]) return false;
  }
  return true;
}

// Splits the input string based on whitespace
vector<string> split(string input) {
  vector<string> array;
  size_t pos = 0, found;
  while((found = input.find_first_of(' ', pos)) != string::npos) {
      array.push_back(input.substr(pos, found - pos));
      pos = found+1;
  }
  array.push_back(input.substr(pos));
  return array;
}

int main(int argc, char* argv[]) {
  // for (string line; getline(cin, line);) {
  //   vector<string> splitted = split(line);

  //   for (size_t i = 0; i < splitted.size(); ++i) {
  //     cout << splitted[i] << endl;
  //     printf("%d\n", "hello" == splitted[i]);
  //   }

  //   cout << line << endl;
  // }
  // return 0;
  Game(true);
}