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
