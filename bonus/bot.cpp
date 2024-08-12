#include "bot.hpp"

Bot::Bot(std::string name)
{
    this->name = name;
}
Bot::Bot()
{
}
void Bot::clearMessage()
{
    messages.clear();
}
void Bot::parcingBuffer(std::vector<std::string> buffer)
{
    int i = 1;
    int c = 0;

    // for (int i = 0; i < buffer.size(); i++)
    // {
    //     std::cout << "buffer: " << buffer[i] << std::endl;
    // }
    // std::cout << "buffer: " << buffer.size() << std::endl;
    if (buffer.size() < 4)
    {
        return;
    }
    while(i < buffer[0].length())
    {   
        if (buffer[0][i] == '!' )
            break;
        i++;
    }
    messages.push_back(buffer[0].substr(1, i - 1));
    c = i + 2;
    while(i < buffer[0].length())
    {   
        if (buffer[0][i] == '@' )
            break;
        i++;
    }
     c = i + 1;
    // std::cout << "c :" <<buffer[0] << std::endl;
    // std::cout << buffer[0][c] <<std::endl;
    while (i < buffer[0].length() && buffer[0][i] != ' ' && i < buffer[0].length())
    {
        i++;
    }
    messages.push_back(buffer[0].substr(c, i - 1));
    // std::cout << "i :" <<messages[1] << std::endl;
    std::string str;

    i = 1;
    if (buffer[3][0] == ':')
    {
        while (i < buffer[3].length() && buffer[3][i] == ':')
        {
            i++;
        }
        if (i < buffer[3].length())
        {
        // std::cout << "bchakh "<<buffer[3].substr(i, buffer[3].length()) << std::endl;
            str = buffer[3].substr(i , buffer[3].length());
            i = 4;
        }
        else
        {
            // std::cout << "iiii: " << str << std::endl;
            // std::cout << "hello" << std::endl;
            str = buffer[4];
            i = 5;
        }
    }
    else
    {
        str = buffer[3];
        i = 4;
    }
    messages.push_back(str.substr(0 , str.length()));
    std::cout << "str  fdd: " << str << std::endl;
    if (i < buffer.size())
    {
        str = "";
        while (i < buffer.size())
        {
            str = str + buffer[i] + " ";
            i++;
        }
        messages.push_back(str.substr(0 , str.length() - 1));
        return;
    }
}
int Bot::checkDate(std::string date)
{
    int i = 0;
    // while ( i < date.length())
    // {
    //     std::cout << "i = "<<i<<" date "<<date[i] << std::endl;
    //     i++;
    // }
    if (date.length() != 10)
    {
    //  std::cout << date<<"------"<< std::endl;
    //  std::cout << date[12]<< std::endl;
        return 1;
    }
    if (date[4] != '-' || date[7] != '-')
    {
        // std::cout << "hello" << std::endl;
        // std::cout << date[4] << std::endl;
        // std::cout << date[7] << std::endl;
        return 1;
    }
    for (int i = 0; i < 10; i++)
    {
        if (i == 4 || i == 7)
            continue;
        if (date[i] < '0' || date[i] > '9')
        {
            // std::cout << "hello" << std::endl;
            return 1;
        }
    }
    int k = 0;
    return 0;
}

std::string notenghparam(std::string nick, std::string hostname)
{
    std::string msg = "privmsg " + nick + " 461 " +  " :Not enough parameters\r\n";
    return msg;
}
int Bot::checkDateparcing(std::string date, std::string nick, std::string hostname)
{
    // t_date date1;
    // t_date date2;
    date1.year = std::stoi(date.substr(0, 4));
    if (date1.year < 1900 || date1.year > 2024)
        return ( notenghparam(nick, hostname), -1);
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
int parcedate(std::string date, t_date *date1)
{
    t_date date2;
    int i = 0;
    int k ;
    while (date.length() != 0 && i < date.length())
    {
        if (date[i] == ' ')
        {
            i++;
            break;
        }
        i++;
    }
    k = i;
    i++;
    while (i < date.length())
    {
        if (date[i] == ' ')
        {
            i++;
            break;
        }
        i++;
    }
    i++;
    int j = 0;
    std::string str = date.substr(k, i - k);
    if (str == "Jan")
        date2.month = 1;
    else if (str == "Feb")
        date2.month = 2;
    else if (str == "Mar")
        date2.month = 3;
    else if (str == "Apr")
        date2.month = 4;
    else if (str == "May")
        date2.month = 5;
    else if (str == "Jun")
        date2.month = 6;
    else if (str == "Jul")
        date2.month = 7;
    else if (str == "Aug")
        date2.month = 8;
    else if (str == "Sep")
        date2.month = 9;
    else if (str == "Oct")
        date2.month = 10;
    else if (str == "Nov")
        date2.month = 11;
    else if (str == "Dec")
        date2.month = 12;
    // std::cout << "str: " << str << std::endl;
    // while (i < )
    k = i;
    i++;
    while (i < date.length())
    {
        if (date[i] == ' ')
        {
            i++;
            break;
        }
        i++;
    }
    str = date.substr(k, i - k);
    date2.day = std::stoi(str);
    // std::cout << "day: " << date2.day << std::endl;
    k = i;
    i++;
    while (i < date.length())
    {
        if (date[i] == ' ')
        {
            i++;
            break;
        }
        i++;
    }

    k = i;
    i++;
    while (i < date.length())
    {
        if (date[i] == ' ' || date[i] == '\n')
        {
            i++;
            break;
        }
        i++;
    }
    str = date.substr(k, i - k);
    date2.year = std::stoi(str);
    int year = date2.year - date1->year;
    if (date2.month < date1->month)
    {
        year--;
    }
    else if (date2.month == date1->month && date2.day < date1->day)
    {
        year--;
    }
    if (year < 0)
    {
        year = 0;
    }
    std::cout << "year: " << year << std::endl;
    // std::cout << "year: " << date2.year << std::endl;
    // date12->year = std::stoi(date.substr(20, 4));
    // date12->month = std::stoi(date.substr(4, 3));
    // date12->day = std::stoi(date.substr(8, 2));
    return year;
}
std::vector<std::string>  Bot::getJocks()
{
    return jocks;
}
void Bot::addjocks()
{
    // std::vector<std::string> jocke;
    jocks.push_back("Why did the scarecrow win an award? Because he was outstanding in his field.");
    jocks.push_back("What do you call a belt made out of watches? A waist of time.");
    jocks.push_back("Why don't scientists trust atoms? Because they make up everything.");
    jocks.push_back("What do you call cheese that isn't yours? Nacho cheese.");
    jocks.push_back("Why couldn't the bicycle find its way home? It lost its bearings.");
    jocks.push_back("Why did the tomato turn red? Because it saw the salad dressing.");
    jocks.push_back("What do you call a fish wearing a crown? A king fish.");
    jocks.push_back("Why did the golfer bring two pairs of pants? In case he got a hole in one.");
    jocks.push_back("What do you call a pile of cats? A meowtain.");
    jocks.push_back("Why did the math book look sad? Because it had too many problems.");
    jocks.push_back("What do you call a bear with no teeth? A gummy bear.");
    // jocke.push_back("Why did the scarecrow win an award? Because he was outstanding in his field.");
}
void Bot::execbot(int fd )
{
    std::cout << "hello" <<messages.size()<< std::endl;
    for (int i = 0; i < messages.size(); i++)
    {
        std::cout << "i  = " << i << " " << messages[i] << std::endl;
    }
    if (messages.size() < 3)
    {
        return;
    }

    if (messages.size() >= 1 && messages[2] == "age")
    {
        if (checkDate(messages[3]) != 0)
        {
            
            std::string msg = notenghparam(messages[0], messages[1]);
            // std::cout << "msg: " << msg << std::endl;
            // std::cout << fd << std::endl;
            if (send(fd, msg.c_str(), msg.length(), 0) == -1) {
                messages.clear();
                // std::cerr << "Send failed: " << strerror(errno) << std::endl;
                return;
            }
            return;
            sleep(1);
            messages.clear();
        }
            // sleep(1);
        checkDateparcing(messages[3],messages[0], messages[1]);
        if (checkDateparcing(messages[3],messages[0], messages[1]) == -1)
        {
            messages.clear();
            return;
        }
        std::time_t currentTime = std::time(NULL);
        int age = parcedate(std::ctime(&currentTime), &date1);
        std::string msg = "privmsg " + messages[0] +  " :Your age is " + std::to_string(age) + "\r\n";
        date1.year = 0;
        date1.month = 0;
        date1.day = 0;
        if (send(fd, msg.c_str(), msg.length(), 0) == -1) {
            messages.clear();
            // std::cerr << "Send failed: " << strerror(errno) << std::endl;
            return;
        }
        sleep(1);
        messages.clear();
    }
    else if (messages[2] == "cmds")
    {
        // std::cout << "hello" << std::endl;
        // std::cout << "msg: " << messages[0] << std::endl;
        std::string msg = "privmsg " + messages[0] +  " :!age <date> - to get your age   /" +  "!jock - to get a jock   /" +  "!cmds - to get all commands   /" +  "!LIST - to get all user in server   /" +  "!JOIN <#channel> - to join a channel   /" 
   +  " :!KICK <#channel> <user> - to kick a user from a channel   /" +  "!TOPIC <#channel> <topic> - to change the topic of a channel   /" +  "!MSG <user> <message> - to send a private message to a user   /" +  "!QUIT - to quit the server\r\n";
        // std::cout << "msg: " << msg << std::endl;
        if (send(fd, msg.c_str(), msg.length(), 0) == -1) {
            messages.clear();
            // std::cerr << "Send failed: " << strerror(errno) << std::endl;
            return;
        }
        sleep(1);
        messages.clear();
    }
    else if (messages[2] == "jock")
    {
        int i = rand() % jocks.size();
        std::string msg = "privmsg " + messages[0] +  " :" + jocks[i] + "\r\n";
        // std::string msg = "privmsg " + messages[0] +  " :Why did the scarecrow win an award? Because he was outstanding in his field.\r\n";
        if (send(fd, msg.c_str(), msg.length(), 0) == -1) {
            messages.clear();
            // std::cerr << "Send failed: " << strerror(errno) << std::endl;
            return;
        }
        sleep(1);
        messages.clear();
    } 
}
std::vector<std::string> Bot::getMessage()
{
    return messages;
}
Bot::~Bot()
{
}
