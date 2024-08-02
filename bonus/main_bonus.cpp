/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saboulal <saboulal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 09:09:53 by saboulal          #+#    #+#             */
/*   Updated: 2024/07/18 17:41:41 by saboulal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"srcs/server_bonus.hpp"
int main(int argc, char **argv)
{
// int port_num;
   ser_ ser;
    int pass_bonus;
    //  struct sockaddr_in addr;
     std::vector<std::string> buffer_stor;
    //  struct pollfd fdpoll;
     cli_ cli_client;
    //    client cli;
    //  char buffer[1024];
     std::vector<std::string>split;
     std::string msg;
     std::string cmd;
     char hostname[1024] ;
     hostname [1023] = '\0';
     //returns the standard host name for the current machine
     gethostname(hostname, 1023);
     ser.hostname = hostname;
     

    //  int flag = 0;
    // int a = 0;
     
   
    
    //  and 65535 are reserved ports
   if(argc != 3)
    {
        std::cout <<"Error Try Again"<<std::endl;
        exit (0);
    }
    int socket_fd = socket(AF_INET,SOCK_STREAM,0);
    int port_bonus = -1;
    
    if (std::atoi(argv[1]) < 1024 || std::atoi(argv[1]) > 65535)
    {
        std::cout <<"Error Failed"<<std::endl;
        return(1);
    } 
    port_bonus = std::atoi(argv[1]);
    pass_bonus = std::atoi(argv[2]);
    if (pass_bonus == -1)
    {
        std::cout <<"Password is Empty!"<<std::endl;
        return(1);
    }
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

     if (connect(socket_fd,(struct sockaddr *)&cr_server,sizeof(cr_server) == -1))
     {
        std::cout <<"failed errortt"<<std::endl;
        return(1);
     }
     //authentication
     while(1)
     {
        
     }

}
