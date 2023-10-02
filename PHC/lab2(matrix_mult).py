import argparse
import time
RED = '\033[91m'
RESET = '\033[0m'

def make_matrices_multiplied():
    """Take two parameters (input file with matrix path, output file path for result of multiplying)"""
    arg_parser = argparse.ArgumentParser()
    arg_parser.add_argument("i_path")
    arg_parser.add_argument("o_path")
    args = arg_parser.parse_args()
    i_path = str(args.i_path)
    o_path = str(args.o_path)
    rfile = open(i_path, "r")
    lines = rfile.readlines()
    rfile.close()

    matrix_a = list()
    matrix_b = list()
    def fill_matrix(m_list, lines):
        """Takes list where will be the matrix and fill it by data from list of input matrix strings.
        When string is \n it returns """
        line = lines.pop(0)
        while line:
            m_list.append(list())
            for el in line.split(' '):
                if el == '\n':
                    return
                m_list[-1].append(int(el))
            print(m_list[-1])
            if(len(m_list) > 1 and len(m_list[-1]) < len(m_list[-2])):
                raise ValueError(RED + "Incorrect number of columns in matrix" + RESET)
            if not lines:
                return
            line = lines.pop(0)
    fill_matrix(matrix_a, lines)
    print("\n\n")
    fill_matrix(matrix_b, lines)

    w_file = open(o_path, "w")
    if (not len(matrix_a)) or (not len(matrix_b)) or len(matrix_a[0]) != len(matrix_b):
        # w_file.write("Incorrect input data")
        raise ValueError('Incorrect size of matrix(ces)')
    matrix_c = list()
    for i in range(0, len(matrix_a) - 1):
        matrix_c.append(list())
        for j in range(0, len(matrix_b[0])):
            matrix_c[i].append(0)
            for k in range(0, len(matrix_b)):
                # print(i, j, k)
                matrix_c[i][j] += matrix_a[i][k] * matrix_b[k][j]
        w_file.write(str(matrix_c[i]) + "\n")


make_matrices_multiplied()
# tst_list = [1] * 100000
# # print(tst_list)
# now = time.time()
# while len(tst_list) > 0:
#     tst_list.pop()
# print(time.time() - now)
# tst_list = [1] * 100000
# # print(tst_list)
# now = time.time()
# while len(tst_list) > 0:
#     tst_list.pop(0)
# print(time.time() - now)