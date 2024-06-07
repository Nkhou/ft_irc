#include "command.hpp"

int main() {
    Command command;
    command.ParceCommand("JOIN #foo,#bar fubar,foobar");
    command.execute();
    // std::cout << command.getCommand() << std::endl;
    // std::vector<std::string> args = command.getArgs();
    // for (int i = 0; i < args.size(); i++) {
    //     std::cout << args[i] << std::endl;
    // }
    return 0;
}