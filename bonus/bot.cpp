#include "bot.hpp"

Bot::Bot(std::string name)
{
    this->name = name;
}
Bot::Bot()
{
}
void Bot::parcingBuffer(std::string buffer)
{
    int i = 0;
    int c = 0;
    while (i < buffer.length())
    {
        if (buffer[i] == ':' && i == 0)
        {
            while(i < buffer.length())
            {   
                if (buffer[i] == '!' )
                    break;
                i++;
            }
            messages.push_back(buffer.substr(1, i - 1));
            c = i + 1;
        }
        else if (c != 0 && buffer[i] != ':')
        {
            while(i < buffer.length())
            {   
                if (buffer[i] == '@' )
                    break;
                i++;
            }
            messages.push_back(buffer.substr(1, i - 1));
        }
        else if (buffer[i] == ':')
            break;
        i++;
    }
    if ((i < buffer.length()) && i > 0)
    {
        int k = i + 1;
        while (k < buffer.length())
        {
            if (buffer[k] == ' ')
            {
                messages.push_back(buffer.substr(i + 1, k));
                i = k;
            }
            k++;
        }
        if (k > i)
        {
            messages.push_back(buffer.substr(i + 1, k));
            // std::cout << "----  "<< buffer.substr(i + 1, k) << " ------"<<std::endl;
            if (k != buffer.length())
                messages.push_back(buffer.substr(k + 1, buffer.length()));
        }
    }
}
int Bot::checkDate(std::string date)
{
    if (date.length() != 10)
    {
        // std::cout << "Error: bad input => " << date << std::endl;
        return 1;
    }
    if (date[4] != '-' || date[7] != '-')
    {
        // std::cout << "Error: bad input => " << date << std::endl;
        return 1;
    }
    for (int i = 0; i < 10; i++)
    {
        if (i == 4 || i == 7)
            continue;
        if (date[i] < '0' || date[i] > '9')
        {
            // std::cout << "Error: bad input => " << date << std::endl;
            return 1;
        }
    }
    int k = 0;
    // date.year = std::atoi(date.substr(0, 4));
    // date.month = std::atoi(date.substr(5, 7));
    // date.day = std::atoi(date.substr(7, 10));
    // std::cout << date.year<< std::endl;
    // std::cout << date.month<< std::endl;
    // std::cout << date.day<< std::endl;
    return 0;
}
// int date::getyear(){
//     return (year);
// }
// int date::getmonth(){
//     return month;
// }
// int date::getday()
// {
//     return day;
// }
// void date::setyear(int year){
//    this->year = year;
// }
// void date::setmonth(int month){
//    this->month = month;
// }
// void date::setday(int day)
// {
//     this->day = day;
// }
std::string notenghparam(std::string nick, std::string hostname)
{
    std::string msg = ":" + hostname + " 461 " + nick + " :Not enough parameters\r\n";
    return msg;
}
int Bot::checkDateparcing(std::string date)
{
    // t_date date1;
    // t_date date2;
    date1.year = std::stoi(date.substr(0, 4));
    if (date1.year < 2009 || date1.year > 2024)
        return -1;
    date1.month = std::stoi(date.substr(5, 2));
    if (date1.month < 1 || date1.month > 12)
        return -1;
    date1.day = std::stoi(date.substr(8, 2));
    if (date1.day < 1 || date1.day > 31)
        return -1;
    if (date1.month == 2 && date1.day > 29 && date1.year % 4 == 0)
        return -1;
    if (date1.month == 2 && date1.day > 28 && date1.year % 4 != 0)
        return -1;
    if (date1.month <= 7 && date1.month % 2 == 0 && date1.day > 30)
        return -1;
    if (date1.month > 7 && date1.month % 2 != 0 && date1.day > 30)
        return -1;
    return 0;
}
void Bot::execbot()
{
    if (messages[2] == "age")
    {
        if (checkDate(messages[3]) != 0 messages.size())
        {
            notenghparam(messages[0], messages[1]);
        }
        checkDateparcing(messages[3]);
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
