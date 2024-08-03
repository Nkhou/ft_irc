
#include<iostream>
#include"srcs/server.hpp"
#include"./cmd/command.hpp"

int find_first_space(std::string str)
{
	size_t i = 0;
  while(i < str.length())
    {
        if(str[i] != ' ')
        {
            return i;
        }
        if (str[i] != '\n')
		        break;

        i++;
    
    }
	return 0;
}

int find_last_space(std::string str)
{
    
  size_t i = str.length() - 1;
   while(i > 0)
    {
        if(str[i] != ' ')
        {
            return i;
        }
        i--;
    }
	return INT_MAX;
}
int check_double_point(std::string cmd)
{
  for(size_t i = 0; i < cmd.length(); i++)
  {
      if(cmd[i] == ':')
      {
          return 1;
    }
    
  }
    return 0;
}
std::vector<std::string> split_string(std::string cmd,std::vector<std::string> split) 
{
    size_t start = find_first_space(cmd);
						size_t end = find_last_space(cmd);
                    if (check_double_point(cmd))
                        cmd = cmd.substr(start, cmd.length());
                    else
						cmd = cmd.substr(start, end + 1);
                    
					std::string word = "";
					
						for (size_t i = 0; i < cmd.length(); i++)
						{
                            if (cmd[i] == ':')
                            {
                                int j = i;
                                while (cmd[i])
                                    i++;
                                word = cmd.substr(j + 1, i);
                                if (word != "")
                                    split.push_back(word);
                                word = "";
                            }
                            else
                            {
                                while (cmd[i] && cmd[i] != ' ' )
                                {
                                    word += cmd[i];
                                    i++;
                                }
                                if (word != "")
                                    split.push_back(word);
                                word = "";
                            }

						}

    return split;
}
int check_error_nickname(std::string nickname)
{
    if(nickname.size() > 9)
    {
        return 1;
    }
    if(nickname.size() == 1)
    {
        return 1;
    }
    std::string alpha ="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
    std::string alpha_find = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ[]\\^_`{}|";

   
        if(alpha.find(nickname[0]) == std::string::npos)
        {
            return 1;
        }
        if(nickname.find_first_not_of(alpha_find) != std::string::npos)
        {
            return 1;
        }
     
    return 0;
}
int main(int argc,char **argv)
{
   int port_num;
    server ser;
    std::string pass;
     struct sockaddr_in addr;
     std::vector<std::string> buffer_stor;
     struct pollfd fdpoll;
       client cli;
     char buffer[1024];
     std::vector<std::string>split;
     std::string msg;
     std::string cmd;
     char hostname[1024] ;
     hostname [1023] = '\0';
     //returns the standard host name for the current machine
     gethostname(hostname, 1023);
     ser.hostname = hostname;
     
    if (argc != 3)
    {
        std::cout <<"Error Try Again"<<std::endl;
        exit (0);
    }
    port_num = std::atoi(argv[1]);
    pass= argv[2];  
    if(pass.empty())
    {
        std::cout <<"Error Password empty"<<std::endl;
        exit (0);
    }
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
    std::cout <<GREEN<< "Server is Ready to Accept a Connection...."<<std::endl;
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
                cli.flag = false;
               cli.flag_cmd = false;

               
                   ser.clients.push_back(cli);
                   ser.fds.push_back(fdpoll);
                   std::cout << "<<< New Client Connected >>> " << GREEN << cli_fd << std::endl;
                }
                else
                {
                    memset(buffer,0,sizeof(buffer));
                    ssize_t size = recv(ser.fds[i].fd,buffer,sizeof(buffer),0); //receive the data from the client
                    if(size == 0)
                    {
                        std::cout << "<<< Client Disconnected >>> " << buffer<< ser.fds[i].fd << std::endl;
                        close(ser.fds[i].fd);
                        ser.fds.erase(ser.fds.begin() + i);
                        ser.clients.erase(ser.clients.begin() + i - 1);
                      
                    }
                    else
                    {
                       buffer[size] = '\0';
                       if (size < 2)
						{
							buffer_stor.push_back(buffer);
							continue;
						}
                       
                    // Check if the buffer doesn't end with '\n' or '\r'
                     bool ends_with_newline = (buffer[size - 1] == '\n');
                     bool ends_with_carriage_return = (size > 2 && buffer[size - 2] == '\r');
                      
                      if (!ends_with_newline && buffer[size - 2] == '\r')
                      {
                          buffer_stor.push_back(buffer);
                          continue;
                      }
                      
                    //   // Null-terminate the buffer correctly
                      if (ends_with_newline)
                          buffer[size - 1] = '\0';
                      if (ends_with_carriage_return)
                          buffer[size - 2] = '\0';
                      
                      // Push the buffer to buffer_stor
                      buffer_stor.push_back(buffer);
                      
                      // Concatenate all buffers in buffer_stor into cmd
                      for (size_t j = 0; j < buffer_stor.size(); j++)
                            cmd += buffer_stor[j];
                      
                      // Clear buffer_stor after concatenation
                      buffer_stor.clear();
                      if (cmd.empty())
                          continue;

                       
						split.clear();

                        split = split_string(cmd, split);
                        if (split.size() == 0)
                        {
                            cmd.clear();
                            continue;
                        }
                        
						if (split[0][0] == ':')
							split.erase(split.begin());

						cmd.clear();
                        if(split[0] == "pass" || split[0] == "nick" || split[0] == "user")
                        {
                            for (size_t i = 0; i < split[0].length(); i++)
							    split[0][i] = std::toupper(split[0][i]);
                        }
                        std::cout << "Command: " << split[0] << std::endl;
                       if(ser.clients[i - 1].password == false)
                       {
                            if (split[0] == "PASS")
                            {
                                if(split.size() < 2)
                                {
                                    msg = msg_err(split[0], ser.hostname);
                                    if(send(ser.clients[i - 1].fd, msg.c_str(), msg.length(), 0) < 0)
                                    {
                                        std::cout << "Failed Send Try Again"<<std::endl;
                                    }
                                }
                                else if(split[1] == pass)
                                    ser.clients[i - 1].password = true;
                            
                            }

                        }
                        if (ser.clients[i - 1].nickname == "" || ser.clients[i - 1].password == true) 
                        {
        
                          if (split[0] == "NICK")
                          {
                         
                             if(split.size() >= 2)
                             {
                                if(check_error_nickname(split[1]) != 0)
                                     {
                                        msg = msg_erroneusnickname(split[1],  ser.hostname);
                                        if(send(ser.clients[i - 1].fd, msg.c_str(), msg.length() ,0) < 0)
                                        {
                                            std::cout << "Failed Send Try Again"<<std::endl;
                                        }
                                       
                                     }
                                     else
                                     {
                                        int a = 0;
                                        for (size_t j = 0; j < ser.clients.size(); j++)
                                        {
                                            if (ser.clients[j].nickname == split[1])
                                            {
                                                a = 1;
                                                msg = msg_nicknameinuse(split[1], ser.hostname);
                                               if(send(ser.clients[i - 1].fd, msg.c_str(), msg.length(), 0) < 0)
                                                {
                                                    std::cout << "Failed Send Try Again"<<std::endl;
                                                }
                                            }
                                        }
                                        if(a == 0)
                                          ser.clients[i - 1].nickname= split[1];
                                     }
                              }
                            if(split.size() < 2)
                             {
                                    msg = msg_nonicknamegiven(ser.hostname);
                                    if(send(ser.clients[i - 1].fd, msg.c_str(), msg.length(), 0) < 0)
                                        std::cout << "Failed Send Try Again"<<std::endl;
                             }
                            }
                        }
                        if(ser.clients[i - 1].user_name == ""  && ser.clients[i - 1].password == true)
                        {
                             if (split[0] == "USER")
                            {
                                 
                                if(split.size() == 5 || split.size() > 5)
                                {
                                      ser.clients[i - 1].user_name = split[1];
                                      ser.clients[i - 1].hostname = split[2];
                                      ser.clients[i - 1].servername = split[3];
                                      ser.clients[i - 1].realname = split[4];
                                    
                                }  
                                else if(split.size() < 5)
                                {
                                      msg = msg_err(split[0],ser.hostname);
                                         if(send(ser.clients[i - 1].fd, msg.c_str(), msg.length(), 0) < 0)
                                            std::cout << "Failed Send Try Again"<<std::endl;
                                }
                            }
                           
                        }
                       
                    }
                    if(ser.clients[i - 1].flag == false)
                    {
                        if(ser.clients[i - 1].nickname != "" && ser.clients[i - 1].user_name != "" && ser.clients[i - 1].password == true)
                        {
                            msg = msg_welcome(ser.clients[i - 1].nickname, ser.hostname);
                            if(send(ser.clients[i - 1].fd,msg.c_str(), msg.length(), 0) < 0)
                            {
                                return(std::cout << "Failed Send Try Again"<<std::endl,1);
                            }
                            ser.clients[i - 1].flag = true;
                         
                        }
                    }
                    if(ser.clients[i - 1].flag == false && split.size() > 0 )
                    {
                        
                            if(split[0] == "JOIN" || split[0] == "KICK" || split[0] == "TOPIC" || split[0] == "PRIVMSG" || split[0] == "MODE" )
                            {
                                if(ser.clients[i - 1].nickname == "" || ser.clients[i - 1].user_name == "" || ser.clients[i - 1].password == false)
                                {

                                 msg = msg_notregistered(ser.clients[i - 1].nickname, ser.hostname);
                                 if(send(ser.clients[i - 1].fd, msg.c_str(), msg.length(), 0) < 0)
                                 {
                                     std::cout << "Failed Send Try Again"<<std::endl;
                                 }
                                }
                                split.clear();
                             ser.clients[i - 1].flag_cmd = true;
                            }
                         
                    }
                 

}
            }
             
            if (split.size() > 0 && ser.clients[i - 1].password == true && ser.clients[i - 1].nickname != "" && ser.clients[i - 1].user_name != "")
            {
                if (split[0] != "PASS" && split[0] != "NICK" && split[0] != "USER" && split[0] != "PONG")
                {
                std::cout << split[0] << std::endl;
                std::cout << "3liya kat9alab"<< std::endl;
                ser.splited = split;
                ser.client_fd = ser.fds[i].fd;
                ser.client_cmd = ser.clients[i - 1];
                Command cmd;
                cmd.execCommand(&ser);
                split.clear();
                ser.client_fd = 0;
                }
            }
            i++;
        } 
         
    }    

    return (0);
}
