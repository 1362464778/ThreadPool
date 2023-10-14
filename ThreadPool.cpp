//
// Created by 13624 on 2023/9/24.
//

#include <future>
#include "ThreadPool.h"

ThreadPool::ThreadPool(int num) : stop(false){
    for(int i = 0; i < num; i++)
    {
        threads.emplace_back([this](){
            while(true)
            {
                std::unique_lock<std::mutex> lock(mtx_tasks);
                condition.wait(lock, [this](){
                    return stop || !tasks.empty();
                });     //等待任务队列通知
                //一旦有任务，就从任务队列取出来执行
                if (stop && tasks.empty())
                    return;
                auto task = std::move(tasks.front());
                tasks.pop();
                lock.unlock();
                task();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(mtx_tasks);   //存疑
        stop = true;
    }
    condition.notify_all();
    for(auto& t: threads)   //由于是线程，所以必须用引用方式
        t.join();
}
//!!!!!!模板类不能定义在.cpp文件当中！！！
//template<class F, class ...Args>
//void ThreadPool::enqueue(F &&f, Args&&... args) {
//    auto func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);     //将参数与任务函数绑定
//    {
//        std::unique_lock<std::mutex> lock(mtx_tasks);       //加锁访问队列
//        tasks.emplace(std::move(func));
//    }
//    condition.notify_one();
//}
