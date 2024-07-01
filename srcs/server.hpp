/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saboulal <saboulal@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 12:53:03 by saboulal          #+#    #+#             */
/*   Updated: 2024/07/01 17:10:46 by saboulal         ###   ########.fr       */
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
#include <climits>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iterator>
#define GREEN "\033[0;32m"
typedef struct server
{
    int ser_fd; //server socket file descriptor 
    std::vector<struct pollfd>fds; // vector for pollfds
    std::string password_arg;
    std::vector<struct client>clients; // vector of client 
    void setPassword(std::string password){this->password_arg=password;};
    
  
}ser;

typedef struct client
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
}cli;

#endif