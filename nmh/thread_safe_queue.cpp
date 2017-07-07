#include "thread_safe_queue.h"

template<typename T> void thread_safe_queue<T>::push(T element)
{
    std::lock_guard<std::mutex> lock(mutex);
    queue.push(element);
}

template<typename T> T thread_safe_queue<T>::pop()
{
    std::lock_guard<std::mutex> lock(mutex);
    auto element = queue.front();
    queue.pop();
    return element;
}

template<typename T> bool thread_safe_queue<T>::empty()
{
    std::lock_guard<std::mutex> lock(mutex);
    return queue.empty();
}

template class thread_safe_queue<std::string>;