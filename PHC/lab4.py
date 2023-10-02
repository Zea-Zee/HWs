import argparse
import numpy as np


def shuffle_data():
    argparser = argparse.ArgumentParser()
    argparser.add_argument("input_1")
    argparser.add_argument("input_2")
    argparser.add_argument("P")
    args = argparser.parse_args()
    input_1 = str(args.input_1)
    input_2 = str(args.input_2)
    P = str(args.P)
    real_data_istream = open(args.input_1, 'r')
    synthetic_data_istream = open(args.input_2, 'r')
    real_data = (str(real_data_istream.readline())).split(' ')
    synthetic_data = (str(synthetic_data_istream.readline())).split(' ')
    print("Args:", real_data, synthetic_data, "P:", P)
    selecter = np.random.choice(5, len(real_data))
    return np.where(selecter < 1 / float(P) - 1, real_data, synthetic_data)

print("Res:", shuffle_data())