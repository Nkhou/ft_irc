#include "command.hpp"

Command::Command(Command const &command) {
    this->command = command.command;
    this->args = command.args;
}

void Command::ParceCommand(std::string command) {

    this->command = command;
    std::string arg;
    for (int i = 0; i < command.size(); i++) {
        if (command[i] == ' ') {
            args.push_back(arg);
            arg = "";
        } else {
            arg += command[i];
        }
    }
    args.push_back(arg);
}

bool Command::isCommand(std::string command) {
    return this->command == command;
}

bool Command::isCommand(std::vector<std::string> commands) {
    int i = 0;
    std::cout << commands[i] << std::endl;
    if (strcmp(commands[i].c_str(),  "JOIN") || strcmp(commands[i].c_str(),  "INVITE") || strcmp(commands[i].c_str(),  "PART") || strcmp(commands[i].c_str(),  "MODE")
    || strcmp(commands[i].c_str(),  "TOPIC") || strcmp(commands[i].c_str(),  "NAMES") || strcmp(commands[i].c_str(),  "LIST")
    || strcmp(commands[i].c_str(),  "QUIT")
    || strcmp(commands[i].c_str(),  "NICK") || strcmp(commands[i].c_str(),  "USER") || strcmp(commands[i].c_str(),  "OPER")
    || strcmp(commands[i].c_str(),  "PASS") || strcmp(commands[i].c_str(),  "PING")
    || strcmp(commands[i].c_str(),  "PRIVMSG") || strcmp(commands[i].c_str(),  "NOTICE") 
    || strcmp(commands[i].c_str(),  "KILL") || strcmp(commands[i].c_str(),  "KICK" ) ) {
        return true;
    }
    return false;
}
void Command::execute() {
    if (isCommand("JOIN")) {
        std::cout << "JOIN" << std::endl;
    } else if (isCommand("INVIT")) {
        std::cout << "INVIT" << std::endl;
    }
    else if (isCommand("PART")) {
        std::cout << "PART" << std::endl;
    }
    else if (isCommand("MODE")) {
        std::cout << "MODE" << std::endl;
    }
    else if (isCommand("TOPIC")) {
        std::cout << "TOPIC" << std::endl;
    }
    else if (isCommand("NAMES")) {
        std::cout << "NAMES" << std::endl;
    }
    else if (isCommand("LIST")) {
        std::cout << "LIST" << std::endl;
    }
    else if (isCommand("QUIT")) {
        std::cout << "QUIT" << std::endl;
    }
    else if (isCommand("NICK")) {
        std::cout << "NICK" << std::endl;
    }
    else if (isCommand("USER")) {
        std::cout << "USER" << std::endl;
    }
    else if (isCommand("OPER")) {
        std::cout << "OPER" << std::endl;
    }
    else if (isCommand("PASS")) {
        std::cout << "PASS" << std::endl;
    }
    else if (isCommand("PING")) {
        std::cout << "PING" << std::endl;
    }
    else if (isCommand("PRIVMSG")) {
        std::cout << "PRIVMSG" << std::endl;
    }
    else if (isCommand("NOTICE")) {
        std::cout << "NOTICE" << std::endl;
    }
    else if (isCommand("KILL")) {
        std::cout << "KILL" << std::endl;
    }
    else if (isCommand("KICK")) {
        std::cout << "KICK" << std::endl;
    }
    else {
        std::cout << "Command not found" << std::endl;
    }
}