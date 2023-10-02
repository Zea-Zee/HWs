import random
import math
import os

'''while 1:
    a = math.floor(random.random() * 1000)
    if a == 0:
        print(a)
        break
    if a == 999:
        print(a)
        break'''

print("\nex1")
#1
a = str(math.floor(random.random() * 1000))
sum = 0
for i in range(0, len(a)):
    sum += int(a[i])
print("num is", a, "\nsum of digits is", sum)

print("\nex2")
#2
a = str(random.random())
sum = 0
for i in range(0, len(a)):
    if a[i] != '.':
        sum += int(a[i])
print("num is", a, "\nsum of digits is", sum)

print("\nex3")
#3
a = float(input("enter spehere's R\n"))
print("square of sphere is", 4 * math.pi * (a ** 2))

print("\nex4")
#4
a = int(input("enter a year\n"))
print("this year is ...") if a % 4 == 0 else print("this year isn't ...")

print("\nex5")
#5
a = int(input("enter the N\n"))
arr = list([1] * (a + 1))
arr[0], arr[1] = 0, 0
for i in range(1, a):
    if(arr[i]):
        # print(i, "is simple")
        if(i * i <= a):
            # print(i, "ok")
            for j in range(i * i, a, i):
                # print(j, "Was deleted")
                arr[j] = 0
for i in range(0, a):
    if(arr[i]):
        print(i)

print("\nex6")
# print(round(1.331000000002))
#6
cash = int(input("how many money do you give us?\n"))
years = int(input("how many years do you want to keep your money?\n"))
def make_profit(sum, percent, years_remained):
    if years_remained == 0:
        return sum
    return make_profit(sum + sum * percent, percent, years_remained - 1)
print(round(make_profit(cash, 0.1, years), 15))

print("\nex7")
# print(round(1.331000000002))
#7

def search_files(directory):
    # print("dir is", directory)
    for el in os.listdir(path=directory):
        # print(el, os.path.isdir(directory + '/' + el))
        if os.path.isdir(directory + '/' + el):
            search_files(directory+ '/' + el)
        else:
            print(el)

search_files('./')
