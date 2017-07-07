#include <iostream>
#include <memory>
#include <mutex>
#include <regex>
#include <string>
#include <thread>
#include "thread_safe_queue.h"

size_t get_length()
{
    size_t length = 0;
    for (size_t i = 0; i < 4; ++i)
    {
        size_t buf = getchar();
        length = length | (buf << i*8);
    }

    return length;
}

void reading_thread(std::shared_ptr<thread_safe_queue<std::string>> queue)
{
    size_t message_number = 0;
    while (true)
    {
        size_t length = get_length();
        
        if(length == -1)
            return;

        auto json = new char[length];
        std::cin.read(json, length);

        queue->push(json);

        ++message_number;
    }
}

void write(std::shared_ptr<thread_safe_queue<std::string>> queue)
{
    while (!queue->empty())
    {
        auto message = queue->pop();

        size_t len = message.length();

        std::cout << char(len >> 0) << char(len >> 8) << char(len >> 16) << char(len >> 24);
        std::cout << message << std::flush;
    }

    return;
}

int main()
{
    auto queue = std::make_shared<thread_safe_queue<std::string>>();

    reading_thread(queue);
    auto reading = std::thread(reading_thread, queue);

    reading.join();
}