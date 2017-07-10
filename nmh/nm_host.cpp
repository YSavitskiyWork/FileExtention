#include <iostream>
#include <string>
#include "thread_safe_queue.h"
#include <fstream>
#include <sstream>
#include <dirent.h>


size_t get_length()
{
    size_t length = 0;
    auto f_log = std::ofstream("log.txt", std::ofstream::app);
    for (size_t i = 0; i < 4; ++i)
    {
        auto buf = getchar();
        if (buf < 0)
            return 0;

        //f_log << "char: " << buf << std::endl;
        length = length | (buf << i * 8);
    }

    return length;
}

void get_path_from_json(std::string &msg)
{
    auto str = msg.replace(msg.length() - 2, msg.length(), "");
    str = msg.replace(0, 9, "");
};

std::ifstream::pos_type get_file_size(const std::string &path)
{
    std::ifstream in(path, std::ifstream::ate | std::ifstream::binary);

    return in.tellg();
}

std::string get_file_message(const size_t &size)
{
    std::ostringstream sstream;
    sstream << "{\"text\":\"" << size << "\"}";
    return sstream.str();
}

std::string get_folder_message(const std::string &path)
{
    DIR *pdir = NULL;
    pdir = opendir(path.c_str());
    struct dirent *pent = NULL;
    while ((pent = readdir(pdir)))
    {
        switch (pent->d_type)
        {
        case DT_REG:
            break;
        case DT_DIR:
            break;
        }
    }
}

bool is_file(const std::string &path)
{
    DIR *pdir = NULL;
    pdir = opendir(path.c_str());

    return pdir == NULL ? true : false;
}

void recieve_send(std::shared_ptr<thread_safe_queue<std::string>> queue)
{
    auto f_log = std::ofstream("log.txt", std::ofstream::app);

    size_t message_number = 0;
    while (true)
    {
        size_t length = get_length();

        if (f_log)
            f_log << "length : " << length << std::endl;

        if (length == 0)
            break;

        auto json = new char[length];
        std::cin.read(json, length);
        std::string json_str(json);
        //std::string json_str = "{\"text\":\"/Users/developer/Documents/test.txt\"}";

        queue->push(json_str);

        if (f_log)
            f_log << "input : " << json_str << std::endl;

        ++message_number;
        delete[] json;

        // output

        auto path = queue->pop();

        get_path_from_json(path);

        std::string message = "";
        int size = get_file_size(path);
        if (size > 0)
        {
            if (is_file(path))
                message = get_file_message(size);
            else
                message = get_folder_message(path);                        
        }

        size_t len = message.length();

        std::cout << char(len >> 0) << char(len >> 8) << char(len >> 16) << char(len >> 24);
        std::cout << message << std::flush;

        if (f_log)
            f_log << "sending : " << message << std::endl;
    }

    if (f_log)
        f_log << "recieving/sending : end" << std::endl;

    f_log.close();
}

int main()
{
    auto f_log = std::ofstream("log.txt");
    f_log << "Native message host created!" << std::endl;
    f_log.close();

    auto queue = std::make_shared<thread_safe_queue<std::string>>();

    recieve_send(queue);

    //auto reading = std::thread(reading_thread, queue);

    //auto writing = std::thread(writing_thread, queue);
    f_log = std::ofstream("log.txt", std::ofstream::app);
    f_log << "Native message host terminated!" << std::endl;
    f_log.close();

    //reading.join();
    //writing.join();
}