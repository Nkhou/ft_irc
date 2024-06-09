#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <iostream>
#include <string.h>
#include <vector>
class Command {
    private:
        std::string command;
        std::vector<std::string> args;
    public:  
        Command(){};
        Command(Command const &command);
        void ParceCommand(std::string command);
        ~Command(){};
        std::string getCommand() { return command; }
        std::vector<std::string> getArgs() { return args; }
        void setCommand(std::string command) { this->command = command; }
        void setArgs(std::vector<std::string> args) { this->args = args; }
        bool isCommand(std::string command);
        bool isCommand(std::vector<std::string> commands);
        void execute();
        void JoinCommand();
        // void execute();
};

#endif