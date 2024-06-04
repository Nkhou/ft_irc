/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saboulal <saboulal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 13:28:11 by saboulal          #+#    #+#             */
/*   Updated: 2024/06/04 17:13:29 by saboulal         ###   ########.fr       */
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
    //  size_t i = 0;
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
    if(setsockopt(ser_fd, SOL_SOCKET, SO_REUSEADDR, &pe, sizeof(pe)) == -1) //-> set the socket option (SO_REUSEADDR) to reuse the address
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
        // while(i < fds.size())
        // {
        //     if(fds[i].revents & POLLIN)
        //     {
        //         if(fds[i].fd == ser_fd)
        //         {
        //             AcceptNewclient();
        //         }
        //         else
        //         {
        //             ReceiveNewData(fds[i].fd);
        //         }
        //     }
        //     i++;
        // }
        
    }
} 


void Client::setIP(std::string ip_addr)
{
    this->ip_addr = ip_addr;
}

int Client::setFD(int fd)
{
    this->fd = fd;
    return fd;
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
//     cli.setFD(cli_fd);
//     cli.setIP(inet_ntoa(addr.sin_addr));
//     clients.push_back(cli);
//     fdpoll.fd = cli_fd;
//     fdpoll.events = POLLIN;
//     fdpoll.revents = 0;
//     fds.push_back(fdpoll);
// }
void Server::ReceiveNewData(int cli_fd)
{
    char buffer[1024]; 
    int read_bytes = recv(cli_fd,buffer,1024,0); //receive data from client
    if(read_bytes < 0)
    {
        std::cout << "Failed Receive Try Again"<<std::endl;
        exit(0);
    }
    std::cout << "Client: " << buffer << std::endl;
}