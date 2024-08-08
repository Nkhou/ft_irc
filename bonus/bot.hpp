#ifndef BOT_HPP
#define BOT_HPP

#include <iostream>
#include <iostream>
#include <vector>
#include <string>
#include <sys/socket.h>
#include"srcs/server_bonus.hpp"
// #include "../srcs/server.hpp"
typedef struct date
{
    // private:
        int year;
        int month;
        int day;
} t_date;


class Bot {
    private:
        std::string name;
        int age;
        t_date date1;
        std::vector<std::string> messages;
    public:
        Bot();
        Bot(std::string name);
        ~Bot();
        void parce_command(std::string buffer);
        void parcingBuffer(std::vector<std::string> buffer);
        std::vector<std::string> getMessage();
        void execbot(int fd);
        int checkDate(std::string date);
        int checkDateparcing(std::string date, std::string nick, std::string hostname);
        // int getYear();
        // int getMonth();
        // int getDay();
        // void setYear(int year);
        // void setMonth(int month);
        // void setDay(int day);
        // Bot(std::string name, int age);
        // Bot(std::string name, int   age, std::
};
#endif