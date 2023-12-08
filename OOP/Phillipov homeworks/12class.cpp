#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <tuple>

using namespace std;

template<typename T>
struct logging_allocator {
    using value_type = T;
    T *allocate(std::size_t n) const {
        std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" << std::endl;
        auto p = std::malloc(n * sizeof(T));
        if (!p)
            throw std::bad_alloc();
        return reinterpret_cast<T *>(p);
    }
    void deallocate(T *p, std::size_t n) const {
        std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" << std::endl;
        std::free(p);
    }
    template<typename U, typename ...Args>
    void construct(U *p, Args &&...args) const {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        new(p) U(std::forward<Args>(args)...);
    };
    void destroy(T *p) const {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        p->~T();
    }
};


int main(int, char *[]) {
    auto v = list<int, logging_allocator<int>>{};
    for (size_t i = 0; i < 1; ++i) {
        v.emplace_back(i);
        std::cout << "---" << std::endl;
    }
    for (auto i: v) {
        std::cout << i << std::endl;
    }
    return 0;
}
