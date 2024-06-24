
#include<iostream>
#include"srcs/server.hpp"

std::vector<std::string>  split_string(std::string str)
{
    std::vector<std::string> split;
    std::string word;
    for (size_t i = 0; i < str.size(); i++)
    {
        if (str[i] == ' ')
        {
            split.push_back(word);
            word.clear();
        }
        else
        {
            word += str[i];
        }
    }
    split.push_back(word);
    return split;
}

int main(int argc,char **argv)
{
   int port_num;
    server ser;
    std::string pass;
     struct sockaddr_in addr;
     struct pollfd fdpoll;
       client cli;
     char buffer[1024];
     std::vector<std::string>split;
     std::string msg;
     
   
    
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
    // infinite loop
    while(1) 
    {
        //now using function poll to check if there is any incoming connection
        if(poll(ser.fds.data(),ser.fds.size(),-1) < 0)
        {
            std::cout << "Failed Poll Try Again"<<std::endl;
            exit(0);
        }
        size_t i = 0;
        while(i < ser.fds.size())
        {
            if(ser.fds[i].revents & POLLIN)
            {
                if(ser.fds[i].fd == ser.ser_fd)
                {
                 
                   struct sockaddr_in addr;
                   struct pollfd fdpoll;
                   socklen_t len = sizeof(addr);
               
                   int cli_fd = accept(ser.ser_fd,(struct sockaddr*)&addr,&len); //accept the incoming connection
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
                   
                cli.ip_addr = inet_ntoa(addr.sin_addr);
                cli.hostname = "";
                cli.fd = cli_fd;
                cli.nickname ="";
                cli.realname = "";
                cli.user_name = "";
                cli.servername = "";
                cli.password = false;
               
                   ser.clients.push_back(cli);
                   ser.fds.push_back(fdpoll);
                   std::cout << GREEN << "<<< New Client Connected >>> " << cli_fd << std::endl;
                }
                else
                {
                    memset(buffer,0,sizeof(buffer));
                    ssize_t size = recv(ser.fds[i].fd,buffer,sizeof(buffer),0); //receive the data from the client
                    if(size <= 0)
                    {  
                      std::cout << "client Disconnected"<<ser.fds[i].fd<<std::endl;
                      close(ser.fds[i].fd);
                    }

                    else
                    {
                       buffer[size] = '\0';
                       split = split_string(buffer);
                       
                        
                          if (split[0] == "PASS")
                          {
                                
                                printf("%s\n",split[0].c_str());
                                printf("%s\n",split[1].c_str());
                            if (split[1] == "PASS")
                            {
                                 send(ser.fds[i].fd,"Welcome to the server",sizeof("Welcome to the server"),0);
                                 ser.clients[i].setPassword_bool(true);
                            }
                            else
                            {
                                 send(ser.fds[i].fd,"Wrong Password",sizeof("Wrong Password"),0);
                            }
                          }
                          else if (split[0] == "NICK")
                          {
                            ser.clients[i].nickname=split[1];
                            
                          }
                          else if (split[0] == "USER")
                          {
                            ser.clients[i].user_name = split[1];
                            ser.clients[i].hostname = split[2];
                            ser.clients[i].servername = split[3];
                            ser.clients[i].realname = split[4];

                          }
                          else if (split[0] == "QUIT")
                          {
                            close(ser.fds[i].fd);
                          }
                        //   else
                        //   {
                        //     send(ser.fds[i].fd,"Invalid Command",sizeof("Invalid Command"),0);
                        //   }
                    }
                  
                }
            }
            i++;
        }
         
    }    

    return (0);
}

