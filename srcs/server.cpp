/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saboulal <saboulal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 13:28:11 by saboulal          #+#    #+#             */
/*   Updated: 2024/06/13 23:50:20 by saboulal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

Server::Server()
{
    this->ser_fd = -1;
}

void Server::Server_init(int port_num)
{
   
    //creation du Server
     struct sockaddr_in addr;
     struct pollfd fdpoll;
       Client cli;
     char buffer[1024]={0};
     std::vector<std::string>split;
     std::string msg;
     std::string cmd;
   
    int pe = 1;
     addr.sin_family = AF_INET; // specifies IPV4(protocol for communication)
     addr.sin_port = htons(port_num); // sets the port ,function htons:converting it to network byte order 
     addr.sin_addr.s_addr = INADDR_ANY; // sets the address to any interface on the server (bind to any ip address on the server)

     ser_fd = socket(AF_INET,SOCK_STREAM,0);// create socket server
     // not finich here some condition to check if the socket is created or not
     
     if (ser_fd < 0)
    {   
       std::cout << "Failed Socket Try Again"<<std::endl;
       exit(0);
    }
    if (setsockopt(ser_fd, SOL_SOCKET, SO_REUSEADDR, &pe, sizeof(pe)) == -1) //-> set the socket option (SO_REUSEADDR) to reuse the address
        throw(std::runtime_error("faild to set option (SO_REUSEADDR) on socket"));
	if (fcntl(ser_fd, F_SETFL, O_NONBLOCK) == -1) //-> set the socket option (O_NONBLOCK) for non-blocking socket
		throw(std::runtime_error("faild to set option (O_NONBLOCK) on socket"));
    if (bind(ser_fd,(struct sockaddr*)&addr,sizeof(addr)) < 0) //bind the socket to the address and port number
    {
        std::cout << "Failed Bind Try Again"<<std::endl;
        exit(0);
    }
    if (listen(ser_fd,5) < 0) //listen for incoming connections
    {
        std::cout << "Failed Listen Try Again"<<std::endl;
        exit(0);
    }
    //polling
    fdpoll.fd = ser_fd;
    fdpoll.events = POLLIN;
    fds.push_back(fdpoll);
    std::cout << "Server is Ready to Accept a Connection...."<<std::endl;
    // infinite loop
    while(1) 
    {
        //now using function poll to check if there is any incoming connection
        if(poll(fds.data(),fds.size(),-1) < 0)
        {
            std::cout << "Failed Poll Try Again"<<std::endl;
            exit(0);
        }
        size_t i = 0;
        while(i < fds.size())
        {
            if(fds[i].revents & POLLIN)
            {
                if(fds[i].fd == ser_fd)
                {
                 
                   struct sockaddr_in addr;
                   struct pollfd fdpoll;
                   socklen_t len = sizeof(addr);
               
                   int cli_fd = accept(ser_fd,(struct sockaddr*)&addr,&len); //accept the incoming connection
                   if(cli_fd < 0)
                   {
                       std::cout << "Failed Accept Try Again"<<std::endl;
                       exit(0);
                   }
                   if (fcntl(cli_fd, F_SETFL, O_NONBLOCK) == -1) //-> set the socket option (O_NONBLOCK) for non-blocking socket
                       throw(std::runtime_error("faild to set option (O_NONBLOCK) on socket"));
                   
                   fdpoll.fd = cli_fd;
                   fdpoll.events = POLLIN;
                   fdpoll.revents = 0;
                   
                   cli.setFD(cli_fd);
                   cli.setIP(inet_ntoa(addr.sin_addr));
                   cli.setUserName("");
                   cli.setHostName("");
                   cli.setServerName("");
                   cli.setRealName("");
                   cli.setNickName("");   
                   clients.push_back(cli);
                   fds.push_back(fdpoll);
                   std::cout << GREEN << "<<< New Client Connected >>> " << cli_fd << std::endl;
                }
                else
                {
                    memset(buffer,0,sizeof(buffer));
                    ssize_t size = recv(fds[i].fd,buffer,sizeof(buffer),0); //receive the data from the client
                    if(size <= 0)
                    {  
                      std::cout << "client Disconnected"<<fds[i].fd<<std::endl;
                      close(fds[i].fd);
                    }

                    else
                    {
                        buffer[size] = '\0';
                        if(split[0] == "PASS")
                        {

                        }
                        if(cli.setNickName("") || cli.setUserName(""))
                        {
                            if(split[0] == "NICK")
                            {
                                printf("a\n");
                            }
                            else if(split[0] == "USER")
                            {
                                printf("b\n");   
                            }
                        }
                        
                    }
                  
                }
            }
            i++;
        }
         
    }    
} 


// void Server::AcceptNewclient()
// {
//     Client cli;
//     struct sockaddr_in addr;
//     struct pollfd fdpoll;
//     socklen_t len = sizeof(addr);

//     int cli_fd = accept(ser_fd,(struct sockaddr*)&addr,&len); //accept the incoming connection
//     if(cli_fd < 0)
//     {
//         std::cout << "Failed Accept Try Again"<<std::endl;
//         exit(0);
//     }
//     if (fcntl(cli_fd, F_SETFL, O_NONBLOCK) == -1) //-> set the socket option (O_NONBLOCK) for non-blocking socket
//         throw(std::runtime_error("faild to set option (O_NONBLOCK) on socket"));
//     fdpoll.fd = cli_fd;
//     fdpoll.events = POLLIN;
//     fdpoll.revents = 0;
    
//     cli.setFD(cli_fd);
//     cli.setIP(inet_ntoa(addr.sin_addr));
//     cli.setUserName("");
//     cli.setHostName("");
//     cli.setServerName("");
//     cli.setRealName("");
//     cli.setNickName("");
    
//     clients.push_back(cli);
//     fds.push_back(fdpoll);
//     std::cout << "<<< New Client Connected >>> " << cli_fd<< std::endl;
// }



// void Server::ReceiveNewData(int cli_fd)
// {
//     char buffer[1024]; 
//     memset(buffer,0,sizeof(buffer));
//     ssize_t size = recv(cli_fd,buffer,sizeof(buffer),0); //receive the data from the client
//     if(size <= 0)
//       {  
//          printf("1its here/n");
//         std::cout << "client Disconnected"<<cli_fd<<std::endl;
//         close(cli_fd);
//       }
//     else 
//     {
//          printf("2its here/n");
//         buffer[size] = '\0';
//         std::cout<< "Client< "<< cli_fd << ">Data: "<< buffer;
        
//     }
//     if(buffer[0] == 'Q' && buffer[1] == 'U' && buffer[2] == 'I' && buffer[3] == 'T')
//     {
//         close(cli_fd);
//         std::cout << "Client Disconnected"<<cli_fd<<std::endl;
       
//     }
//     else
//     {
//         printf("3its here/n");
//         std::string msg = "Server: ";
//         msg += buffer;
//         send(cli_fd,msg.c_str(),msg.size(),0);
//     }
 
// }