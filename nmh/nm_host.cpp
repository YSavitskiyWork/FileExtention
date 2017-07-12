#include <iostream>
#include <string>
#include <vector>
#include "thread_safe_queue.h"
#include <fstream>
#include <sstream>
#include <dirent.h>
#include "json.hpp"

using json = nlohmann::json;

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

std::ifstream::pos_type get_file_size(const std::string &path)
{
    std::ifstream in(path, std::ifstream::ate | std::ifstream::binary);

    return in.tellg();
}

std::string get_message(const std::string &path, const size_t &size)
{
    json msg;
    json t;
    auto parent = path;
    while(parent.back() != '/')
        parent.pop_back();

    if(size > 1024)
    {
        t["parent"] = parent;
        t["file"] = "File too big";
        msg.push_back(t);
        return msg.dump();
    }
    if(size == 0)
    {
        t["parent"] = parent;
        t["file"] = "";
        msg.push_back(t);
        return msg.dump();
    }

    std::ifstream file(path);
    std::vector<char> buf(size);

    file.read(buf.data(), size);

    std::string file_str(buf.data());
    buf.erase(buf.begin());
    
    t["parent"] = parent;
    t["file"] = file_str;
    msg.push_back(t);

    return msg.dump();
}

std::string get_message(json &msg, const std::string &name, const std::string value, const int ind=-1)
{
    if (ind == -1)
        {
            msg["text"] = name;
            msg["value"] = value;
        }
    else
    {
        json t;
        t["text"+std::to_string(ind)] = name;
        t["value"+std::to_string(ind)] = value;
        msg.push_back(t);
    }

    return msg.dump();
}

std::string proc_path(const std::string &path)
{
    auto full_path = realpath(path.c_str(), NULL);

    std::string real_path(full_path);

    return real_path;
}

inline bool is_hidden(std::string name)
{
    return ((name.length() >=2 && name[1] != '.' && name[0] == '.') || (name.length() == 1 && name[0] == '.'));
}

std::string get_folders_contains_message(const std::string &path_)
{
    auto path = proc_path(path_);

    DIR *pdir = NULL;
    pdir = opendir(path.c_str());
    struct dirent *pent = NULL;
    
    json msg;
    json t;
    t["path"] = path;
    msg.push_back(t);
    
    pent = readdir(pdir);
    for (size_t i = 0; pent != NULL; pent = readdir(pdir), ++i)
    {
        std::string name(pent->d_name);
        if(is_hidden(name))
            continue;

        switch (pent->d_type)
        {
        case DT_REG:
        {
            std::string size = std::to_string(get_file_size(path+"/"+std::string(name))) + " bytes";
            get_message(msg, name, size, i);
            break;
        }
        case DT_DIR:
        {
            get_message(msg, name, "folder", i);
            break;
        }
        }
    }
    
    closedir(pdir);
    return msg.dump();
}

inline bool is_file(const std::string &path)
{
    DIR *pdir = NULL;
    pdir = opendir(path.c_str());
    bool file = pdir == NULL ? true : false;
    if(pdir)
        closedir(pdir);

    return file;
}

std::string create_sending_message(std::string path)
{
    std::string msg = "";
    int size = get_file_size(path);
    if (size >= 0)
    {
        if (is_file(path))
            msg = get_message(path, size);
        else
            msg = get_folders_contains_message(path);                        
    }

    return msg;
}

std::string get_path_from_json(const std::string &path)
{
    json msg = json::parse(path);

    return msg["text"];
}

inline void send_message(std::string msg)
{
    size_t len = msg.length();
    std::cout << char(len >> 0) << char(len >> 8) << char(len >> 16) << char(len >> 24);
    std::cout << msg << std::flush;
}

size_t receive_msg(std::string &msg)
{
    size_t length = get_length();
    //size_t length = 1;

    if (length == 0)
        return length;

    std::vector<char> json(length);
    
    std::cin.read(json.data(), length);
    std::string json_str(json.data());
    //костыль, который дает гарантию, что строка будет заданного размера.
    //по каким-то причинам, периодчисеки в json записывается больше, чем length
    json_str.replace(length, json_str.length(), "");
    msg = json_str;

    return length;
}

void receive_send()
{
    auto f_log = std::ofstream("log.txt", std::ofstream::app);

    size_t msg_numb = 0;
    while (true)
    {
        std::string msg_json = "";
        size_t length = receive_msg(msg_json);

        if (f_log)
            f_log << "length : " << length << std::endl;
            f_log << "input : " << msg_json << std::endl;

        if (!length)
            break;

        //msg_json = "{\"text\":\"/Users/developer/Documents/test.txt\"}";

        ++msg_numb;

        // output
    
        auto path = get_path_from_json(msg_json);

        std::string msg = create_sending_message(path);

        send_message(msg);

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

    receive_send();

    f_log = std::ofstream("log.txt", std::ofstream::app);
    f_log << "Native message host terminated!" << std::endl;
    f_log.close();
}