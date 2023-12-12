#include <iostream>
#include <list>
#include <vector>

using namespace std;

template <typename T>
class LinearAllocator {
public:
    using value_type = T;   //necessary to work custom alloc
    LinearAllocator() : capacity(0), offset(0), memory(nullptr) {}  //necessary to work custom alloc

    LinearAllocator(size_t size) : capacity(size * 2), offset(0) {
        memory = reinterpret_cast<T*>(::operator new[](capacity * (sizeof(T) * 2)));
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
//    std::vector<int, LinearAllocator<int>> linearList(linearAllocator);
    std::vector<int, LinearAllocator<int>> linearList(linearAllocator);
    for (int i = 0; i < 10; ++i) {
        linearList.push_back(i);
    }
    for (const auto& elem : linearList) {
        std::cout << elem << "";
    }
    cout << "\n";
    exit(0);
    return 0;
}
