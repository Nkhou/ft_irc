/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saboulal <saboulal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 12:53:03 by saboulal          #+#    #+#             */
/*   Updated: 2024/06/04 11:50:11 by saboulal         ###   ########.fr       */
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
/* For Clients */
class Client
{
    private:
         int fd; //file descriptor clients
         std::string ip_addr; // ip address for clients
    public:
        Client();
        void setIP(std::string ip);
        int setFD(int fd);
};
/*For Server*/
class Server
{
    private:
       int ser_fd; //server socket file descriptor 
       static bool Signal;//for signals
       std::vector<Client> clients; // vector of client 
       std::vector<struct pollfd>fds; // vector for pollfds
    public:
        Server(); //Default constructer 
        void Server_init(int port_num); // initialisation du server
        void Server_creation(); // Creation du server
        void AcceptNewclient(); // accept new client
        void ReceiveNewData(int fd); // receive new data
        
    
};


#endif