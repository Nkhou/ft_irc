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
    return strcmp(this->command.c_str(), command.c_str());
}

void Command::JoinCommand() {
    std::cout << "JOIN" << std::endl;
    

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