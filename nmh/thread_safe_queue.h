#pragma once

#include <string>
#include <mutex>
#include <queue>

template<typename T> 
class thread_safe_queue
{
    public:
        void push(T);
        T pop();
        bool empty();
    
    private:
        std::mutex mutex;
        std::queue<T> queue;
};