#ifndef COMMAND_HPP
#define COMMAND_HPP

// #include <string>
// #include <iostream>
// #include <string.h>
// #include <vector>
#include"../srcs/server.hpp"

class Command {
    private:
        // std::string command;
        int fd;
        std::vector<std::string> args;
        std::vector<std::string> keys;
        std::string message;
    public:  
        Command(){};
        void ParceCommand(std::vector<std::string> command, int fd, std::string hostname);
        ~Command(){};
        // std::string getCommand() { return command; }
        std::vector<std::string> getArgs() { return args; }
        // void setCommand(std::string command) { this->command = command; }
        void setArgs(std::vector<std::string> args) { this->args = args; }
        bool isCommand(std::string command);
        bool isCommand(std::vector<std::string> commands);
        void executecmd( server *ser);
        void JoinCommand( server *ser);
        void execute(); 
        void execCommand( server *ser);
        void addusertoChannel(server *server, std::string channel, int o);
        void KickCommand(server *ser);
        void   ParceCommandkick(std::vector<std::string> command, int fd, std::vector<Channel>channel, std::string hostname);
        void ModeCommand(server *ser);
        // void ParceModeCommand(std::vector <std::string> splited, int client_fd);
        void ParceTopic(std::vector <std::string> splited, int client_fd);
        void PrintTopic(int fd);
        void TopicCommand(server *ser);
        void ParceInvite(std::vector <std::string> splited, int client_fd);
        void InviteCommand(server *ser);
        void ParceModeCommand(std::vector <std::string> splited, int client_fd);
        void ParcePrivmsg(std::vector <std::string> splited, int client_fd);
        void PrivmsgCommand(server *ser);
        client *getClientByFd(server *ser, int fd);
};

#endif