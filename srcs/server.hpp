/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saboulal <saboulal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 12:53:03 by saboulal          #+#    #+#             */
/*   Updated: 2024/06/13 23:18:26 by saboulal         ###   ########.fr       */
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
#define GREEN "\033[0;32m"
/* For Clients */
class Client
{
    private:
         int fd; //file descriptor clients
         std::string ip_addr; // ip address for clients
         std::string user_name;
         std::string hostname;
         std::string servername;
         std::string realname;
         std::string nickname;
        // bool password;
    public:
        Client(){};
        void setIP(std::string ip){this->ip_addr = ip;};
        int  setFD(int fd){this->fd = fd; return fd;};
        void setUserName(std::string user_name){this->user_name = user_name;};
        void setHostName(std::string hostname){this->hostname = hostname;};
        void setServerName(std::string servername){this->servername = servername;};
        void setRealName(std::string realname){this->realname = realname;};
        void setNickName(std::string nickname){this->nickname = nickname;};
        // void setPassword(std::string password){this->password = password;};
};
/*For Server*/
class Server
{
    private:
       int ser_fd; //server socket file descriptor 
       static bool Signal;//for signals
       std::vector<Client> clients; // vector of client 
       std::vector<struct pollfd>fds; // vector for pollfds
       std::string password_arg;
    public:
        Server(); //Default constructer 
        void Server_init(int port_num); // initialisation du server
        void Server_creation(); // Creation du server
        void AcceptNewclient(); // accept new client
        void ReceiveNewData(int fd); // receive new data
        
    
};


#endif