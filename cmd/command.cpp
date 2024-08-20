#include "command.hpp"



void Command::execCommand(server *server) {
    if (server->splited.size() == 0 || server->client_fd == 0)
        return;
    try
    {
        executecmd(server);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return;
    }
}

client *Command::getClientByFd(server *ser, int fd)
{
    for (unsigned long i = 0; i < ser->clients.size(); i++)
    {
        if (ser->clients[i].fd == fd)
            return &ser->clients[i];
    }
    return NULL;
}

void Command::nick_auth(std::vector<std::string> split,server *server,int client_fd)
{
  
     std::string msg;
     if( split.size() < 2)
     {
       std::string msg = message_err_nick_name(server->hostname, ERR_NONICKNAMEGIVEN_CODE,"*", server->client_cmd.nickname, ERR_NONICKNAMEGIVEN_MSG);
             if(send(client_fd, msg.c_str(), msg.length(), 0) < 0)
                 std::cout << "Failed Send Try Again"<<std::endl;
     }
     else if(server->check_error_nickname(split[1]) != 0)
     {
        msg = message_err_nick_name(server->hostname, ERR_ERRONEUSNICKNAME_CODE,"*", server->client_cmd.nickname, ERR_ERRONEUSNICKNAME);
        if(send(client_fd, msg.c_str(), msg.length() ,0) < 0)
            std::cout << "Failed Send Try Again"<<std::endl;
     }
    else
    {
        int a = 0;
            for (size_t j = 0; j < server->clients.size(); j++)
            {
                if (server->client_cmd.nickname == split[1])
                {
                   a = 1;
                   msg = message_err_nick_name(server->hostname, ERR_NICKNAMEINUSE_CODE,"*", server->client_cmd.nickname, ERR_NICKNAMEINUSE_MSG);
                   if(send(client_fd, msg.c_str(), msg.length(), 0) < 0)
                       std::cout << "Failed Send Try Again"<<std::endl;
                    

                }
            }
            if (a == 0)
            {
                     std::string  msg = ":" + server->client_cmd.nickname +"!~"+server->client_cmd.user_name+"@"+server->hostname+ " NICK " + split[1] + "\r\n";
                      if(send(client_fd, msg.c_str(), msg.length(), 0) < 0)
                          std::cout<< "Failed Send Try Again"<<std::endl;
                
                for (unsigned long i = 0; i < server->channels.size(); i++)
                {
                   for (unsigned long j = 0; j < server->channels[i].getUsers().size(); j++)
                   {
                       if(server->channels[i].getUsers()[j].nickname == server->client_cmd.nickname)
                       {
                        server->channels[i].setOpiritornames(split[1]);
                       
                           server->channels[i].sendMessagenick(msg, client_fd);
                       }
                   }
                }
                for (unsigned long i = 0; i < server->clients.size(); i++)
                {
                    if (server->clients[i].fd == client_fd)
                    {
                        server->clients[i].nickname = split[1];
                        server->client_cmd.nickname = split[1];
                        return;
                    }
                }
               
                
            }
            
    }
}
    
void Command::user_auth(std::vector<std::string> split,server *server,int client_fd)
{
    std::cout << "user_authhereeeeeee" << std::endl;
    
     if (split.size() == 5 || split.size() > 5)
     {
           std::string msg = msg_may_registered(server->client_cmd.nickname,server->hostname);
            if(send(client_fd, msg.c_str(), msg.length(), 0) < 0)
                std::cout << "Failed Send Try Again"<<std::endl;
          
     }  
     else
     {     
           std::cout << "ana wasal tal hna" << std::endl;
           std::string msg = msg_err(split[0],server->hostname);
              if(send(client_fd, msg.c_str(), msg.length(), 0) < 0)
                std::cout << "Failed Send Try Again"<<std::endl;
     }
    
}

void Command::pass_auth(server *server)
{
   
    if(server->splited.size() < 2)
         {
             std::string msg = msg_err(server->splited[1], server->hostname);
             if(send(server->client_cmd.fd, msg.c_str(), msg.length(), 0) < 0)
                 std::cout << "Failed Send Try Again"<<std::endl;
         }
         else
         {
             std::string msg = msg_may_registered(server->splited[1],server->hostname);
            if(send(server->client_cmd.fd, msg.c_str(), msg.length(), 0) < 0)
                std::cout << "Failed Send Try Again"<<std::endl;
         }
}
void Command::executecmd(server *server) {
    if(server->splited[0] == "NICK")
    {
         nick_auth(server->splited,server,server->client_fd);
         return;
    }
    else if(server->splited[0] == "USER")
    { 
        user_auth(server->splited,server,server->client_fd);
        return;
    }
    else if(server->splited[0] == "PASS")
    {
         pass_auth(server);
         return;
    }
    else if (server->splited[0] == "QUIT")
    {
        
        for (unsigned long i = 0; i < server->clients.size(); i++)
        {
            if (server->clients[i].fd == server->client_fd)
            {
                server->clients.erase(server->clients.begin() + i);
               
            }
        }
        std::string msg = ":" + server->client_cmd.nickname + "!~" + server->client_cmd.user_name + "@" + server->hostname + " QUIT :" + server->splited[1] + "\r\n";
        for (unsigned long i = 0; i < server->channels.size(); i++)
        {
            for (unsigned long j = 0; j < server->channels[i].getUsers().size(); j++)
            {
                if (server->channels[i].getUsers()[j].fd == server->client_fd)
                {
                    server->channels[i].sendMessagenick(msg, server->client_fd);
                    server->channels[i].removeUser(server->channels[i].getUsers()[j].nickname);
                    if (server->channels[i].getUsers().size() == 0)
                    {
                        server->channels.erase(server->channels.begin() + i);
                    }
                    
                }
            }
        }
        close(server->client_fd);
        return;
    }

    if (server->splited[0] == "JOIN")
    {
        if (server->splited.size() < 2)
        {
            std::string msg = msg_errcmd(server->client_cmd.nickname, server->splited[0], server->hostname);
            if(send(server->client_fd, msg.c_str(), msg.length(), 0) < 0)
            {
                std::cout << "Failed Send Try Again"<<std::endl;
            }
            return;
        }
        ParceCommand(server->splited, server->client_fd, server->hostname);
        JoinCommand(server);
    }
    else if (server->splited[0] == "KICK")
    {
        if (server->splited.size() < 3)
        {
                std::string msg = kickerr(server->client_cmd.nickname, server->hostname);
                    if(send(server->client_fd, msg.c_str(), msg.length(), 0) < 0)
                    {
                        std::cout << "Failed Send Try Again"<<std::endl;
                    }
                    return;
            return;
        }
        ParceCommandkick(server->splited, server->client_fd, server->channels, server->hostname);
        KickCommand(server);
    }
    else if (server->splited[0] == "MODE")
    {
        std::cout << "MODE" << std::endl;
        if (server->splited.size() < 2)
        {
            std::string msg = msg_errcmd(server->client_cmd.nickname,server->splited[0], server->hostname);
            if(send(server->client_fd, msg.c_str(), msg.length(), 0) < 0)
            {
                std::cout << "Failed Send Try Again"<<std::endl;
            }
            return;
        }
        int c = 0;
        for (size_t i = 0; i < server->channels.size(); i++)
        {
            if (server->channels[i].getName() == server->splited[1])
            {
                c = 1;
                break;

                
            }
        }
        if (c == 0)
        {
            for (size_t i = 0; i < server->clients.size(); i++)
            {
                if (server->clients[i].fd == server->client_fd)
                {
                    std::string msg = ERR_NOSUCHCHANNEL(server->splited[1], server->hostname);
                    if(send(server->client_fd, msg.c_str(), msg.length(), 0) < 0)
                    {
                        std::cout << "Failed Send Try Again"<<std::endl;
                    }
                    return;
                }
            }
        }
        if (server->splited.size() >= 3 && server->splited[2][0] != '-' && server->splited[2][0] != '+')
        {
            for (size_t i = 0; i < server->clients.size(); i++)
            {
                if (server->clients[i].fd == server->client_fd)
                {
                    std::string msg = ":" + server->clients[i].nickname + " MODE " + server->splited[1] + " :is unknown mode char to me\r\n";
                    if(send(server->client_fd, msg.c_str(), msg.length(), 0) < 0)
                    {
                        std::cout << "Failed Send Try Again"<<std::endl;
                    }
                    return;
                }
            }
        }
        ParceModeCommand(server->splited, server->client_fd);
        ModeCommand(server);
    }
    else if (server->splited[0] == "TOPIC")
    {
        if (server->splited.size() < 2)
        {
            std::string msg = msg_errcmd(server->client_cmd.nickname,server->splited[0], server->hostname);
            if(send(server->client_fd, msg.c_str(), msg.length(), 0) < 0)
            {
                std::cout << "Failed Send Try Again"<<std::endl;
            }
            return;
        }
        ParceTopic(server->splited, server->client_fd);
        TopicCommand(server);
    }
    else if (server->splited[0] == "INVITE")
    {
        if (server->splited.size() < 3)
        {
            std::string msg = msg_errcmd(server->client_cmd.nickname,server->splited[0], server->hostname);
            if(send(server->client_fd, msg.c_str(), msg.length(), 0) < 0)
            {
                std::cout << "Failed Send Try Again"<<std::endl;
            }
            return;
        }
        ParceInvite(server->splited, server->client_fd);
        InviteCommand(server);
    }
    else if (server->splited[0] == "PRIVMSG")
    {
        if (server->splited.size() <= 1)
        {
            std::string msg = msg_errcmd(server->client_cmd.nickname, server->splited[0], server->hostname);
            if(send(server->client_fd, msg.c_str(), msg.length(), 0) < 0)
            {
                std::cout << "Failed Send Try Again"<<std::endl;
            }
            return;
        }
        ParcePrivmsg(server->splited, server->client_fd);
        PrivmsgCommand(server);
    }
    else if (server->splited[0] != "PASS" && server->splited[0] != "NICK" && server->splited[0] != "USER")
    {
        std::string  msg =  unknowncommand(server->splited[0], server->client_cmd,server->hostname);
        if(send(server->client_fd, msg.c_str(), msg.length(), 0) < 0)
        {
            std::cout << "Failed Send Try Again"<<std::endl;
        }
    }
    this->args.clear();
    this->fd = 0;
}
void Command::addusertoChannel(server *server, std::string channel, int o) {
    for (unsigned long i = 0; i < server->channels.size(); i++)
    {
        if (server->channels[i].getName() == channel)
        {
            for (unsigned long j = 0; j < server->clients.size(); j++)
            {
                if (server->clients[j].fd == server->client_fd)
                {
                    if ((server->channels[i].addUser(server->clients[j], o) == 1) || (server->channels[i].addUser(server->clients[j], o) == 2))
                    {
                        server->channels[i].removefd(server->client_fd);
                        return;
                    }
                    server->channels[i].notifyUserJoin(server->clients[j].nickname, server->client_cmd.ip_addr, o);
                    server->channels[i].removefd(server->client_fd);
                    return ;
                }
            } 
            return;
        }
    }
}
int checkMode(std::vector<std::string> mode, std::string mode1) {
    for (unsigned long i = 0; i < mode.size(); i++)
    {
        if (mode[i] == mode1)
            return 1;
    }
    return 0;
}
