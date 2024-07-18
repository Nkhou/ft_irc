/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkh <nkh@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 12:53:03 by saboulal          #+#    #+#             */
/*   Updated: 2024/07/16 11:36:46 by nkh              ###   ########.fr       */
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
#include "../cmd/channel.hpp"
#define GREEN "\033[0;32m"

typedef struct server
{
    int ser_fd; //server socket file descriptor 
    std::vector<struct pollfd>fds; // vector for pollfds
    std::string password_arg;
    std::vector<struct client>clients; // vector of client 
    std::string hostname;
    std::vector<Channel>channels;
    std::vector<std::string>splited;
    int client_fd;
 
  
}ser;


std::string msg_err(std::string nick,std::string hostname);
std::string msg_erroneusnickname(std::string nick,std::string hostname);
std::string msg_welcome(std::string nick,std::string hostname);
std::string msg_nonicknamegiven(std::string hostname);
std::string msg_notregistered(std::string nick,std::string hostname);
std::string msg_nicknameinuse(std::string nick,std::string hostname);
std::string ChannelExist(std::string channel, std::string hostname);
std::string NotOPRT(std::string nick, std::string hostname);
std::string msg_errpriv(std::string nick, std::string hostname);
std::string sendMessage(std::string nick, std::string hostname, std::string message);
std::string RPL_NOTOPIC(std::string nick, std::string hostname);
std::string ERR_BADCHANNELKEY(std::string nick, std::string hostname);
std::string ERR_INVITEONLYCHAN(std::string nick, std::string hostname);
std::string ERR_NOSUCHCHANNEL(std::string nick, std::string hostname);
std::string msg_errsend(std::string nick, std::string hostname);
std::string NotMode(std::string nick, std::string hostname);

#endif