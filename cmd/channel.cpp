#include "channel.hpp"

Channel::Channel(std::string name)
{
    this->name = name;
}

Channel::Channel(std::string name, int o, int fd, std::vector<client> clients)
{
    this->name = name;
    if (o)
    {
        operators.push_back(clients[fd]);
    }
}

std::vector<std::string> Channel::getMode()
{
    return mode;
}

void Channel::setMode(std::string mode)
{
    this->mode.push_back(mode);
}
void Channel::setTopic(std::string topic)
{
    this->topic = topic;
}
void Channel::setKey(std::string key)
{
    this->key = key;
}
std::string Channel::getTopic()
{
    return topic;
}
std::string Channel::getKey()
{
    return key;
}
int Channel::addUser(cli client, int o)
{
    for (unsigned long i = 0; i < users.size(); i++)
    {
        if (users[i].fd == client.fd)
        {
            return 1;
        }
    }
    if (users.size() >= maxUsers)
    {
        return 2;
    }
    users.push_back(client);
    if (o)
    {
        client.nickname = "@" + client.nickname;
        operators.push_back(client);
    }
    return 0;
}
void Channel::PrintTopic(int fd)
{
    write(fd, "TOPIC ", 6);
    write(fd, name.c_str(), name.length());
    write(fd, " : ", 2);
    write(fd, topic.c_str(), topic.length());
    write(fd, "\n", 1);
}
void Channel::setMaxUsers(int max)
{
    this->maxUsers = max;
}
// int Channel::getMaxUsers()
// {
//     return maxUsers;
// }
void Channel::removeUser(std::string user)
{
    for (unsigned long i = 0; i < users.size(); i++)
    {
        if (users[i].nickname == user)
        {
            users.erase(users.begin() + i);
            return;
        }
    }
}

std::string Channel::getName()
{
    return name;
}

std::vector<cli> Channel::getUsers()
{
    return users;
}

void Channel::printUsers()
{
    for (unsigned long i = 0; i < users.size(); i++)
    {
        std::cout << users[i].nickname << std::endl;
    }
}

// bool Channel::userExists(cli *client, int fd)
// {
//     for (unsigned long i = 0; i < client.size(); i++)
//     {
//         if (clients[i].fd == fd)
//         {
//             for (unsigned long j = 0; j < users.size(); j++)
//             {
//                 if (users[j] == client[i].nickname)
//                 {
//                     return true;
//                 }
//             }
//         }
//     }
//     return false;
// }


void Channel::clearUsers()
{
    users.clear();
}

Channel::~Channel()
{
    users.clear();
}

// void Channel::sendMessage(std::string message)
// {
//     for (unsigned long i = 0; i < users.size(); i++) 
//     {
//         std::cout << users[i]. << ": " << message << std::endl;
//     }
// }

// void Channel::joinChannel(cli *client, int client_fd)
// {
//     if (!userExists(client, client_fd))
//     {
//         addUser(client->getNickName());
//         notifyUserJoin(client->getNickName());
//     }
// }

void Channel::createChannel(std::string name)
{
    this->name = name;
}

// void Channel::createChannel(std::string name, int o)
// {
//     this->name = name;
//     if (o)
//     {
//         operators.push_back(name);
//     }
// }

void Channel::addOperator(cli user)
{
    operators.push_back(user);
}
void Channel::notifyUserJoin(std::string user)
{
    for (unsigned long i = 0; i < users.size(); i++)
    {
        write(users[i].fd, ":", 1);
        write(users[i].fd, user.c_str(), user.length());
        write(users[i].fd, " JOIN ", 6);
        write(users[i].fd, name.c_str(), name.length());
        write(users[i].fd, "\n", 1);
    }
}
 
int Channel::getUserCount()
{
    return users.size();
}

int Channel::getOperatorCount()
{
    return operators.size();
}

long unsigned int Channel::getMaxUsers()
{
    return maxUsers;
}

std::vector<client> Channel::getOperators()
{
    return operators;
}

// void Channel::addOperator(cli user)
// {
//     operators.push_back(user);
// }

void Channel::removeOperator(std::string user)
{
    for (unsigned long i = 0; i < operators.size(); i++)
    {
        if (operators[i].nickname == user)
        {
            operators.erase(operators.begin() + i);
            return;
        }
    }
}

int Channel::getChannelSize()
{
    return users.size();
}

void Channel::setuserCount(int userCount)
{
    this->userCount = userCount;
}

void Channel::setoperatorCount(int operatorCount)
{
    this->operatorCount = operatorCount;
}