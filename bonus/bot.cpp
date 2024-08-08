#include "bot.hpp"

Bot::Bot(std::string name)
{
    this->name = name;
}
Bot::Bot()
{
}
void Bot::parcingBuffer(std::vector<std::string> buffer)
{
    int i = 0;
    int c = 0;
    while(i < buffer[0].length())
    {   
        if (buffer[0][i] == '!' )
            break;
        i++;
    }
    messages.push_back(buffer[0].substr(1, i - 1));
    c = i + 1;
    while(i < buffer[0].length())
    {   
        if (buffer[0][i] == '@' )
            break;
        i++;
    }
     c = i + 1;
    while (buffer[0][i] != ' ' && i < buffer[0].length())
    {
        i++;
    }
    messages.push_back(buffer[0].substr(c, i - 1));
    std::string str;


    if (buffer[3][0] == ':' && buffer[3].length() > 1)
    {
        str = buffer[3].substr(1 , buffer[3].length());
    }
    else 
        str = buffer[4];
    // std::cout << "i: " << std::endl;
    messages.push_back(str.substr(0 , str.length()));
    if (buffer.size() > 4)
    {
       i = 5;
       while (i < buffer.size())
       {
         str = str + buffer[i] + " ";
         i++;
       }

    // std::cout << "i: " << std::endl;
        messages.push_back(str.substr(0 , str.length() - 1));
    }
}
int Bot::checkDate(std::string date)
{
    if (date.length() != 10)
    {
        return 1;
    }
    if (date[4] != '-' || date[7] != '-')
    {
        return 1;
    }
    for (int i = 0; i < 10; i++)
    {
        if (i == 4 || i == 7)
            continue;
        if (date[i] < '0' || date[i] > '9')
        {
            return 1;
        }
    }
    int k = 0;
    return 0;
}

std::string notenghparam(std::string nick, std::string hostname)
{
    std::string msg = "privmsg " + nick + " 461 " + nick + " :Not enough parameters\r\n";
    return msg;
}
int Bot::checkDateparcing(std::string date, std::string nick, std::string hostname)
{
    // t_date date1;
    // t_date date2;
    date1.year = std::stoi(date.substr(0, 4));
    if (date1.year < 2009 || date1.year > 2024)
        return (notenghparam(nick, hostname),-1);
    date1.month = std::stoi(date.substr(5, 2));
    if (date1.month < 1 || date1.month > 12)
        return (notenghparam(nick, hostname),-1);
    date1.day = std::stoi(date.substr(8, 2));
    if (date1.day < 1 || date1.day > 31)
        return (notenghparam(nick, hostname),-1);
    if (date1.month == 2 && date1.day > 29 && date1.year % 4 == 0)
        return (notenghparam(nick, hostname),-1);
    if (date1.month == 2 && date1.day > 28 && date1.year % 4 != 0)
        return (notenghparam(nick, hostname),-1);
    if (date1.month <= 7 && date1.month % 2 == 0 && date1.day > 30)
        return (notenghparam(nick, hostname),-1);
    if (date1.month > 7 && date1.month % 2 != 0 && date1.day > 30)
        return (notenghparam(nick, hostname),-1);
    return 0;
}
void Bot::execbot(int fd )
{
    if (messages[2] == "age")
    {
        if (checkDate(messages[3]) != 0)
        {
            // std::cout << "age" << std::endl;
            std::string msg = notenghparam(messages[0], messages[1]);
            // std::cout << "msg: " << msg << std::endl;
            // std::cout << fd << std::endl;
            if (send(fd, msg.c_str(), msg.length(), 0) == -1) {
                // std::cerr << "Send failed: " << strerror(errno) << std::endl;
                return;
            }
            return;
        }
            sleep(1);
        checkDateparcing(messages[3],messages[0], messages[1]);
        std::cout << "year: " << date1.year << std::endl;
        std::cout << "month: " << date1.month << std::endl;
        std::cout << "day: " << date1.day << std::endl;
    }
    // else if (messages[2] == "LIST")
    // {
    //     //list of user in server
    // }
    // else if (messages[2] == "commands")
    // {
    //     //list all commands
    // }
    // else if (messages[2] == "jock")
    // {

    // } 
}
std::vector<std::string> Bot::getMessage()
{
    return messages;
}
Bot::~Bot()
{
}
