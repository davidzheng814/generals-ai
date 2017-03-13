import os
import json
import numpy as np
import subprocess

REPLAY_FOLDER = '/Users/dzd123/Downloads/replays_prod'
OUTPUT_FOLDER = ''

files = os.listdir(REPLAY_FOLDER)

X = []
y = []

mapping = {
    -18: 0,
    1: 1,
    18: 2,
    -1: 3,
}

file_ind = 0
for file_ in files:
    file_ = os.path.join(REPLAY_FOLDER, file_)
    with open(file_) as f:
        a = json.loads(f.read())
        if (len(a['usernames']) == 2 
            and a['stars']
            and a["mapHeight"] == 18 and a["mapWidth"] == 18 
            and max(a["stars"]) >= 55
            and len(a["afks"]) == 0):

            process = subprocess.Popen(['node',
                '/Users/dzd123/Documents/2016-2017/generals.io-Replay-Utils/simulator.js', 
                file_], stdout=subprocess.PIPE)
            out, err = process.communicate()

            move_ind = 0
            for line in out.split('\n'):
                if ';' not in line:
                    continue
                turn, state = line.split(';')
                turn = int(turn)
                state = np.array(json.loads(state))
                while turn + 1 == a["moves"][move_ind]["turn"]:
                    move = a["moves"][move_ind]
                    X.append(state)
                    y.append(move["start"] * 4 + mapping[move["end"] - move["start"]])
                    move_ind += 1

                    if move_ind >= len(a["moves"]):
                        break
                if move_ind >= len(a["moves"]):
                    break

            if len(X) > 1e5:
                np.savez_compressed(os.path.join(
                    OUTPUT_FOLDER, 'data'+str(file_ind)),
                    board=np.array(X), move=np.array(y))
                X = []
                y = []
                file_ind += 1
