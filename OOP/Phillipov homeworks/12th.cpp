<<<<<<< HEAD
#include <cstddef>
#include <iostream>
#include <list>

template <typename T>
class CustomAllocator {
public:
    using value_type = T;

    CustomAllocator(size_t capacity) : capacity_(capacity), memory_(new char[sizeof(T) * capacity]) {}

    ~CustomAllocator() {
        delete[] memory_;
    }

    T* allocate(std::size_t n) {
        if (n > capacity_) {
            throw std::bad_alloc();
        }

        if (currentPosition_ + n > capacity_) {
            std::cerr << "Out of memory!\n";
            throw std::bad_alloc();
        }

        T* ptr = reinterpret_cast<T*>(memory_ + currentPosition_);
        currentPosition_ += n;

        return ptr;
    }

    void deallocate(T* p, std::size_t n) {
        // Ничего не делаем, так как выделенная память освобождается вместе с аллокатором
    }

private:
    size_t capacity_;
    char* memory_;
    size_t currentPosition_ = 0;
};

int main() {
    // Создаем контейнер list с использованием нашего аллокатора
    std::list<int, CustomAllocator<int>> myList(5, CustomAllocator<int>(10));

    // Добавляем элементы в контейнер
    for (int i = 0; i < 5; ++i) {
        myList.push_back(i);
    }

    // Выводим элементы контейнера
    for (const auto& elem : myList) {
        std::cout << elem << " ";
    }
    std::cout << "\n";

    return 0;
}
=======
#include <iostream>
#include <list>
#include <vector>

using namespace std;

template <typename T>
class LinearAllocator {
public:
    using value_type = T;   //necessary to work custom alloc
    LinearAllocator() : capacity(0), offset(0), memory(nullptr) {}  //necessary to work custom alloc

    LinearAllocator(size_t size) : capacity(size), offset(0) {
        memory = reinterpret_cast<T*>(::operator new[](capacity * sizeof(T)));
    }

    ~LinearAllocator() {
        ::operator delete[](memory);
    }

    T* allocate(size_t n) {
        if (offset + n > capacity)
            throw bad_alloc();

        T* res = memory + offset;
        offset += n;
        return res;
    }

    void deallocate(T* p, size_t n) {
//        ::operator delete[](memory);
    }

private:
    size_t capacity;
    size_t offset;
    T* memory;
};


int main() {
    LinearAllocator<int> linearAllocator(15);
    std::vector<int, LinearAllocator<int>> linearList(linearAllocator);
    for (int i = 0; i < 5; ++i) {
        linearList.push_back(i);
    }
    for (const auto& elem : linearList) {
        std::cout << elem << " ";
    }
    exit(0);
    return 0;
}
>>>>>>> 1e06dbb26eb373d18eaf1595de135176f6f0b386
