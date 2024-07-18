#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <vector>
#include <string>
#include "../srcs/client.hpp"
// #include "../srcs/server.hpp"
class Channel
{
    private:
        std::vector<client> users;
        std::vector<client> operators;
        std::string name;
        std::string topic;
        std::vector<std::string> mode;
        std::string key;
        int userCount;
        int inviteOnly;
        int operatorCount;
        long unsigned int maxUsers;
    public:
        Channel(std::string name);
        Channel(std::string name, int o, int fd, std::vector<client> clients);
        int addUser(cli client, int o);
        void removeUser(std::string user);
        std::string getName();
        int getUserCount();
        int getOperatorCount();
        // int getMaxUsers();
        int getChannelSize();
        void setuserCount(int userCount);
        void setoperatorCount(int operatorCount);
        std::vector<cli> getOperators();
        void addOperator(cli user);
        void removeOperator(std::string user);
        std::vector<cli> getUsers();
        std::vector<std::string> getMode();
        std::string getTopic();
        void setTopic(std::string topic);
        std::string getKey();
        void setKey(std::string key);
        void setMode(std::string mode);
        void printUsers();
        void clearUsers();
        void sendMessage(std::string message);
        bool userExists(cli *client, int fd);
        void createChannel(std::string name);
        void notifyUserJoin(std::string user, std::string hostname, int o);
        void setMaxUsers(int maxUsers);
        long unsigned int getMaxUsers();
        void PrintTopic(int fd);
        void PrintMode(int fd);
        void setInviteOnly(int inviteOnly);
        // void executecmdJOIN(server *server);
        ~Channel();
}; 
#endif