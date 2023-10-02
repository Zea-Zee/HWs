import argparse
import numpy as np


def shuffle_data():
    argparser = argparse.ArgumentParser()
    argparser.add_argument("input_1")
    argparser.add_argument("input_2")
    argparser.add_argument("P")
    args = argparser.parse_args()
    P = str(args.P)
    real_data_istream = open(args.input_1, 'r')
    synthetic_data_istream = open(args.input_2, 'r')
    real_data = (str(real_data_istream.readline())).split(' ')
    synthetic_data = (str(synthetic_data_istream.readline())).split(' ')
    print("Args:", real_data, synthetic_data, "P:", P)
    selector = np.random.choice(5, len(real_data))
    return np.where(selector < 1 / float(P) - 1, real_data, synthetic_data) if float(P) > 0 else real_data


print("Res:", shuffle_data())
#   python lab4.py random_select_1.txt random_select_2.txt 0.2
