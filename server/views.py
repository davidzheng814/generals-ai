from flask import Blueprint, render_template
from flask_socketio import emit
import os
import json

main = Blueprint('main', __name__,
    template_folder='static/')

@main.route('/')
def index():
    return render_template('index.html')

def game_router(socketio):
    # @socketio.on('connect')
    # def connect():
    #     return

    @socketio.on('new_game')
    def newgame(replay_id):
        filename = 'replays/' + replay_id + '.txt'
        if not os.path.isfile(filename):
            return

        settings = json.loads(open(filename).read())
        emit("new_game", settings)

