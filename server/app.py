from views import main, game_router

from flask import Flask
from flask_socketio import SocketIO, emit

import os
import argparse

app = Flask(__name__)
app.register_blueprint(main)
socketio = SocketIO(app)
game_router(socketio)

@app.after_request
def add_header(response):
    return response

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--production', action="store_false")

    args = parser.parse_args()

    socketio.run(app, debug=True)
