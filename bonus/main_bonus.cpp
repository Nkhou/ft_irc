/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saboulal <saboulal@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 09:09:53 by saboulal          #+#    #+#             */
/*   Updated: 2024/07/13 23:51:58 by saboulal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../srcs/server.hpp"

int main(int argc, char **argv)
{
    if(argc != 3)
    {
        std::cout <<"Error Try Again"<<std::endl;
        exit (0);
    }
    int socket_fd = socket(AF_INET,SOCK_STREAM,0);
    int port_bonus;

    if (std::atoi(argv[1]) < 1024 || std::atoi(argv[1]) > 65535)
    {
        std::cout <<"Error Failed"<<std::endl;
        return(1);
    } 
    port_bonus = std::atoi(argv[2]);
    if(socket_fd == -1)
    {
        std::cout <<"Error Failed"<<std::endl;
        return(1);
    }
    struct sockaddr_in cr_server;
    std::memset(&cr_server,0,sizeof(cr_server));
    cr_server.sin_family = AF_INET; // specifies IPV4(protocol for communication)
     cr_server.sin_port = htons(port_bonus); // sets the port ,function htons:converting it to network byte order 
     cr_server.sin_addr.s_addr = INADDR_ANY; 

     //connection to server

     if(connect(socket_fd,(struct sockaddr *)&cr_server,sizeof(cr_server) == -1))
     {
        std::cout <<"failed errortt"<<std::endl;
        return(1);
     }
     while(1){}

}