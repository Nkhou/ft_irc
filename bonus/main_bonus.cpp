/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saboulal <saboulal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 09:09:53 by saboulal          #+#    #+#             */
/*   Updated: 2024/08/03 12:31:53 by saboulal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"srcs/server_bonus.hpp"
#define GREEN "\033[0;32m"
int main(int argc, char **argv)
{
// int port_num;
   ser_ ser;
   std::string pass_bonus;
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
 
    if(socket_fd == -1)
    {
        std::cout <<"Error Failed"<<std::endl;
        return(1);
    }
    if (std::atoi(argv[1]) < 1024 || std::atoi(argv[1]) > 65535)
    {
        std::cout <<"Error Failed"<<std::endl;
        return(1);
    } 
    
    int port_bonus = std::atoi(argv[1]);
    pass_bonus = argv[2];
    if (pass_bonus.empty())
    {
        std::cout <<"Password is Empty!"<<std::endl;
        return(1);
    }
    struct sockaddr_in cr_server;
    std::memset(&cr_server,0,sizeof(cr_server));
    cr_server.sin_family = AF_INET; // specifies IPV4(protocol for communication)
     cr_server.sin_port = htons(port_bonus); // sets the port ,function htons:converting it to network byte order 
     cr_server.sin_addr.s_addr = INADDR_ANY; 

     //connection to server

     if (connect(socket_fd,(struct sockaddr *)&cr_server,sizeof(cr_server) )== -1)
     {
        std::cout <<"failed Error"<<std::endl;
        return(1);
     }
     //authentication
    std::string pass = "PASS " + pass_bonus + "\r\n";
    if (send(socket_fd, pass.c_str(), pass.length(), 0) == -1)
        return (std::cerr << "Error: send failed" << std::endl, 1);
     
    sleep(1);
    std::string user = "USER UsernameBot 0 * RealnameBot\r\n";
    if (send(socket_fd, user.c_str(), user.length(), 0) == -1)
        return (std::cerr << "Error: send failed" << std::endl, 1);

    sleep(1);

    // srand((unsigned int)time(0));
    // std::string n = std::to_string(rand() % 2);
    std::string nick = "NICK bot\r\n";
    if (send(socket_fd, nick.c_str(), nick.length(), 0) == -1)
        return (std::cerr << "Error: send failed" << std::endl, 1);
        char buffer[1024];  
        std::memset(buffer,0,1024);
         ssize_t bytes = recv(socket_fd,buffer,1024,0);
        if (bytes == -1)
        {
            std::cout <<"Error Failed"<<std::endl;
            return(1);
        }
        std::cout << buffer << std::endl;
     
     while(1)
     {
        ssize_t bytes = recv(socket_fd,buffer,1024,0);
        if (bytes == -1)
        {
            std::cout <<"Error Failed"<<std::endl;
            return(1);
        }
        
     }
}
