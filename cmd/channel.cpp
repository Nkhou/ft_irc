#include "channel.hpp"

Channel::Channel(std::string name)
{
    this->name = name;
    limits = false;
}

Channel::Channel(std::string name, int o, int fd, std::vector<client> clients)
{
    this->name = name;
    limits = false;
    // this->maxUsers = std::numeric_limits<size_t>::max();
    if (o)
    {
        operators.push_back(clients[fd]);
    }
}
void Channel::sendMessage(std::string message)
{
    for (unsigned long i = 0; i < users.size(); i++)
    {
        if (send(users[i].fd, message.c_str(), message.length(), 0) == -1)
        {
            perror("send");
        }
    }
}
bool Channel::getLimits()
{
    return limits;
}
void Channel::sendMessagenick(std::string message, int fd)
{
    for (unsigned long i = 0; i < users.size(); i++)
    {
        if (users[i].fd != fd)
        {
            if (send(users[i].fd, message.c_str(), message.length(), 0) == -1)
            {
                perror("send");
            }
        }
    }
}
// {
//     for (unsigned long i = 0; i < users.size(); i++)
//     {
//         if (send(users[i].fd, message.c_str(), message.length(), 0) == -1)
//         {
//             perror("send");
//         }
//     }
// }
std::vector<std::string> Channel::getMode()
{
    return mode;
}
void Channel::setMaxUsers(size_t maxUsers)
{
    this->maxUsers = maxUsers;
}
void Channel::notifieusers(Channel channel,std::string nickname, std::string message, std::string hostname)
{
    for (unsigned long i = 0; i < channel.getUsers().size(); i++)
    {
        //need to modifier the message
        std::string msg = ":" + nickname + "!" + channel.getUsers()[i].nickname + "@" + hostname + " " + channel.getName() + " :" + message + "\n";
        if (send(channel.getUsers()[i].fd, msg.c_str(), msg.length(), 0) == -1)
        {
            perror("send");
        }
    }
}
client *Channel::getUser(int fd)
{
    for (unsigned long i = 0; i < users.size(); i++)
    {
        if (users[i].fd == fd)
        {
            return &users[i];
        }
    }
    return NULL;
}
void Channel::setFd(int fd)
{
    fds.push_back(fd);
}
int Channel::userinvite(int fd)
{
    for (unsigned long i = 0; i < fds.size(); i++)
    {
        if (fds[i] == fd)
        {
            return 1;
        }
    }
    return 0;
}
void Channel::setInviteOnly(int inviteOnly)
{
    this->inviteOnly = inviteOnly;
}
void Channel::removefd( int fd)
{
    for (unsigned long i = 0; i < fds.size(); i++)
    {
        if (fds[i] == fd)
        {
            fds.erase(fds.begin() + i);
            return;
        }
    }
}
void Channel::setMode(std::string mode)
{
    // std::cout << "mode: " << mode << std::endl;
    for (unsigned long i = 0; i < this->mode.size(); i++)
    {
        if (this->mode[i] == mode)
        {
            return;
        }
        else if (this->mode[i] == "+" + mode.substr(1))
        {
            this->mode.erase(this->mode.begin() + i);
            this->mode.push_back(mode);
            return;
        }
        else if (this->mode[i] == "-" + mode.substr(1))
        {
            this->mode.erase(this->mode.begin() + i);
            this->mode.push_back(mode);
            return;
        }
    }
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
void Channel::setLimits(bool limits)
{
    this->limits = limits;
}
int Channel::addUser(cli client, int o)
{
    for (unsigned long i = 0; i < users.size(); i++)
    {
        if (users[i].fd == client.fd)
        {
            // std::cout << "User already in channel" << std::endl;
            return 1;
        }
    }
    std::cout << "users.size(): " << limits << std::endl;
    if (limits)
    {
        if (users.size() >= maxUsers)
        {
            // std::cout << "Channel is full" << std::endl;
            return 2;
        }
    }
    // if (users.size() >= maxUsers)
    // {
    //     // std::cout << "Channel is full" << std::endl;
    //     return 2;
    // }
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
    for (unsigned long i = 0; i < operators.size(); i++)
    {
        if (operators[i].nickname == user)
        {
            operators.erase(operators.begin() + i);
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


void Channel::clearUsers()
{
    users.clear();
}

Channel::~Channel()
{
    users.clear();
}

void Channel::createChannel(std::string name)
{
    this->name = name;
}

void Channel::addOperator(cli user)
{
    operators.push_back(user);
}
void Channel::notifyUserJoin(std::string user, std::string hostname, int o)
{
    for (unsigned long i = 0; i < users.size(); i++)
    {
        std::string msg = ":" + user + "!~" + users[i].user_name + "@" + hostname + " JOIN " + name + "\n";
        if (send(users[i].fd, msg.c_str(), msg.length(), 0) == -1)
        {
            perror("send");
        }
        if (users[i].nickname == user)
        {
    
        if (o)
            msg = ":" + hostname + " 353 " + user + " @ " + name + " :" + "@" + user + "\n";
        else
        {
            msg = ":" + hostname + " 353 " + user + " = " + name + " :";
            for (unsigned long i = 0; i < users.size(); i++)
            {
                // unsigned long k = 0;
                for (unsigned long j = 0; j < operators.size(); j++)
                {
                    if (users[i].nickname != operators[j].nickname.substr(1))
                    {
                        // std::cout << "operators[j].nickname: " << operators[j].nickname.substr(1) << std::endl;
                        msg += users[i].nickname + " ";
                    }
                }
             }
             for (unsigned long j = 0; j < operators.size(); j++)
                {
                    msg += operators[j].nickname + " ";
                }
            msg += "\n";
        }
        if (send(users[i].fd, msg.c_str(), msg.length(), 0) == -1)
        {
            perror("send");
        }
        // msg = ":" + hostname + " MODE " + name;
        // for (unsigned long j = 0; j < mode.size(); j++)
        // {
        //     msg += mode[j];
        //     // if (send(users[i].fd, msg.c_str(), msg.length(), 0) == -1)
        //     // {
        //     //     perror("send");
        //     // }
        // }
        // msg += "\n";
        // if (send(users[i].fd, msg.c_str(), msg.length(), 0) == -1)
        // {
        //     perror("send");
        // }
        msg = ":" + hostname + " 366 " + user + " " + name + " :End of /NAMES list\n";
        if (send(users[i].fd, msg.c_str(), msg.length(), 0) == -1)
        {
            perror("send");
        }
        }
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

int Channel::checkModeexist(Channel channel, std::string mode)
{
    for (unsigned long i = 0; i < channel.getMode().size(); i++)
    {
        // std::cout << "channel.getMode()[i]: " << channel.getMode()[i] << std::endl;
        if (channel.getMode()[i] == mode)
        {
            return 1;
        }
    }
    return 0;
}