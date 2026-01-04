#include "../exercise.h"

// READ: 复制构造函数 <https://zh.cppreference.com/w/cpp/language/copy_constructor>
// READ: 函数定义（显式弃置）<https://zh.cppreference.com/w/cpp/language/function>


class DynFibonacci {
    size_t *cache;
    int capacity;
    int cached; // 已经初始化/计算好的元素个数（即下一个要算的位置）

public:
    // 实现动态设置容量的构造器
    DynFibonacci(int capacity_)
        : cache(nullptr), capacity(capacity_), cached(0) {
        ASSERT(capacity > 0, "capacity must be positive");

        cache = new size_t[capacity]();

        // 初始化 F0
        cache[0] = 0;
        cached = 1;

        // 初始化 F1（如果有空间）
        if (capacity > 1) {
            cache[1] = 1;
            cached = 2;
        }
    }

    // 修改为启用复制构造器
    DynFibonacci(DynFibonacci const &other)
        : cache(new size_t[other.capacity]()),
          capacity(other.capacity),
          cached(other.cached) {
        for (int i = 0; i < cached; ++i) {
            cache[i] = other.cache[i];
        }
    }

    // 禁用赋值运算符
    DynFibonacci &operator=(DynFibonacci const &) = delete;

    // 实现析构器，释放缓存空间
    ~DynFibonacci() {
        delete[] cache;
    }

    // 实现正确的缓存优化斐波那契计算
    size_t get(int i) {
        ASSERT(i >= 0 && i < capacity, "i out of range");

        while (cached <= i) {
            cache[cached] = cache[cached - 1] + cache[cached - 2];
            ++cached;
        }
        return cache[i];
    }

    // NOTICE: 不要修改这个方法
    // NOTICE: 名字相同参数也相同，但 const 修饰不同的方法是一对重载方法，可以同时存在
    //         本质上，方法是隐藏了 this 参数的函数
    //         const 修饰作用在 this 上，因此它们实际上参数不同
    size_t get(int i) const {
        if (i < cached) {
            return cache[i];
        }
        ASSERT(false, "i out of range");
    }
};

int main(int argc, char **argv) {
    DynFibonacci fib(12);
    ASSERT(fib.get(10) == 55, "fibonacci(10) should be 55");
    DynFibonacci const fib_ = fib;
    ASSERT(fib_.get(10) == fib.get(10), "Object cloned");
    return 0;
}
