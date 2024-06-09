#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <vector>
#include <string>
class Channel
{
    private:
        std::vector<std::string> users;
        std::string name;
    public:
        Channel(std::string name);
        void addUser(std::string user);
        void removeUser(std::string user);
        std::string getName();
        std::vector<std::string> getUsers();
        void printUsers();
        bool userExists(std::string user);
        void clearUsers();
        void sendMessage(std::string message);
        void joinChannel(std::string user);
        void createChannel(std::string name);
        void notifyUserJoin(std::string user);
        ~Channel();
}; 
#endif