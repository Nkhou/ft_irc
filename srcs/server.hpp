/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saboulal <saboulal@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 12:53:03 by saboulal          #+#    #+#             */
/*   Updated: 2024/06/23 04:50:04 by saboulal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
#include <iostream>
#include <vector> //-> for vector
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
#include <cstring>
#define GREEN "\033[0;32m"
/* For Clients */
// class Client
// {
//     private:
//          int fd; //file descriptor clients
//          std::string ip_addr; // ip address for clients
//          std::string user_name;
//          std::string hostname;
//          std::string servername;
//          std::string realname;
//          std::string nickname;
//          std::string buff;
//          std::string cmd; 
//          bool password;
//     public:
//         Client(){};
//         void setIP(std::string ip){this->ip_addr = ip;};
//         int  setFD(int fd){this->fd = fd; return fd;};
//         void setUserName(std::string user_name){this->user_name = user_name;};
//         void setHostName(std::string hostname){this->hostname = hostname;};
//         void setServerName(std::string servername){this->servername = servername;};
//         void setRealName(std::string realname){this->realname = realname;};
//         void setNickName(std::string nickname){this->nickname = nickname;};
//         bool setPassword_bool(bool val){this->password = val; return(0);};
//         void setBuffer(std::string msg_rec){buff += msg_rec;};
//         std::string getBuffer(){return buff;};
// };
// /*For Server*/
// class Server
// {
//     private:
//        int ser.ser_fd; //server socket file descriptor 
//        static bool Signal;//for signals
//        std::vector<Client> clients; // vector of client 
//        std::vector<struct pollfd>fds; // vector for pollfds
//        std::string password_arg; 
     
//     public:
//         Server(); //Default constructer 
//         void Server_init(int port_num); // initialisation du server
//         void Server_creation(); // Creation du server
//         void AcceptNewclient(); // accept new client
//         void ReceiveNewData(int fd); // receive new data
//         void setPassword(std::string password){this->password_arg=password;};
       
        
    
// };

struct server
{
    int ser_fd; //server socket file descriptor 
    std::vector<struct pollfd>fds; // vector for pollfds
    std::string password_arg;
    std::vector<struct client>clients; // vector of client 
    void setPassword(std::string password){this->password_arg=password;};
    
    // void setUserName(std::string user){cli.user_name = user;};
    // void setHostName(std::string host){cli.hostname = host;};
    // void setServerName(std::string server){cli.servername = server;};
    // void setRealName(std::string real){cli.realname = real;};
    // void Server_init(int port_num); // initialisation du server
    // void Server_creation(); // Creation du server
    // void AcceptNewclient(); // accept new client
    // void ReceiveNewData(int fd); // receive new data
    // void SendData(int fd,std::string msg); // send data
    // void SendDataAll(std::string msg); // send data to all clients
    // void SendDataAllExcept(int fd,std::string msg); // send data to all clients except one
    // void SendDataAllExcept(std::string nickname,std::string msg); // send data to all clients except one
    // void SendDataAllExcept(std::string nickname,std::string msg,int fd); // send data to all clients except one
    // void SendDataAllExcept(std::string nickname,std::string msg,std::string nickname2); // send data to all clients except one
    // void SendDataAllExcept(std::string nickname,std::string msg,std::string nickname2,int fd); // send data to all clients except one
    // void SendDataAllExcept(std::string nickname,std::string msg,std::string nickname2,std::string nickname3); // send data to all clients except one
    // void SendDataAllExcept(std::string nickname,std::string msg,std::string nickname2,std::string nickname3,int fd); // send data to all clients except one
    // void SendDataAllExcept(std::string nickname,std::string msg,std::string nickname2,std::string nickname3,std::string nickname4); // send data to all clients except one
    // void SendDataAllExcept(std::string nickname,std::string msg,std::string nickname2,std::string nickname3,std::string nickname4,int fd); // send data to all clients except one
    // void SendDataAllExcept(std::string nickname,std::string msg,std::string nickname2,std::string nickname3,std::string nickname4,std::string nickname5); // send data to all clients except one
    // void SendDataAllExcept(std::string nickname,std::string msg,std::string nickname2,std::string nickname3,std::string nickname
    
}ser;

struct client
{
    int fd; //file descriptor clients
    std::string ip_addr; // ip address for clients
    std::string user_name;
    std::string hostname;
    std::string servername;
    std::string realname;
    std::string nickname;
    std::string buff;
    std::string cmd; 
    bool password;
    void setPassword_bool(bool val){password = val;}
}cli;

#endif