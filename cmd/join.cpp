#include "command.hpp"
void Command::ParceCommand(std::vector<std::string> command, int fd, std::string hostname)
{
    for (unsigned long i = 1; i < 3; i++)
    {
        unsigned long j = 0;
        while (j < command[i].size())
        {
            if (command[i][j] == ',')
            {
                j++;
                continue;
            }

            unsigned long k = j;

            if (command[i][j] == '#')
            {

                    j++; 
                    while (j < command[i].size() && ((command[i][j] != ',')))
                    {
                        j++;
                    }
                    if (j > k)
                    {
                        this->args.push_back(command[i].substr(k, j - k));
                    }
            }
            else if (i == 2)
            {
                while (j < command[i].size() && command[i][j] != ',')
                {
                    j++;
                }
                if (j > k)
                {
                    this->keys.push_back(command[i].substr(k, j - k));
                }
                
            }
            else if (i == 1 && command[i][j] != '#')
            {
                while (j < command[i].size() && command[i][j] != ',')
                {
                    j++;
                }
                std::string msg = ChannelExist(command[0], hostname);
                if(send(fd, msg.c_str(), msg.length(), 0) < 0)
                {
                    std::cout << "Failed Send Try Again"<<std::endl;
                }
            }
        }
    }
    this->fd = fd;
}
void Command::JoinCommand(server *server) {
    if (this->args.size() == 0)
        return;
    if (this->args.size() == 1)
    {
        if (server->channels.size() == 0){
            server->channels.push_back(Channel(this->args[0]));
            if (this->keys.size() > 0)
            {
                server->channels[0].setMode("+k");
                server->channels[0].setKey(this->keys[0]);
            }
        }
        else
        {
            for (unsigned long i = 0; i < server->channels.size(); i++)
            {
                if (server->channels[i].getName() == this->args[0])
                {
                    if (server->channels[i].getUser(server->client_fd))
                    {
                        return ;
                    }
                    if (checkMode(server->channels[i].getMode(), "+i") == 1 && !server->channels[i].userinvite(server->client_fd))
                    {
                        std::string msg = ERR_INVITEONLYCHAN(args[0], server->hostname);
                        if (send(fd, msg.c_str(), msg.length(), 0) < 0)
                        {
                            std::cout << "Failed Send Try Again"<<std::endl;
                        }
                        return ;
                    }
                    else if (checkMode(server->channels[i].getMode(), "+k") == 1 && this->keys.size() > 0 && server->channels[i].getKey() != this->keys[0])
                    {
                        if (server->channels[i].getKey() != this->args[1] || checkMode(server->channels[i].getMode(), "+k") == 1)
                        {
                            std::string msg = ERR_BADCHANNELKEY(args[0], server->hostname);
                            if (send(fd, msg.c_str(), msg.length(), 0) < 0)
                            {
                                std::cout << "Failed Send Try Again"<<std::endl;
                            }
                            return ;
                        }
                        return ;
                    }
                    else if (server->channels[i].getKey().size() > 0 && this->keys.size() == 0)
                    {
                        std::string msg = ERR_BADCHANNELKEY(args[0], server->hostname);
                        if (send(fd, msg.c_str(), msg.length(), 0) < 0)
                        {
                            std::cout << "Failed Send Try Again"<<std::endl;
                        }
                        return ;
                    }
                    else if (server->channels[i].getLimits() && server->channels[i].getUsers().size() + 1 > server->channels[i].getMaxUsers())
                    {
                        std::string msg = ERR_CHANNELISFULL(args[0], server->hostname);
                        if (send(fd, msg.c_str(), msg.length(), 0) < 0)
                        {
                            std::cout << "Failed Send Try Again"<<std::endl;
                        }
                        return ;
                    }
                    addusertoChannel(server, this->args[0], 0);
                    return ;
                }
            }
            server->channels.push_back(Channel(this->args[0])); 
            server->channels[server->channels.size() - 1].setMode("+k");
            if (this->keys.size() > 0)
            {
                server->channels[server->channels.size() - 1].setKey(this->keys[0]);
                server->channels[server->channels.size() - 1].setMode("+k");
            }
        }
        addusertoChannel(server, this->args[0], 1);
        return ;
    }
    else
    {
        
        for (unsigned long i = 0; i < this->args.size(); i++)
        {
            int c = 0;
            if (server->channels.size() == 0)
            {
                server->channels.push_back(Channel(this->args[i]));
                if (this->keys.size() > 0)
                {
                    server->channels[0].setMode("+k");
                    server->channels[0].setKey(this->keys[i]);
                }
            }
            else
            {
               
                for (unsigned long j = 0; j < server->channels.size(); j++)
                {
                    if (server->channels[j].getName() == this->args[i])
                    {
                        if (!server->channels[i].getUser(server->client_fd))
                        {

                            if (checkMode(server->channels[j].getMode(), "+i") == 1 && !server->channels[i].userinvite(server->client_fd))
                            {  
                                c = 1;
                                std::string msg = ERR_INVITEONLYCHAN(args[0], server->hostname);
                                if (send(fd, msg.c_str(), msg.length(), 0) < 0)
                                {
                                    std::cout << "Failed Send Try Again"<<std::endl;
                                }
                            }
                            else if (checkMode(server->channels[j].getMode(), "+k") && this->keys.size() == 0)
                            {
                                c = 1;
                                std::string msg = ERR_BADCHANNELKEY(args[0], server->hostname);
                                if (send(fd, msg.c_str(), msg.length(), 0) < 0)
                                {
                                    std::cout << "Failed Send Try Again"<<std::endl;
                                }
                            }
                            else if (checkMode(server->channels[j].getMode(), "+k") == 1 && i <= this->keys.size() && server->channels[j].getKey() != this->keys[i])
                            {
                                c = 1;
                                if (server->channels[j].getKey() != this->args[i + 1])
                                {
                                     std::string msg = ERR_BADCHANNELKEY(args[0], server->hostname);
                                    if (send(fd, msg.c_str(), msg.length(), 0) < 0)
                                    {
                                        std::cout << "Failed Send Try Again"<<std::endl;
                                    }
                                }
                            }
                            else if (server->channels[i].getKey().size() > 0 && i <= this->keys.size() && server->channels[i].getKey() != this->keys[i])
                            {
                                c=1;
                                 std::string msg = ERR_BADCHANNELKEY(args[0], server->hostname);
                                if (send(fd, msg.c_str(), msg.length(), 0) < 0)
                                {
                                    std::cout << "Failed Send Try Again"<<std::endl;
                                }
                            }
                            else if (server->channels[i].getLimits() && server->channels[j].getUsers().size() + 1 > server->channels[j].getMaxUsers())
                            {
                                c = 1;
                                std::string msg = ERR_CHANNELISFULL(args[0], server->hostname);
                                if (send(fd, msg.c_str(), msg.length(), 0) < 0)
                                {
                                    std::cout << "Failed Send Try Again"<<std::endl;
                                }
                            }
                            if (c == 0)
                            {
                                addusertoChannel(server, this->args[i], 0);
                            }
                            break ;
                        }
                    }
                }
                if (c == 0)
                {
                    server->channels.push_back(Channel(this->args[i]));
                    server->channels[server->channels.size() - 1].setMode("+t");
                    if (this->keys.size() > 0)
                    {
                        server->channels[server->channels.size() - 1].setMode("+k");
                        server->channels[server->channels.size() - 1].setKey(this->keys[i]);
                    }
                }
            }
            if (c == 0)
            {
                addusertoChannel(server, this->args[i], 1);
            }
        }
    }
}
  
