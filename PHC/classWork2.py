import time
import math
import re
# func = lambda x, y: x * y
# def mult(x, y):
#     return x * y
# #
# #
# now = time.time()
# for i in range(0, 10**8):
#     # list1 = list()
#     (lambda x, y: x * y)(15, 17)
# print(time.time() - now)
# now = time.time()
# for i in range(0, 10**8):
#     # list2 = []
#     def mult(x, y):
#         return x * y
#     mult(15, 17)
# print(time.time() - now)
# now = time.time()
# for i in range(0, 10**8):
#     # list1 = list()
#     func(15, 17)
# print(time.time() - now)
# now = time.time()
# for i in range(0, 10**8):
#     # list2 = []
#     mult(15, 17)
# print(time.time() - now)

# Создание бесконечного генератора в функции
# def fun_iter(val):
#     s = 1
#     while True:
#         s = s * val
#         yield s
#
# fit = fun_iter(2)
# for i in range(40):
#     print(next(fit))


def check_is_palindrome(str):
    for i in range(0, math.ceil(len(str) / 2)):
        if str[i] != str[-i - 1]:
            return False
    return True

def find_longest_word(str):
    longest_word = ""
    for word in str.split(' '):
        if len(word) > len(longest_word):
            longest_word = word
    return longest_word

def even_num(list):
    """Get int list and return number of even and odd numbers"""
    odd = 0
    for num in list:
        if num & 1:
            odd += 1
    return len(list) - odd, odd

def replace_to_synonyms(str, dct):
    res = list()
    for word in str.split(' '):
        if len(str) < 0:
            continue
        if dct.get(word):
            res.append(dct[word])
        elif dct.get(word[:-2]) and word[-2:] == "?!":
            res.append(dct[word[:-2]] + word[-2:])
        elif dct.get(word[1:-1]) and ((word[0] == "'" and word[-1] == '"') or (word[0] == "'" and word[-1] == "'") or (word[0] == "`"
                and word[-1] == '`') or (word[0] == "(" and word[-1] == ')') or (word[0] == "<" and word[-1] == '>') or (word[0] == "{" and word[-1] == '}')):
            res.append(word[0] + dct[word[1:-1]] + word[-1])
            # print(word, "''")
        elif dct.get(word[:-1]) and (word[-1] == ',' or word[-1] == '.' or word[-1] == '/' or word[-1] == '?' or word[-1] == ':' or word[-1] == ';'
                or word[-1] == '{' or word[-1] == '}' or word[-1] == '[' or word[-1] == ']' or word[-1] == '=' or word[-1] == '+'
                or word[-1] == '-' or word[-1] == '(' or word[-1] == ')' or word[-1] == '*' or word[-1] == '&' or word[-1] == '$'
                or word[-1] == '%' or word[-1] == '#' or word[-1] == '№' or word[-1] == '@' or word[-1] == '!'):
            res.append(dct[word[:-1]] + word[-1])
            # print(word, "?")
        else:
            res.append(word)
        # print(word, "orig")
    return ' '.join(res)

def fib(n):
    if n <= 1:
        return n
    if n == 2:
        return 1
    def calc_fib(a, b, cur_n):
        if cur_n == n:
            return b
        return calc_fib(b, b + a, cur_n + 1)
    return calc_fib(1, 1, 2)
M = {0: 0, 1: 1}

def fib_with_var(n):
    if n in M:
        return M[n]
    M[n] = fib(n - 1) + fib(n - 2)
    return M[n]

def fib_list(n):
    fibs = [0, 1, 1]
    if n < 3:
        return fibs[n]
    for i in range(3, n + 1):
        fibs.append(fibs[-1] + fibs[-2])
    return  fibs[-1]

def examine_file_data(name):
    file = open(name, 'r')
    strings = 0
    words = 0
    symbols = 0
    for line in file.readlines():
        strings += 1
        for word in line.split(' '):
            words +=1
            for symb in word:
                if symb == ' ' or symb == '\n':
                    continue
                symbols += 1
    return strings, words, symbols

def generate_inf_progression(a, r):
    while True:
        yield a
        a *= r

progression = generate_inf_progression(2, 2)

print("---ex1---", "repaper", "reparer", "a", "aba", "ab", check_is_palindrome("repaper"), check_is_palindrome("reparer"), check_is_palindrome("a"), check_is_palindrome("aba"), check_is_palindrome("ab"))
print("---ex2---", find_longest_word("""Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."""))
print("---ex3---", "range(0, 1001)", even_num(list(range(0, 1001))))
print("---ex3---", "range(0, 1001)", even_num(list(range(0, 1001))))
print("---ex4---", replace_to_synonyms("The cat is sleeping on the couch.", {"cat": "feline", "sleeping": "dozing", "couch": "sofa"}))
print(replace_to_synonyms("Are you kidding?", {"kidding": "joking"}))
print(replace_to_synonyms("Are you kidding?!", {"kidding": "joking"}))
print(replace_to_synonyms("You are 'genious'", {"genious": "dumbass"}))
print(replace_to_synonyms("Are you crazy?!", {"crazy": "insane"}))
print(replace_to_synonyms("I'm ok (no)", {"no": "false"}))
print("---ex5---", fib(0), fib(1), fib(2), fib(3), fib(5), fib(10), fib(100))
print("---ex5---", fib_list(20577))
print("---ex7---")
for i in range(16):
    print(next(progression))