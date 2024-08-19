
#include<iostream>
#include"srcs/server.hpp"
#include"./cmd/command.hpp"

void deletechannels(server &ser, int fd)
{
    if (ser.channels.size() == 0)
        return;
    for (size_t i = 0; i < ser.channels.size(); i++)
    {
        for (size_t j = 0; j < ser.channels[i].getUsers().size(); j++)
        {
            if (ser.channels[i].getUsers()[j].fd == fd)
            {
                std::string msg = ":" + ser.client_cmd.nickname + "!" + ser.client_cmd.user_name + "@" + ser.hostname + " QUIT :" + "Connection closed by the server\r\n";
                ser.channels[i].sendMessagenick( msg, ser.client_cmd.fd);
                ser.channels[i].removeUser(ser.channels[i].getUsers()[j].nickname);
                if (ser.channels[i].getUsers().size() == 0)
                {
                    std::cout << "Channel " << ser.channels[i].getName() << " is empty" << std::endl;
                    ser.channels.erase(ser.channels.begin() + i);
                }
            }
        }
    }
}
int find_first_space(std::string str)
{
	size_t i;
    size_t j;

    i = 0;
    while(i < str.length())
    {
        if(str[i] != '\n')
            break;
        i++;
    }
    j = i;
    while(j < str.length())
    {
        if(str[j] != ' ')
            return j;
        j++;

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
int server::check_error_nickname(std::string nickname)
{
    if(nickname.size() > 9)
    {
        return 1;
    }
    std::string alpha ="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
    std::string alpha_find = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ[]\\^_`{}|0123456789";

   
        if(alpha.find(nickname[0]) == std::string::npos )
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
   
     struct sockaddr_in addr;
     std::vector<std::string> buffer_stor;
     struct pollfd fdpoll;
       client cli;
     char buffer[1024];
     std::vector<std::string>split;
     std::string msg;
     std::string cmd;
     char hostname[1024] ;
     memset(hostname,0,sizeof(hostname));
     hostname [1023] = '\0';
     //returns the standard host name for the current machine
     gethostname(hostname, 1023);
     ser.hostname = hostname;
     
    if (argc != 3)
    {
        std::cout <<"Error Try Again"<<std::endl;
        exit (0);
    }
    std::string str = argv[1];
    if(str.length() > 5)
    {
        std::cout <<"Error Try Again"<<std::endl;
        exit (0);
    }
    port_num = std::atoi(argv[1]);
    ser.pass= argv[2];  
    if(ser.pass.empty())
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
     std::memset(&addr,0,sizeof(addr));
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
    if (listen(ser.ser_fd,SOMAXCONN) < 0) //listen for incoming connections
    {
        std::cout << "Failed Listen Try Again"<<std::endl;
        exit(0);
    }
    //polling
    fdpoll.fd = ser.ser_fd;
    fdpoll.events = POLLIN;
    fdpoll.revents = 0;
    ser.fds.push_back(fdpoll);
    // ssize_t size = 0;
    std::cout <<GREEN<< "Server is Ready to Accept a Connection...."<<std::endl;
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
                std::cout << "ip address " << cli.ip_addr << std::endl;
                cli.fd = cli_fd;
                cli.nickname ="";
                cli.realname = "";
                cli.user_name = "";
                cli.servername = "";
                cli.hostname = "";
            
               
                cli.password = false;
                cli.flag = false;
                cli.flag_cmd = false;

               
                   ser.clients.push_back(cli);
                   ser.fds.push_back(fdpoll);
                   std::cout << "<<< New Client Connected >>> " << GREEN << cli_fd << std::endl;
                }
                else
                {
                    std::string buff;
                    
                    memset(buffer,0,sizeof(buffer));
                    ssize_t size = recv(ser.fds[i].fd,buffer,sizeof(buffer),0);
                    // size += n;
                    buff += buffer;
                    while (buff.find("\n") == std::string::npos  && buff.size() > 0)
                    {
                        memset(buffer,0,sizeof(buffer));
                        std::cout << "size " << size << std::endl;
                        size = recv(ser.fds[i].fd,buffer,sizeof(buffer),0); //receive the data from the client
                        buff += buffer;
                        // size += n;
                    }
                    // memset(buffer,0,sizeof(buffer));
                    // ssize_t size = recv(ser.fds[i].fd,buffer,sizeof(buffer),0); //receive the data from the client
                    // buff += buffer;
                    // need to add ctrl + c to close the server and Ctrl + D to  join the buffer

                    if(size == 0)
                    {
                        // std::cout <<"<<<<<<<<<<<hi"<<std::endl;
                        deletechannels(ser, ser.fds[i].fd);
                        close(ser.fds[i].fd);
                        // close(ser.fds[i].fd);
                        ser.fds.erase(ser.fds.begin() + i);
                        buffer_stor.clear();
                        cmd.clear();
                        ser.clients.erase(ser.clients.begin() + i - 1);

                        std::cout << "<<< Client Disconnected  >>> " << buffer<< ser.fds[i].fd << std::endl;
                    }
                    else
                    {
                    //    buff[buff.size()] = '\0';
                       if (size < 2)
						{
							// buffer_stor.push_back(buff);
							continue;
						}
                       
                    // Check if the buff doesn't end with '\n' or '\r'
                    size_t ends = buff.find("\n");
                    size_t ends1 = buff.find("\r");
                    if (ends != std::string::npos && ends1 != std::string::npos)
                    {
                        // std::cout << "buff " << buff.substr(0, ends) << std::endl;
                        // buff.substr(0, ends);
                        buffer_stor.push_back(buff.substr(0, ends - 1));
                    }
                    else if (ends != std::string::npos)
                    {
                        // std::cout << "2buff " << buff.substr(0, ends - 1)<< "------"<< std::endl;
                        // buff.substr(0, ends);
                        buffer_stor.push_back(buff.substr(0, ends));
                    }
                    // else if (ends1 != std::string::npos)
                    // {
                    //     std::cout << "3buff " << buff.substr(0, ends1) << std::endl;
                    //     buff.substr(0, ends1);
                    //     buffer_stor.push_back(buff.substr(0, ends1 - 1));
                    // }
                    // else
                    //     buff.substr(0, buff.size() - 1);
                    // std::cout << "buff " << buff << std::endl;
                    //  bool ends_with_newline = buff.find("\n") == std::string::npos;
                    //  std::cout << "ends_with_newline " << ends_with_newline << std::endl;
                    //  bool ends_with_carriage_return = (size > 2 && buff.find("\r") == std::string::npos);
                    //     std::cout << "ends_with_carriage_return " << ends_with_carriage_return << std::endl;
                      
                    //   if (!ends_with_newline && buff.find("\r") == std::string::npos)
                    //   {
                    //       buffer_stor.push_back(buff);
                    //       continue;
                    //   }
                      
                    // //   // Null-terminate the buff correctly
                    //   if (ends_with_newline)
                    //       buff[buff.size() - 1] = '\0';
                    //   if (ends_with_carriage_return)
                    //       buff[buff.size() - 2] = '\0';
                      // Push the buffer to buffer_stor
                    //   buffer_stor.push_back(buff);
                      for (size_t j = 0; j < buffer_stor.size(); j++)
                      {
                          std::cout << "buffer_stor " << buffer_stor[j]<< "--------"<< std::endl;
                      }
                      
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
                            cmd.clear();
                         
                        
                        
						if (split[0][0] == ':')
							split.erase(split.begin());

						cmd.clear();
                        if(split[0] == "pass" || split[0] == "nick" || split[0] == "user" || split[0] == "join" || split[0] == "kick" || split[0] == "topic" || split[0] == "privmsg" || split[0] == "mode" ||  split[0] == "invite" || split[0] == "quit")
                        {
                            for (size_t i = 0; i < split[0].length(); i++)
							    split[0][i] = std::toupper(split[0][i]);
                        }
                        if (split[0] == "QUIT")
                        {
                            close(ser.fds[i].fd);
                            ser.fds.erase(ser.fds.begin() + i);
                            ser.clients.erase(ser.clients.begin() + i - 1);
                            split.clear();
                        }
                       if (i > 0 && ser.clients[i - 1].password == false )
                       {

                            if (split[0] == "PASS")
                            {
                                std::cout << "split " <<split[1]<<"--------" << std::endl;
                                if(split.size() < 2)
                                {
                                    msg = msg_err(split[0], ser.hostname);
                                    if(send(ser.clients[i - 1].fd, msg.c_str(), msg.length(), 0) < 0)
                                        return(std::cout << "Failed Send Try Again"<<std::endl,1);
                                }
                                else if(split[1] == ser.pass && ser.clients[i - 1].nickname == "" && ser.clients[i - 1].user_name == "")
                                {
                                    ser.clients[i - 1].password = true;
                                }
                                split.clear();
                           }    
                         

                        }
                        // i > 0 && split.size() > 0 && ser.clients[i - 1].password == true &&  (ser.clients[i - 1].nickname == "" || ser.clients[i - 1].user_name == "")
                        while (i > 0 && split.size() > 0 && ser.clients[i - 1].password == true &&  (ser.clients[i - 1].nickname == "" || ser.clients[i - 1].user_name == ""))
                        {

                            // std::cout << "fd " <<ser.clients[i - 1].fd<< std::endl;
                           
                            // std::cout <<
                            // if (!(ser.clients[i - 1].nickname == "" || ser.clients[i - 1].user_name == ""))
                            //     break;
                            std::cout << "split " <<split[0] << std::endl;
                            if (split.size() > 0 && split[0] == "NICK" )
                            {
                                if(split.size() >= 2)
                                {
                                    std::cout << "fd " <<ser.clients[i - 1].fd<< std::endl;
                                   if(ser.check_error_nickname(split[1]) != 0)
                                    {
                                       msg = message_err_nick_name(ser.hostname, ERR_ERRONEUSNICKNAME_CODE,"*", ser.clients[i - 1].nickname, ERR_ERRONEUSNICKNAME);
                                       if(send(ser.clients[i - 1].fd, msg.c_str(), msg.length() ,0) < 0)
                                           return(std::cout << "Failed Send Try Again"<<std::endl,1);
                                        split.clear();
                                        
                                    }
                                    else
                                    {
                                       int a = 0;
                                       for (size_t j = 0; j < ser.clients.size(); j++)
                                       {
                                           if (ser.clients[j].nickname == split[1])
                                           {
                                               a = 1;
                                               msg = message_err_nick_name(ser.hostname, ERR_NICKNAMEINUSE_CODE,"*", ser.clients[i - 1].nickname, ERR_NICKNAMEINUSE_MSG);
                                              if(send(ser.clients[i - 1].fd, msg.c_str(), msg.length(), 0) < 0)
                                                  return(std::cout << "Failed Send Try Again"<<std::endl,1);
                                                split.clear();
                                               
                                           }
                                       }
                                       if(a == 0 && i > 0 )
                                       {
                                         ser.clients[i - 1].nickname = split[1];
                                         split.clear();
                
                                       }
                                   }
                                }
                                if(i > 0 && split.size() < 2 && split.size() != 0 && ser.clients[i - 1].nickname == "")
                                {
                                  msg = message_err_nick_name(ser.hostname, ERR_NONICKNAMEGIVEN_CODE,"*", ser.clients[i - 1].nickname, ERR_NONICKNAMEGIVEN_MSG);
                                        if(send(ser.clients[i - 1].fd, msg.c_str(), msg.length(), 0) < 0)
                                            return(std::cout << "Failed Send Try Again"<<std::endl,1);
                                    split.clear();
                                }           
                            }
                            else if (split.size() > 0 && split[0] == "USER")
                            {
                                if(split.size() == 5 || split.size() > 5)
                                {
                                    // std::cout << "-------fd " <<ser.clients[i - 1].fd<< std::endl;
                                      ser.clients[i - 1].user_name = split[1];
                                      ser.clients[i - 1].hostname = split[2];
                                      ser.clients[i - 1].servername = split[3];
                                      ser.clients[i - 1].realname = split[4];
                                    split.clear();
                                    //   std::cout<< "split[1]"  << split[1] << std::endl;
                                    //     std::cout<< "split[2]"  << split[2] << std::endl;
                                    //     std::cout<< "split[3]"  << split[3] << std::endl;
                                    //     std::cout<< "split[4]"  << split[4] << std::endl;
                                   
                                }  
                                else if(split.size() < 5 && split.size() != 0)
                                {
                                    msg = msg_err(split[0],ser.hostname);
                                    if(send(ser.clients[i - 1].fd, msg.c_str(), msg.length(), 0) < 0)
                                        return(std::cout << "Failed Send Try Again"<<std::endl,1);
                                    split.clear();
                                }
                                
                            }
                            else if (split.size() > 0 && split[0] != "NICK" && split[0] != "USER")
                            {
                                if (i > 0 && ser.clients[i - 1].flag == false)
                                {
                                         std::cout << buffer << std::endl;
                                        if(split[0] == "JOIN" || split[0] == "KICK" || split[0] == "TOPIC" || split[0] == "PRIVMSG" || split[0] == "MODE" || split[0] == "INVITE")
                                        {
                                            if(ser.clients[i - 1].nickname == "" || ser.clients[i - 1].user_name == "" || ser.clients[i - 1].password == false)
                                            {
                                            
                                             msg = msg_notregistered(ser.clients[i - 1].nickname, ser.hostname);
                                             if(send(ser.clients[i - 1].fd, msg.c_str(), msg.length(), 0) < 0)
                                                return(std::cout << "Failed Send Try Again"<<std::endl,1);
                                            }
                                            // split.clear();
                                         ser.clients[i - 1].flag_cmd = true;
                                        }
                                }
                                // else
                                    split.clear();
                            }
                             if (ser.clients[i - 1].flag == false)
                            {
                                std::cout << ser.clients[i - 1].nickname << std::endl;
                                std::cout << ser.clients[i - 1].user_name << std::endl;
                                // std::cout << "fd " <<ser.clients[i - 1].fd<< std::endl;
                                  if (ser.clients[i - 1].nickname != "" && ser.clients[i - 1].user_name != "")
                                  {
                                      msg = msg_welcome(ser.clients[i - 1].nickname, ser.hostname);
                                      if(send(ser.clients[i - 1].fd,msg.c_str(), msg.length(), 0) < 0)
                                      {
                                          return(std::cout << "Failed Send Try Again"<<std::endl,1);
                                      }
                                      ser.clients[i - 1].flag = true;
                                      split.clear();
                                      break;
                                  }
                            }
                            
                   
                    }
                         
                    
                       
                    }
                 

}

            }
            
            for (size_t j = 0; j < split.size(); j++)
            {
                 std::cout << i<<"  ------   " <<split[j] <<"-------"<< std::endl;
                 std::cout << "^^^^^^^" << ser.clients[i - 1].flag << std::endl;
            }
                std::cout << "fd " <<ser.clients[i - 1].fd<< std::endl;
                if (i > 0 && split.size() > 0 && ser.clients.size() > 0 && split[0] != "PONG" && ser.clients[i - 1].flag == true)
                {
                    std::cout << "split &&&&&&&&&&&&&&&" <<split[1] << std::endl;
                    std::cout << "split " <<split[0] << std::endl;
                    std::cout << "fd " <<ser.clients[i - 1].fd<< std::endl;
                    std::cout << "fd_ser " <<ser.fds[i].fd<< std::endl;
                    ser.splited = split;
                    ser.client_fd = ser.fds[i].fd;
                    ser.client_cmd = ser.clients[i - 1];
                    Command cmd;
                    cmd.execCommand(&ser);
                    split.clear();
                    ser.client_fd = 0;
                }
                else 
                {
                    split.clear();
                }
            
            i++;
        } 
         
    }
    while (ser.fds.size() > 0)
    {
        close(ser.fds[0].fd);
        ser.fds.erase(ser.fds.begin());
    }
    return (0);
}