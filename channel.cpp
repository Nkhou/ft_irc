#include "channel.hpp"

Channel::Channel(std::string name)
{
    this->name = name;
}

void Channel::addUser(std::string user)
{
    users.push_back(user);
}

void Channel::removeUser(std::string user)
{
    for (int i = 0; i < users.size(); i++)
    {
        if (users[i] == user)
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

std::vector<std::string> Channel::getUsers()
{
    return users;
}

void Channel::printUsers()
{
    for (int i = 0; i < users.size(); i++)
    {
        std::cout << users[i] << std::endl;
    }
}

bool Channel::userExists(std::string user)
{
    for (int i = 0; i < users.size(); i++)
    {
        if (users[i] == user)
        {
            return true;
        }
    }
    return false;
}

void Channel::clearUsers()
{
    users.clear();
}

Channel::~Channel()
{
    users.clear();
}

void Channel::sendMessage(std::string message)
{
    for (int i = 0; i < users.size(); i++)
    {
        std::cout << users[i] << ": " << message << std::endl;
    }
}

void Channel::joinChannel(std::string user)
{
    if (!userExists(user))
    {
        addUser(user);
    }
}

void Channel::createChannel(std::string name)
{
    this->name = name;
}


void Channel::notifyUserJoin(std::string user)
{
    for (int i = 0; i < users.size(); i++)
    {
        std::cout << ":" <<  user << " JOIN " <<" #" << name<< std::endl;
    }
}