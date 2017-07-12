#include <iostream>
#include <string>
#include "thread_safe_queue.h"
#include <fstream>
#include <sstream>
#include <dirent.h>


size_t get_length()
{
    size_t length = 0;
    //auto f_log = std::ofstream("log.txt", std::ofstream::app);
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

inline void get_path_from_json(std::string &msg)
{
    auto str = msg.replace(msg.length() - 2, msg.length(), "");
    str = msg.replace(0, 9, "");
};

std::ifstream::pos_type get_file_size(const std::string &path)
{
    std::ifstream in(path, std::ifstream::ate | std::ifstream::binary);

    return in.tellg();
}

inline void in_json(std::string &msg)
{
    msg = "{" + msg + "}";
}

std::string get_message(const size_t &size)
{
    std::ostringstream msg;
    msg << "\"text\":\"" << size << " bytes\"";
    return msg.str();
}

inline std::string get_json_pairs(const std::vector<std::pair<std::string, std::string>> &pairs)
{
    std::string msg = "";

    std::for_each(pairs.begin(), pairs.end(), [&msg](const std::pair<std::string, std::string> &pair)
    {
        msg += "\"" + pair.first + "\":\"" + pair.second + "\",";
    });

    return msg.replace(msg.length() - 1, msg.length(), "");
}

std::string get_message(const std::string &name, const std::string mean, const int ind=-1)
{
    std::string msg = "";
    std::vector<std::pair<std::string, std::string>> pairs;
    pairs.push_back({ind == -1 ? "text" : "text"+std::to_string(ind), name});
    pairs.push_back({ind == -1 ? "mean" : "mean"+std::to_string(ind), mean});

    msg = get_json_pairs(pairs);

    return msg;
}

std::string get_folders_contains_message(const std::string &path)
{
    DIR *pdir = NULL;
    pdir = opendir(path.c_str());
    struct dirent *pent = NULL;
    std::ostringstream msg;
    //while ((pent = readdir(pdir)))
    pent = readdir(pdir);
    for (size_t i = 0; pent != NULL; pent = readdir(pdir), ++i)
    {
        switch (pent->d_type)
        {
        case DT_REG:
        {
            std::string size = std::to_string(get_file_size(path+"/"+std::string(pent->d_name))) + " bytes";
            msg << get_message(pent->d_name, size, i) << ",";
            break;
        }
        case DT_DIR:
        {
            msg << get_message(pent->d_name, "folder", i) << ",";
            break;
        }
        }
    }
    
    return msg.str().replace(msg.str().length()-1, msg.str().length(), "");
}

inline bool is_file(const std::string &path)
{
    DIR *pdir = NULL;
    pdir = opendir(path.c_str());

    return pdir == NULL ? true : false;
}

void recieve_send(std::shared_ptr<thread_safe_queue<std::string>> queue)
{
    auto f_log = std::ofstream("log.txt", std::ofstream::app);

    size_t msg_numb = 0;
    while (true)
    {
        size_t length = get_length();
        //size_t length = 1;

        if (f_log)
            f_log << "length : " << length << std::endl;

        if (length == 0)
            break;

        auto json = new char[length];
        
        std::cin.read(json, length);
        std::string json_str(json);
        delete[] json;

        //костыль, который дает гарантию, что строка будет заданного размера.
        //по каким-то причинам, периодчисеки в json записывается больше, чем length
        json_str.replace(length, json_str.length(), "");

        //проверка длины принятой строки
        if(length != json_str.length())
            {
                f_log << "\nUNEQUETION\n";
            }

        //std::string json_str = "{\"text\":\"/Users/developer/Documents/GitHub/FileExtention/application/\"}";

        queue->push(json_str);

        if (f_log)
            f_log << "input : " << json_str << std::endl;

        ++msg_numb;

        // output

        auto path = queue->pop();

        get_path_from_json(path);

        std::string msg = "";
        int size = get_file_size(path);
        if (size > 0)
        {
            if (is_file(path))
                msg = get_message(size);
            else
                msg = get_folders_contains_message(path);                        
        }

        in_json(msg);

        size_t len = msg.length();

        std::cout << char(len >> 0) << char(len >> 8) << char(len >> 16) << char(len >> 24);
        std::cout << msg << std::flush;

        if (f_log)
            f_log << "sending : " << msg << std::endl;

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