#include <iostream>
#include <list>

template <typename T, std::size_t InitialCapacity = 10>
class allocator {
public:
    using value_type = T;

    allocator() {
        reserveMemory();
    }

    template <typename U>
    struct rebind {
        typedef allocator<U> other;
    };

    T* allocate(std::size_t n) {
        if (n > capacity - size)
            reserveMemory();

        T* ptr = memory + size;
        std::cout << "Address of element " << size << " is: " << ptr << std::endl;
        size += n;
        return ptr;
    }

    void destroy(T* p) const {
        p->~T();
    }

    template <typename U, typename... Args>
    void construct(U* p, Args&&... args) const {
        new (p) U(std::forward<Args>(args)...);
    }

    void deallocate(T* ptr, std::size_t n) {
        size -= n;
        if (size == 0) {
            free(memory);
            memory = nullptr;
            capacity = 0;
        }
    }

private:
    T* memory = nullptr;
    T* originalMemory = nullptr;
    std::size_t size = 0;
    std::size_t capacity = 0;

    void reserveMemory() {
        capacity += InitialCapacity;
        T* newMemory = static_cast<T*>(std::malloc(capacity * sizeof(T)));
        if (!newMemory)
            throw std::bad_alloc();
        if (memory) {
            std::copy(memory, memory + size, newMemory);
            std::free(originalMemory);
        }
        memory = newMemory;
        originalMemory = memory;
    }
};

int main() {
    std::list<int, allocator<int>> list;
    for (int i = 0; i < 11; i++) {
        list.push_back(i + 20);
    }
    return 0;
}
