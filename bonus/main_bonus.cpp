/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saboulal <saboulal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 09:09:53 by saboulal          #+#    #+#             */
/*   Updated: 2024/07/12 10:29:28 by saboulal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"srcs/server_bonus.hpp"
int main(int argc, char **argv)
{
int port_num;
   ser_ ser;
    std::string pass;
     struct sockaddr_in addr;
     std::vector<std::string> buffer_stor;
     struct pollfd fdpoll;
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
    if (argc != 3)
    {
        std::cout <<"Error Try Again"<<std::endl;
        exit (0);
    }
    port_num = atoi(argv[1]);
    pass= argv[2];    
    if (port_num < 1024 || port_num > 65535)
    {
        std::cout <<"Error Try Again"<<std::endl;
        exit (0);
    }
    // Server_init(port_num);
     int pe = 1;
     addr.sin_family = AF_INET; // specifies IPV4(protocol for communication)
     addr.sin_port = htons(port_num); // sets the port ,function htons:converting it to network byte order 
     addr.sin_addr.s_addr = INADDR_ANY; // sets the address to any interface on the server (bind to any ip address on the server)

     ser.ser_fd = socket(AF_INET,SOCK_STREAM,0);// create socket server
     // not finich here some condition to check if the socket is created or not
     
     if (ser.ser_fd < 0)
    {   
       std::cout << "Failed Socket Try Again"<<std::endl;
       exit(0);
    }
    if (setsockopt(ser.ser_fd, SOL_SOCKET, SO_REUSEADDR, &pe, sizeof(pe)) == -1) //-> set the socket option (SO_REUSEADDR) to reuse the address
    {
        throw(std::runtime_error("faild to set option (SO_REUSEADDR) on socket"));
       
    }
	if (fcntl(ser.ser_fd, F_SETFL, O_NONBLOCK) == -1) //-> set the socket option (O_NONBLOCK) for non-blocking socket
		throw(std::runtime_error("faild to set option (O_NONBLOCK) on socket"));
    if (bind(ser.ser_fd,(struct sockaddr*)&addr,sizeof(addr)) < 0) //bind the socket to the address and port number
    {
        std::cout << "Failed Bind Try Again"<<std::endl;
        exit(0);
    }
    if (listen(ser.ser_fd,5) < 0) //listen for incoming connections
    {
        std::cout << "Failed Listen Try Again"<<std::endl;
        exit(0);
    }
    //polling
    fdpoll.fd = ser.ser_fd;
    fdpoll.events = POLLIN;
    ser.fds.push_back(fdpoll);
    std::cout << "Server is Ready to Accept a Connection...."<<std::endl;

    // create client 
    if(cli_client.fd(socket(AF_INET, SOCK_STREAM, 0) < 0))
    {
        std::cout << "Failed Socket Try Again"<<std::endl;
        exit(0);
    }
    cli_client.addr.sin_family = AF_INET;
    cli_client.addr.sin_port = htons(po);
    while(1)
    {
        
    }
}