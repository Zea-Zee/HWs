RESET = '\033[0m'
RED = '\033[31m'
GREEN = '\033[32m'
YELLOW = '\033[33m'
BOLD = '\033[1m'

class Item:
    def __init__(self, count=3, max_count=16):
        self._count = count
        self._max_count = max_count

    def update_count(self, val):
        if val <= self._max_count:
            self._count = val
            return True
        else:
            return False

    # Свойство объекта. Не принимает параметров кроме self, вызывается без круглых скобок
    # Определяется с помощью декоратора property
    @property
    def count(self):
        return self._count

    # Ещё один способ изменить атрибут класса
    @count.setter
    def count(self, val):
        self._count = val
        if val <= self._max_count:
            self._counts = val
        else:
            pass

    @staticmethod
    def static():
        print('I am function')

    @classmethod
    def my_name(cls):
        return cls.__name__

    def __add__(self, num):
        """ Сложение с числом """
        return self.count + num

    def __mul__(self, num):
        """ Умножение на число """
        return self.count * num

    def __len__(self):
        """ Получение длины объекта """
        return self.count

    def __gt__(self, num):
        return self.count > num

    def __lt__(self, num):
        """ Сравнение меньше """
        return self.count < num

    def __le__(self, num):
        """ Сравнение меньше """
        return self.count <= num

    def __ge__(self, num):
        """ Сравнение меньше """
        return self.count >= num

    def __eq__(self, num):
        """ Сравнение меньше """
        return self.count == num

    def __ne__(self, num):
        """ Сравнение меньше """
        return self.count != num

    def __sub__(self, num):
        """ Сравнение меньше """
        return self.count - num

    def __truediv__(self, num):
        """ Сравнение меньше """
        return self.count / num

    def __iadd__(self, num):
        """ Сравнение меньше """
        if self._max_count < (self.count + num):
            raise ValueError("num is so big to adds")
        self._count += num
        return self._count

    def __isub__(self, num):
        """ Сравнение меньше """
        if 0 > (self._count - num):
            raise ValueError("num is so big to sub")
        self._count -= num
        return self._count

    def __imul__(self, num):
        """ Сравнение меньше """
        if self._max_count < (self._count * num):
            raise ValueError("num is so big to mul")
        # self._count *= num
        # return self.count
        # self.count(self.count * num)
        self.count = num
        return  self.count

    def __itruediv__(self, num):
        """ Сравнение меньше """
        if num == 0:
            raise ValueError("Num can't be zero")
        self._count /= num
        return self._count



item = Item(count=5, max_count=100)
print(item + 5, item * 10, item / 2, item, item._max_count)#, item.count, item._max_count)
print(item / 5)
# item += 1500
print(item)
item *= 2
# print(item , item)


class Fruit(Item):
    def __init__(self, ripe=True, **kwargs):
        super().__init__(**kwargs)
        self._ripe = ripe


class Food(Item):
    def __init__(self, saturation=0, **kwargs):
        super().__init__(**kwargs)
        self._saturation = saturation

    @property
    def eatable(self):
        return self._saturation > 0

    def extract(self, num=1):
        if self.count < num:
            print(RED + "Exception: " + RESET + "there isn't enought items in ", self)
        else:
            self._count = self._count - num


class Orange(Fruit, Food):  #eatable
    def __init__(self, ripe=0, count=1, max_count=32, saturation=0):
        super().__init__(ripe=ripe, count=count, max_count=max_count, saturation=saturation)
        self._color = "orange"
        # self._saturation = saturation
    @property
    def color(self):
        return self._color

class SpoiledMango(Fruit, Food):   #not eatable
    def __init__(self, ripe=0, count=1, max_count=32,saturation=0):
        super().__init__(ripe=ripe, count=count, max_count=max_count)
        self._color = "yellow"
        self._saturation = saturation
    @property
    def color(self):
        return self._color

class Bread(Food):
    def __init__(self, count=1, max_count=32,saturation = 0):
        super().__init__(count=count, max_count=max_count)
        self._color = "bright-brown"
        self._saturation = saturation
    @property
    def color(self):
        return self._color


class SpoiledFish(Food):
    def __init__(self, count=1, max_count=32, saturation = 0):
        super().__init__(count=count, max_count=max_count)
        self._color = "red"
        self._saturation = saturation
    @property
    def color(self):
        return self._color

# class Inventory(list):
class Inventory():
    def __init__(self, length=1):
        self._lst = [None] * length

    def __getitem__(self, index):
        if index < len(self._lst):
            return self._lst[index]
        raise IndexError("Out of range")

    def __str__(self):
        # return(str(self._lst))
        # return "list is " + str(self._lst)
        return str(self._lst)

    # def __repr__(self):
    #     # return(str(self._lst))
    #     return self._lst

    def append(self, el):
        if isinstance(el, Food) and el.eatable:
            self._lst.append(el)
            print(str(el), GREEN + "was added in the inventory" + RESET)
        else:
            print(str(el), YELLOW + "this food isn't eatable, it can't be added to inventory" + RESET)
            # raise ValueError("Item isn't eatable")
    def take_food(self, index, num=1):
        if index >= len(self._lst):
            print(RED + "Exception" + RESET + " out of range")
        else:
            # self._lst[index].extract(num)
            self._lst[index]._count -= num
            if self._lst[index]._count == 0:
                self._lst[index] = None

    def print_detailed_info(self):
        print("\033[34m" + "------DETAILED BACKPACK INFO BELOW:------" + RESET)
        for el in self._lst:
            if(el):
                print(el, "count:\033[34m", el.count, "\033[0m", "max_count:\033[34m", el._saturation, "\033[0m", "edible:\033[34m", str(el.eatable), "\033[0m")
        print("\033[34m------end of detailed info------\033[0m")




backpack = Inventory(0)
# orange = Orange()
# backpack.append(Orange())
print(backpack)
backpack.append(Orange(count=10, saturation=0))
backpack.append(Orange(count=10, saturation=100))
backpack.print_detailed_info()
backpack.take_food(0, 2)
backpack.append(SpoiledMango())
backpack.append(Bread(count=5, saturation=10))
backpack.append(SpoiledFish(saturation=0))
print("backpack is:" + BOLD, backpack, RESET)
print("0 index:", backpack[0])
backpack.print_detailed_info()
backpack.take_food(1, 5)
backpack.print_detailed_info()
