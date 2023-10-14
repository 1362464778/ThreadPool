#include <iostream>
#include "ThreadPool.h"

void func1(int i){
    int sum = 8;
    sum += i;
    std::cout << "sum = " << sum << std::endl;
}


int main() {

    {
        ThreadPool pool(5);
        for (int i = 0; i < 10; i++) {
            pool.enqueue([i] {
                std::cout << "task : " << i << "is running" << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
                std::cout << "task : " << i << "is done" << std::endl;
            });
        }
        pool.enqueue(func1, 2);
    }
    std::cout << "success!" << std::endl;
    return 0;
}