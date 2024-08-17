#ifndef CLIENT_HPP
# define CLIENT_HPP
#include <iostream>
// #include <vector> //-> for vector
#include <sys/socket.h> //-> for socket()
#include <sys/types.h> //-> for socket()
#include <netinet/in.h> //-> for sockaddr_in
#include <fcntl.h> //-> for fcntl()
#include <unistd.h> //-> for close()
#include <arpa/inet.h> //-> for inet_ntoa()
#include <poll.h> //-> for poll()
#include <csignal> //-> for signal()
#include <cstdlib>
#include <cstdio>
#define ERR_ERRONEUSNICKNAME_CODE "432"
#define ERR_ERRONEUSNICKNAME ":Erroneous Nickname"
#define ERR_NICKNAMEINUSE_CODE "433"
#define ERR_NICKNAMEINUSE_MSG "Nickname is already in use"

#include <cstring>

#define GREEN "\033[0;32m"

typedef struct client
{
    int fd; //file descriptor clients
    std::string ip_addr; // ip address for clients
    std::string user_name;
    std::string hostname;
    std::string servername;
    std::string realname;
    std::string nickname;
    std::string ip_adddress;
    std::string buff;
    std::string cmd; 
    
    bool password;
    bool flag;
    bool flag_cmd;
    
}cli;

#endif