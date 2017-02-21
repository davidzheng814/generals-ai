from flask import Blueprint, render_template
from flask_socketio import emit
import os
import json

main = Blueprint('main', __name__,
    template_folder='static/')

@main.route('/')
def index():
    return render_template('index.html')

def process_piece(piece_str):
    piece = {}
    for token in piece_str.split(','):
        key, val = token.split('=')
        if key in ['owner', 'size']:
            val = int(val)
        piece[key] = val
    return piece

def process_replay(lines):
    settings = {}
    actions = settings['actions'] = []

    for line in lines:
        tokens = line.strip().split(' ')
        if line.startswith('setting num_players'):
            settings['num_players'] = int(tokens[-1])
        elif line.startswith('setting start_player'):
            settings['start_player'] = int(tokens[-1])
        elif line.startswith('setting board_width'):
            settings['board_width'] = int(tokens[-1])
        elif line.startswith('setting board_height'):
            settings['board_height'] = int(tokens[-1])
        elif line.startswith('setting usernames'):
            settings['usernames'] = tokens[2:]
        elif line.startswith('action new_piece'):
            actions.append({
                'type':'new_piece',
                'new_piece':process_piece(tokens[2]),
                'loc':int(tokens[3])
            })
        elif line.startswith('action next_move'):
            actions.append({
                'type':'next_move',
                'next_player':int(tokens[2])
            })
        elif line.startswith('action move'):
            actions.append({
                'type':'move',
                'new_piece1':process_piece(tokens[2]),
                'new_piece2':process_piece(tokens[3]),
                'loc1':int(tokens[4]),
                'loc2':int(tokens[5]),
            })
        elif line.startswith('action no_move'):
            actions.append({
              'type':'no_move'
            })
        elif line.startswith('action set_piece'):
            actions.append({
                'type':'set_piece',
                'new_piece':int(tokens[2]),
                'loc':int(tokens[3]),
            })
        elif line.startswith('action set_land'):
            actions.append({
                'type':'set_land',
                'color':int(tokens[2]),
                'size':int(tokens[3]),
            })
        elif line.startswith('action set_army'):
            actions.append({
                'type':'set_army',
                'color':int(tokens[2]),
                'size':int(tokens[3]),
            })
    return settings

def game_router(socketio):
    @socketio.on('connect')
    def connect():
        return

    @socketio.on('new_game')
    def new_game(replay_id):
        filename = 'replays/' + replay_id + '.txt'
        if not os.path.isfile(filename):
            return

        settings = process_replay(open(filename).readlines())
        emit("new_game", settings)

    @socketio.on('save_map')
    def save_map(new_map):
        with open("maps/new_map.txt", 'w') as f:
            f.write(new_map)
