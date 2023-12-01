def genDataFor7thOOP(n, out):
    a = open(out, 'w')
    a.write(str(n) + "\n")
    for i in range(int(n / 3)):
        a.write(str(i) + " " + str(i - 1) + "\n")
    for i in range(int(n / 3), int(n / 3) * 2):
        a.write(str(i) + " " + str(-i) + "\n")
    for i in range(int(n / 3) * 2, n):
        a.write(str(i) + " " + str(n - i) + "\n")
    a.close()

genDataFor7thOOP(100000, "./data/gendata.txt")