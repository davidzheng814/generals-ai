#include "./engine.h"
#include "./players.h"
#include <stdio.h>
#include <iostream>
#include <assert.h>

int NUM_PLAYERS = 2;
int START_PLAYER = 0;
int BOARD_WIDTH = 15;
int BOARD_HEIGHT = 15;
int ORDER[32];

string Types[] =
{
    "EMPTY",
    "GENERAL",
    "ARMY",
    "CITY",
    "MOUNTAIN"
};

const vector<string> split(const string& s, const char& c)
{
	string buff{""};
	vector<string> v;
	
	for(auto n:s)
	{
		if(n != c) buff+=n; else
		if(n == c && buff != "") { v.push_back(buff); buff = ""; }
	}
	if(buff != "") v.push_back(buff);
	
	return v;
}

Game::Game(bool use_input, bool verbose) {
  this->verbose = verbose;
  this->use_input = use_input;
  this->board = new piece[BOARD_SIZE];
  this->vision = new bool*[NUM_PLAYERS];
  for (int i = 0; i < NUM_PLAYERS; ++i) {
    this->vision[i] = new bool[BOARD_SIZE];
  }

  this->is_alive = new bool[NUM_PLAYERS];
  this->num_land = new int[NUM_PLAYERS];
  this->num_army = new int[NUM_PLAYERS];

  if (this->verbose) {
    cout << "setting num_players " << NUM_PLAYERS << "\n";
    cout << "setting start_player " << START_PLAYER << "\n";
    cout << "setting board_width " << BOARD_WIDTH << "\n";
    cout << "setting board_height " << BOARD_HEIGHT << "\n";
    cout << "setting usernames " << "dzd123 edwardpark97" << "\n";
  }
}

Game::~Game() {
  delete [] this->board;
  for (int i = 0; i < NUM_PLAYERS; ++i) {
    delete [] this->vision[i];
  }
  delete [] this->vision;
  delete [] this->is_alive;
  delete [] this->num_land;
  delete [] this->num_army;
}

// Generates a new unit on generals and cities at the end of every turn
// Only works if the current move number is divisible by the (moves per turn)
void Game::generate() {
  if (this->move % MPT != 0) return;

  bool army_generation = ((this->move / MPT) % 25 == 0);

  for (int i = 0; i < BOARD_SIZE; ++i) {
    piece &p = this->board[i];
    if (p.type == GENERAL || 
      (p.type == CITY && (p.owner >= 0 || p.size < 40)) || 
      (p.type == ARMY && army_generation)) {
      // Is a general or a city or an army on the 25th turn
      p.size++;
      this->num_army[p.owner]++;
      cout << "action set_piece type=" << Types[p.type] << ",owner="
        << (int)p.owner << ",size=" << p.size << " " << i << "\n";
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
  if (player(this->move) != this->board[m.loc1].owner
      || this->board[m.loc1].size <= 1) {
    return false;
  }

  if (m.loc1 < 0 || BOARD_SIZE <= m.loc1 || m.loc2 < 0 || BOARD_SIZE <= m.loc2) {
    return false;
  }

  if (abs(COL_OF(m.loc1) - COL_OF(m.loc2)) != 1
      && abs(m.loc1 - m.loc2) != BOARD_WIDTH) {
    return false;
  }

  if (this->board[m.loc1].type == MOUNTAIN || this->board[m.loc2].type == MOUNTAIN) {
    return false;
  }

  return true;
}

// Generates all possible moves and stores into this->moves
void Game::set_all_moves() {
  this->moves.clear();
  for (int i = 0; i < BOARD_SIZE; ++i) {
    if (player(this->move) == this->board[i].owner
        && this->board[i].size > 1) {
      int row = ROW_OF(i), col = COL_OF(i);
      if (row != 0) {
        int loc2 = IND_OF(row-1, col);
        if (this->board[loc2].type != MOUNTAIN) {
          this->moves.push_back({i, loc2, false});
          this->moves.push_back({i, loc2, true});
        }
      }
      if (row != BOARD_HEIGHT - 1) {
        int loc2 = IND_OF(row+1, col);
        if (this->board[loc2].type != MOUNTAIN) {
          this->moves.push_back({i, loc2, false});
          this->moves.push_back({i, loc2, true});
        }
      }
      if (col != 0) {
        int loc2 = IND_OF(row, col-1);
        if (this->board[loc2].type != MOUNTAIN) {
          this->moves.push_back({i, loc2, false});
          this->moves.push_back({i, loc2, true});
        }
      }
      if (col != BOARD_WIDTH - 1) {
        int loc2 = IND_OF(row, col+1);
        if (this->board[loc2].type != MOUNTAIN) {
          this->moves.push_back({i, loc2, false});
          this->moves.push_back({i, loc2, true});
        }
      }
    }
  }

  this->moves.push_back(NO_MOVE);
}

// Initializes all relevant fields at the start of a new game
void Game::init(vector<int> &mountains,
    vector<int> &cities, vector<int> &generals) {
  for (int i = 0; i < BOARD_SIZE; ++i) {
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
    this->board[generals[i]] = {GENERAL, (int8_t)i, 1};
    this->create_vision(i, generals[i]);
    if (this->verbose) {
      cout << "action new_piece type=GENERAL,owner=" << i << ",size=1 " << generals[i] << "\n";
    }
  }

  this->move = 0;

  for (int i = 0; i < NUM_PLAYERS; ++i) {
    this->is_alive[i] = true;
    this->num_land[i] = 1;
    this->num_army[i] = 1;
    ORDER[i] = (i + START_PLAYER) % NUM_PLAYERS;
    ORDER[MPT - i - 1] = (i + START_PLAYER) % NUM_PLAYERS;
  }

  this->set_all_moves();
}

// Inits the game and then runs the game by calling players.
void Game::start(Player **players) {
  if (this->verbose) {
    cout << "action next_move " << player(this->move) << "\n";
  }

  while (true) {
    int p = player(this->move);
    if (this->use_input && p == 0) {
      string line;
      getline(cin, line);
      const vector<string> tokens = split(line, ' ');
      if (tokens[1] == "move") {
        this->make_move({stoi(tokens[2]), stoi(tokens[3]), false});
      } else if (tokens[1] == "half_move") {
        this->make_move({stoi(tokens[2]), stoi(tokens[3]), true});
      } else if (tokens[1] == "no_move") {
        this->make_move(NO_MOVE);
      } else {
        cout << "Invalid Move!\n";
      }
    } else {
      this->make_move(players[p]->act(this));
    }

    if (this->is_gameover()) {
      cout << "winner " << this->winner << "\n";
      break;
    }
  }
}

// Finds the next player who needs to make a move
void Game::next_move() {
  while (true) {
    this->move++;
    if (this->is_alive[player(this->move)]) {
      if (this->verbose) {
        int p = player(this->move);
        cout << "action set_land " << p << " " << this->num_land[p] << "\n";
        cout << "action set_army " << p << " " << this->num_army[p] << "\n";
        cout << "action next_move " << p << "\n";
      }
      if (this->move % MPT == 0) {
        this->generate();
        if (this->verbose) {
          int p = player(this->move);
          cout << "action set_land " << p << " " << this->num_land[p] << "\n";
          cout << "action set_army " << p << " " << this->num_army[p] << "\n";
          cout << "action next_move " << p << "\n";
        }
      }
      break;
    }
  }
}

// Makes the given move
// Returns true if valid move;
bool Game::make_move(move_t m) {
  if (m.loc1 == -1) { // no move
    if (this->verbose) cout << "action no_move\n";
    this->next_move();
    this->set_all_moves();

    return true;
  }

  if (!this->is_valid_move(m)) {
    cout << "Invalid move\n";
    assert("Invalid move");
    return false;
  }

  piece &start = this->board[m.loc1];
  piece &end = this->board[m.loc2];

  int num_to_move = m.half_move ? start.size / 2 : start.size - 1;
  start.size -= num_to_move;

  if (start.owner == end.owner) {
    // Reinforcing your own tile
    end.size += num_to_move;
  } else {
    // Attacking enemy tile
    int end_size = end.size - num_to_move;
    if (end_size < 0) {
      // Successfully captured
      if (end.type == GENERAL) {
        for (int i = 0; i < BOARD_SIZE; ++i) {
          if (this->board[i].owner == end.owner) {
            this->board[i].owner = start.owner;
          }
        }
        end.type = CITY;
        this->is_alive[end.owner] = false;
      }

      if (end.owner >= 0) {
        this->num_army[end.owner] -= end.size;
        this->num_land[end.owner]--;
      }
      this->num_army[start.owner] -= end.size;
      this->num_land[start.owner]++;

      end.size = -1 * end_size;
      end.owner = start.owner;
      if (end.type == EMPTY) {
        end.type = ARMY;
      }
      this->create_vision(start.owner, m.loc2);
    } else {
      // Unsuccesfully captured
      this->num_army[end.owner] -= num_to_move;
      this->num_army[start.owner] -= num_to_move;

      end.size = end_size;
    }
  }

  if (this->verbose) {
    cout << "action move type=" << Types[start.type]
         << ",owner=" << (int)start.owner << ",size=" << start.size 
         << " type=" << Types[end.type] << ",owner=" << (int)end.owner
         << ",size=" << end.size << " " << m.loc1 << " "
         << m.loc2 << "\n";
  }

  this->next_move();
  this->set_all_moves();

  return true;
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

int main(int argc, const char* argv[]) {
  srand(time(NULL));

  bool is_verbose = false;
  bool use_input = false;
  Player *players[2] = {
    STR_TO_PLAYER.at("random").player0,
    STR_TO_PLAYER.at("random").player1
  };

  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "-v") == 0) {
      is_verbose = true;
    }
    if (strcmp(argv[i], "-i") == 0) {
      use_input = true;
    }
    if (strcmp(argv[i], "-p0") == 0) {
      players[0] = STR_TO_PLAYER.at(argv[i+1]).player0;
    }
    if (strcmp(argv[i], "-p1") == 0) {
      players[1] = STR_TO_PLAYER.at(argv[i+1]).player1;
    }
  }

  vector<int> mountains, cities, generals;
  string line;
  while (true) {
    getline(cin, line);
    if (line == "start") break;
    const vector<string> tokens = split(line, ' ');
    if (tokens.size() < 2) break;

    if (tokens[1] == "board_width") {
      BOARD_WIDTH = stoi(tokens[2]);
    } else if (tokens[1] == "board_height") {
      BOARD_HEIGHT = stoi(tokens[2]);
    } else if (tokens[1] == "start_player") {
      START_PLAYER = stoi(tokens[2]);
    } else if (tokens[1] == "usernames") {
      // do nothing for now.
    } else if (tokens[1] == "generals") {
      for (size_t i = 2; i < tokens.size(); ++i) {
        generals.push_back(stoi(tokens[i]));
      }
    } else if (tokens[1] == "mountains") {
      for (size_t i = 2; i < tokens.size(); ++i) {
        mountains.push_back(stoi(tokens[i]));
      }
    } else if (tokens[1] == "cities") {
      for (size_t i = 2; i < tokens.size(); ++i) {
        cities.push_back(stoi(tokens[i]));
      }
    }
  }

  Game game(use_input, is_verbose);
  game.init(mountains, cities, generals);
  game.start(players);
}
