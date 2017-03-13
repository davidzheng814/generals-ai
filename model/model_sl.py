import tensorflow as tf
from tensorflow.python.framework import ops
import numpy as np
import argparse
import logging
import os
import sys
import json

logging.basicConfig(format='%(asctime)s %(message)s', level=logging.INFO)

NUM_CHANNELS = 4
NUM_PLAYERS = 2
BOARD_WIDTH = 15
BOARD_HEIGHT = 15
BOARD_SIZE = 10

BATCH_SIZE = 32 # batch size
TAU = 10000 # num of steps between target network updates
EPS_MAX = 1 # starting eps-greedy policy
EPS_MIN = .01 # ending eps-greedy policy
EPS_DECAY = .00001 # eps decay per iteration.
GAMMA = 0.99 # Discount factor

# TODO Double Q Learning
# TODO Dueling Network
# TODO restrict random actions to valid actions

class Model(object):
    def __init__(self):
        # TODO Add placeholders
        self.board_batch = tf.placeholder(tf.float32,
            [BATCH_SIZE, NUM_CHANNELS, BOARD_HEIGHT, BOARD_WIDTH])
        self.board = tf.placeholder(tf.float32,
            [1, NUM_CHANNELS, BOARD_HEIGHT, BOARD_WIDTH])
        self.build_model()

    def build_model(self):
        # TODO define model
        # input, prob a 4D tensor (batch, channels, height, width)
        # output, Q-values for each actions
        # Actions will be interpreted first by start loc then by end loc
        pass

    def make_move(self, state):
        # TODO format first as numpy tensors
        qvalues = self.sess.run([self.values], feed_dict={
            self.board: np.array(state['board'])
        })

        return qvalues

    def store_experience(self, state, next_state, move, reward):
        self.exps.append((state, next_state, move, reward))

    def train_step(self):
        pass
 
def main():
    global NUM_PLAYERS, BOARD_WIDTH, BOARD_HEIGHT, BOARD_SIZE

    tokens = raw_input().split(' ')
    NUM_PLAYERS = int(tokens[0])
    BOARD_WIDTH = int(tokens[1])
    BOARD_HEIGHT = int(tokens[2])
    BOARD_SIZE = BOARD_WIDTH * BOARD_HEIGHT

    model = Model()
    line = raw_input()
    state = json.loads(line)

    while True:
        move = model.make_move(state, valid_moves)
        if move['type'] == 'no_move':
            print move['type']
        else:
            print move['type'], move['loc1'], move['loc2']

        reward = float(raw_input())
        line = raw_input()
        if line == "quit":
            break
        next_state = json.loads(line)

        move = model.store_experience(state, next_state, move, reward)
        model.train_step()

if __name__ == '__main__':
    main()
