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
#include "bot.hpp"
#define GREEN "\033[0;32m"

void  splited(std::string str, std::vector<std::string> *split)
{
    // std::vector<std::string> split;
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] == ':' && i < str.length() - 1 && str[i + 1] == ' ')
        {
            i ++;
            while (i < str.length() && str[i] == ' ')
            {
                i++;
            }
            if (i < str.length())
                str = str.substr(i, str.length());
            i = 0;
        }
        if (i < str.length() && str[i] == ' ')
        {
            split->push_back(str.substr(0, i));

            str = str.substr(i + 1, str.length());
            i = 0;
        }
    }
    int i = 0;
    // std::cout << "str: " << str << std::endl;
    while (i < str.length() && (str[i] != ' ' && str[i] != '\n' && str[i] != '\r'))
    {
        i++;
    }
    if (i != 0)
        str = str.substr(0, i);
    // std::cout << "str: " << str << std::endl;
    split->push_back(str);
}
std::ostream& operator<< (std::ostream& os, const std::vector<std::string>& v) {
    for (int i = 0; i < v.size(); i++)
    {
        os << v[i] << std::endl;
    }
    return os;
}

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

    srand((unsigned int)time(0));
    std::string n = std::to_string(rand() / 10000);
    std::string nick = "NICK bot"+n+"\r\n";
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
        std::cout <<buffer << std::endl;
        std::memset(buffer,0,1024);
     Bot bot = Bot("bot"+n);
     if (bot.getJocks().empty())
     {
         bot.addjocks();
     }
     while(1)
     {
        ssize_t bytes = recv(socket_fd,buffer,1024,0);
        if (bytes == -1)
        {
            std::cout <<"Error Failed"<<std::endl;
            return(1);
        }
        std::string  str = buffer;
        std::cout << str << std::endl;
        // std::cout << GREEN << str << std::endl;
        std::vector<std::string> split ;
         splited(str, &split);
        // for (int i = 0; i < split.size(); i++)
        // {
        //     std::cout << "i  = " << i << " " << split[i] << std::endl;
        // }
        
        bot.parcingBuffer(split);
        // for (int i = 0; i < bot.getMessage().size(); i++)
        // {
        //     std::cout <<"i  = " <<i <<" "<<bot.getMessage()[i] << std::endl;
        // }
        std::vector<std::string> msg = bot.getMessage();
        // try 
        // {
        //     bot.execbot(socket_fd);
        // }
        // catch(const std::exception& e)
        // {
        //     std::cerr << e.what() << '\n';
        // }
        bot.execbot(socket_fd);
        std::memset(static_cast<void*>(const_cast<char*>(str.c_str())),0,str.length());
        msg.clear();
        split.clear();
     }
}
