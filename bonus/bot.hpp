#ifndef BOT_HPP
#define BOT_HPP

#include <iostream>
#include <vector>
#include <string>
class Bot {
    private:
        std::string name;
        int age;
        std::vector<std::string> messages;
    public:
        Bot();
        Bot(std::string name);
        ~Bot();
        void parce_command(std::string buffer);
        
        // Bot(std::string name, int age);
        // Bot(std::string name, int   age, std::
};
#endif