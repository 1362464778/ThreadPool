//
// Created by 13624 on 2023/9/24.
//

#ifndef THREADPOOL_THREADPOOL_H
#define THREADPOOL_THREADPOOL_H

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>

class ThreadPool {
private:
    std::vector<std::thread> threads;       //线程数组
    std::mutex mtx_tasks;                         //访问任务队列的互斥锁
    std::condition_variable condition;          //条件变量
    bool stop;
    std::queue<std::function<void()>> tasks;    //已绑定的任务队列

public:
    explicit ThreadPool(int num);
    ~ThreadPool();

    template<class F, class ...Args>
    void enqueue(F &&f, Args&&... args) {
        auto task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);     //将参数与任务函数绑定
        {
            std::unique_lock<std::mutex> lock(mtx_tasks);       //加锁访问队列
            tasks.emplace(std::move(task));
        }
        condition.notify_one();
    }
};


#endif //THREADPOOL_THREADPOOL_H
