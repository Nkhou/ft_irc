#include "command.hpp"

void Command::ParceCommand(std::vector<std::string> command, int fd, std::string hostname)
{
    for (unsigned long i = 1; i < command.size(); i++)
    {
        unsigned long j = 0;
        while (j < command[i].size())
        {
            // Skip commas
            if (command[i][j] == ',')
            {
                j++;
                continue;
            }

            unsigned long k = j;

            if (command[i][j] == '#')
            {
                if ((j + 1 < command[i].size()) && !((command[i][j + 1] >= 'a' && command[i][j + 1] <= 'z') || (command[i][j + 1] >= 'A' && command[i][j + 1] <= 'Z')))
                {
                    while (j < command[i].size() && command[i][j] != ',')
                        j++;
                    throw std::invalid_argument("Invalid argument****");
                }
                else
                {
                    j++; // Move past the '#'
                    while (j < command[i].size() && ((command[i][j] >= 'a' && command[i][j] <= 'z') || (command[i][j] >= 'A' && command[i][j] <= 'Z')))
                    {
                        j++;
                    }
                    if (j > k)
                    {
                        this->args.push_back(command[i].substr(k, j - k));
                    }
                }
            }
            else if (i != 1 )
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
            // else if (((command[i][j] >= 'a' && command[i][j] <= 'z') || (command[i][j] >= 'A' && command[i][j] <= 'Z')) && i != 1)
            // {
            //     while (j < command[i].size() && ((command[i][j] >= 'a' && command[i][j] <= 'z') || (command[i][j] >= 'A' && command[i][j] <= 'Z')))
            //     {
            //         j++;
            //     }
            //     if (j > k)
            //     {
            //         this->keys.push_back(command[i].substr(k, j - k));
            //     }
            // }
            // else if (command[i][j] != '\n' &&  i == 1)
            // {
            //     while (j < command[i].size() && command[i][j] != ',')
            //     {
            //         j++;
            //     }
            //     throw std::invalid_argument("argument+++");
            // }
            // {
            //     while (j < command[i].size() && command[i][j] != ',')
            //     {
            //         j++;
            //     }
            //     throw std::invalid_argument("argument+++");
            // }
            // else
            // {
            //     std::cout << "man naytak" << std::endl;
            // }
        }
    }
    this->fd = fd;
}

void Command::execCommand(server *server) {
    // Command cmd;
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
    // ParceCommand(server->splited, server->client_fd);
}

void Command::ParceCommandkick(std::vector<std::string> command, int fd, std::vector<Channel>channel, std::string hostname)
{
    for (unsigned long i = 1; i < command.size(); i++)
    {
        for (unsigned long j = 0; j < command[i].size(); j++)
        {
            unsigned long k = j;
            while (k < command.size() && command[i][k] != ',')
                k++;
            if (k > j)
            {
                if (i == 1)
                    args.push_back(command[i].substr(j, k));
                else if (i == 2)
                    keys.push_back(command[i].substr(j, k));
                else if (i == 3)
                    message = message + " " + command[i].substr(j,k);
            }
        }
    }
    for (unsigned long i = 1; i < channel.size(); i++)
    {        if (channel[i].getName() == command[1])

        {
            for (unsigned long j = 0; j < channel[i].getOperators().size(); j++)
            {
                if (channel[i].getOperators()[j].fd == fd)
                {
                    // channel[i] = 
                    return ;
                }
            }
            std::string msg = NotOPRT(args[0], hostname);
                if(send(fd, msg.c_str(), msg.length(), 0) < 0)
                {
                    std::cout << "Failed Send Try Again"<<std::endl;
                }
        
        }
    }
    this->fd = fd;
}
void Command::KickCommand(server *ser)
{
    for (unsigned long i = 0; i < ser->channels.size(); i++)
    {
        if (ser->channels[i].getName() == this->args[0])
        {
            for (unsigned long j = 0; j < ser->channels[i].getOperators().size(); j++)
            {
                if (ser->channels[i].getOperators()[j].fd == this->fd)
                {
                    for (unsigned long l = 1; l < ser->channels[i].getUsers().size(); l++)
                    {
                        if (ser->channels[i].getUsers()[l].nickname == this->keys[0])
                        {
                            ser->channels[i].removeUser(this->args[2]);
                            
                            std::string msg = ":" + ser->channels[i].getOperators()[j].nickname.substr(1) + " KICK " + this->args[0] + " " + this->keys[0] + " " + this->message + "\r\n";
                            if (send(fd, msg.c_str(), msg.length(), 0) < 0)
                            {
                                std::cout << "Failed Send Try Again"<<std::endl;
                            }
                            return;
                        }
                    }
                    return;
                }
            }
            std::string msg = NotOPRT(args[0], ser->hostname);
            if(send(fd, msg.c_str(), msg.length(), 0) < 0)
            {
                std::cout << "Failed Send Try Again //////"<<std::endl;
            }
            return;
        }
    }
}
void Command::ModeCommand(server *ser)
{
    if (this->args.size() == 0)
        return;
    if (ser->channels.size() == 0)
    {
        std::string msg = ChannelExist(args[0], ser->hostname);
        if(send(fd, msg.c_str(), msg.length(), 0) < 0)
        {
            std::cout << "Failed Send Try Again"<<std::endl;
        }
        return;
    }
    if (this->args.size() == 1)
    {
        for (unsigned long i = 0; i < ser->channels.size(); i++)
        {
            if (ser->channels[i].getName() == this->args[0])
            {
                if (ser->channels[i].getOperators().size() == 0)
                {
                    std::string msg = NotOPRT(args[0], ser->hostname);
                    if(send(fd, msg.c_str(), msg.length(), 0) < 0)
                    {
                        std::cout << "Failed Send Try Again"<<std::endl;
                    }
                    return;
                }
                for (unsigned long j = 0; j < ser->channels[i].getOperators().size(); j++)
                {
                    if (ser->channels[i].getOperators()[j].fd == this->fd)
                    {
                        ser->channels[i].setMode(this->args[1]);
                        return;
                    }
                }
                std::string msg = NotOPRT(args[0], ser->hostname);
                if(send(fd, msg.c_str(), msg.length(), 0) < 0)
                {
                    std::cout << "Failed Send Try Again"<<std::endl;
                }
                return;
            }
        }
        std::string msg = ChannelExist(args[0], ser->hostname);
        if(send(fd, msg.c_str(), msg.length(), 0) < 0)
        {
            std::cout << "Failed Send Try Again"<<std::endl;
        }
    }
    else if (this->args.size() == 2)
    {
        
        for (unsigned long i = 0; i < ser->channels.size(); i++)
        {
            if (ser->channels[i].getName() == this->args[0])
            {
                if (ser->channels[i].getOperators().size() == 0)
                {
                    std::string msg = NotOPRT(args[0], ser->hostname);
                if(send(fd, msg.c_str(), msg.length(), 0) < 0)
                {
                    std::cout << "Failed Send Try Again"<<std::endl;
                }
                    return;
                }
                for (unsigned long j = 0; j < ser->channels[i].getOperators().size(); j++)
                {
                    if (ser->channels[i].getOperators()[j].fd == this->fd)
                    {
                        ser->channels[i].setMode(this->args[1]);
                        return;
                    }
                }
                std::string msg = NotOPRT(args[0], ser->hostname);
                if(send(fd, msg.c_str(), msg.length(), 0) < 0)
                {
                    std::cout << "Failed Send Try Again"<<std::endl;
                }
                return;
            }
        }
        throw std::invalid_argument("Channel does not exist");
    }
    else if (this->args.size() == 3)
    {
        for (unsigned long i = 0; i < ser->channels.size(); i++)
        {
            if (ser->channels[i].getName() == this->args[0])
            {
                if (ser->channels[i].getOperators().size() == 0)
                {
                    std::string msg = NotOPRT(args[0], ser->hostname);
                if(send(fd, msg.c_str(), msg.length(), 0) < 0)
                {
                    std::cout << "Failed Send Try Again"<<std::endl;
                }
                    return;
                }
                for (unsigned long j = 0; j < ser->channels[i].getOperators().size(); j++)
                {
                    if (ser->channels[i].getOperators()[j].fd == this->fd)
                    {
                        if (this->args[1][0] == '+' && this->args[1][1] == 'l')
                        {
                            ser->channels[i].setMode(this->args[1]);
                            ser->channels[i].setMaxUsers(std::atoi(this->args[2].c_str()));
                            return;
                        }
                        else if (this->args[1][0] == '-' && this->args[1][1] == 'l')
                        {
                            ser->channels[i].setMode(this->args[1]);
                            ser->channels[i].setMaxUsers(-1);
                            return;
                        }
                        if (this->args[1][0] == '+' && this->args[1][1] == 't')
                        {
                            ser->channels[i].setMode(this->args[1]);
                            ser->channels[i].setTopic(this->args[2]);
                            return;
                        }
                        else if (this->args[1][0] == '-' && this->args[1][1] == 't')
                        {
                            ser->channels[i].setMode(this->args[1]);
                            ser->channels[i].setTopic("\0");
                            return;
                        }
                        if (this->args[1][0] == '+' && this->args[1][1] == 'k')
                        {
                            ser->channels[i].setMode(this->args[1]);
                            ser->channels[i].setKey(this->args[2]);
                            return;
                        }
                        else if (this->args[1][0] == '-' && this->args[1][1] == 'k')
                        {
                            ser->channels[i].setMode(this->args[1]);
                            ser->channels[i].setKey("\0");
                            return;
                        }
                        
                    }
                }
                std::string msg = NotOPRT(args[0], ser->hostname);
                if(send(fd, msg.c_str(), msg.length(), 0) < 0)
                {
                    std::cout << "Failed Send Try Again"<<std::endl;
                }
                return;
            }
            std::string msg = ChannelExist(args[0], ser->hostname);
            if(send(fd, msg.c_str(), msg.length(), 0) < 0)
            {
                std::cout << "Failed Send Try Again"<<std::endl;
            }
        }
    }
    else
    {
        for (unsigned long i = 0; i < ser->channels.size(); i++)
        {
            if (ser->channels[i].getName() == this->args[0])
            {
                if (ser->channels[i].getOperators().size() == 0)
                {
                    std::string msg = NotOPRT(args[0], ser->hostname);
                if(send(fd, msg.c_str(), msg.length(), 0) < 0)
                {
                    std::cout << "Failed Send Try Again"<<std::endl;
                }
                    return;
                }
                for (unsigned long j = 0; j < ser->channels[i].getOperators().size(); j++)
                {
                    if (ser->channels[i].getOperators()[j].fd == this->fd)
                    {
                        
                        if (this->args[1][0] == '+' && this->args[1][1] == 't')
                        {
                            ser->channels[i].setMode(this->args[1]);
                            for (unsigned long l = 2; l < ser->channels[i].getOperators().size(); l++)
                            {
                                
                            if (l == 2)
                                ser->channels[i].setTopic(this->args[l]); // need to add mode if not existe
                            else if (l > 1)
                                ser->channels[i].setTopic((ser->channels[i].getTopic() + this->args[l]));
                            }
                            return;
                        }
                        else if (this->args[1][0] == '-' && this->args[1][1] == 't')
                        {
                            ser->channels[i].setMode(this->args[1]);
                            ser->channels[i].setTopic("\0");
                            return;
                        }
                    }
                }
                std::string msg = NotOPRT(args[0], ser->hostname);
                if(send(fd, msg.c_str(), msg.length(), 0) < 0)
                {
                    std::cout << "Failed Send Try Again"<<std::endl;
                }
                if(send(fd, msg.c_str(), msg.length(), 0) < 0)
                {
                    std::cout << "Failed Send Try Again"<<std::endl;
                }
                return;
            }
            std::string msg = ChannelExist(args[0], ser->hostname);
            if(send(fd, msg.c_str(), msg.length(), 0) < 0)
            {
                std::cout << "Failed Send Try Again"<<std::endl;
            }
        }
    }
            
}
void Command::ParceModeCommand(std::vector <std::string> splited, int client_fd)
{
    if (splited.size() < 3)
        throw std::invalid_argument("Invalid argument*");
    for (unsigned long i = 1; i < splited.size(); i++)
    {
        if (splited[1][0] != '#' && splited[1][0] != '&' && splited[1][0] != '!')
        {
            throw std::invalid_argument("Invalid argument--------------------------------------------");
        }
        if (i == 1)
        {
            for (unsigned long j = 1; j < splited[1].size(); j++)
            {
                if (!((splited[i][j] >= 'a' && splited[i][j] <= 'z') || (splited[i][j] >= 'A' && splited[i][j] <= 'Z')  || (splited[i][j] == '_')))
                {
                    throw std::invalid_argument("Invalid argument++++++++++++++++++++++++++++++++++++++++++");
                }
            }
            this->args.push_back(splited[i]);
        }
        if (splited[i][0] == '+')
        {
            for (unsigned long j = 1; j < splited[i].size(); j++)
            {
                if (splited[i][j] == 'o' || splited[i][j] == 't' || splited[i][j] == 'i' || splited[i][j] == 'k' || splited[i][j] == 'l')
                {
                    this->args.push_back(splited[i]);
                }
                else
                {
                    throw std::invalid_argument("Invalid argument................................");
                }
            }
        }
        else if (splited[i][0] == '-')
        {
            for (unsigned long j = 1; j < splited[i].size(); j++)
            {
                if (splited[i][j] == 'o' || splited[i][j] == 't' || splited[i][j] == 'i' || splited[i][j] == 'k' || splited[i][j] == 'l')
                {
                    this->args.push_back(splited[i]);
                }
                else
                {
                    throw std::invalid_argument("Invalid argument~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
                }
            }
        }
        else if (i == 4)
        {
            this->args.push_back(splited[i]);
        }
        else if (splited[i][0] == '#')
        {
            this->args.push_back(splited[i]);
        }
        else if (splited[2] == "+t")
        {
            this->args.push_back(splited[i]);
        }
        else
        {
            throw std::invalid_argument("Invalid argument___________________________________________");
        }
    }
    this->fd = client_fd;
}
void Command::ParceTopic(std::vector <std::string> splited, int client_fd)
{
    for (unsigned long i = 1; i < splited.size(); i++)
    {
        this->args.push_back(splited[i]);
    }
    this->fd = client_fd;
}
void Command::TopicCommand(server *ser)
{
    for (unsigned long i = 0; i < ser->channels.size(); i++)
        {
            if (ser->channels[i].getName() == this->args[0])
            {
                if (ser->channels[i].getOperators().size() == 0)
                {
                    std::string msg = NotOPRT(args[0], ser->hostname);
                    if(send(fd, msg.c_str(), msg.length(), 0) < 0)
                    {
                        std::cout << "Failed Send Try Again"<<std::endl;
                    }
                    return;
                }
                for (unsigned long j = 0; j < ser->channels[i].getOperators().size(); j++)
                {
                    if (this->args.size() == 1)
                    {
                        if (ser->channels[i].getTopic() == "\0")
                        {
                            std::string msg = RPL_NOTOPIC(ser->hostname, ser->channels[i].getName());
                            if(send(fd, msg.c_str(), msg.length(), 0) < 0)
                            {
                                std::cout << "Failed Send Try Again"<<std::endl;
                            }
                            return;
                        }
                        ser->channels[i].PrintTopic(this->fd);
                    }
                    if (ser->channels[i].getOperators()[j].fd == this->fd)
                    {
                        for (unsigned long l = 1; l < this->args.size(); l++)
                        {
                            if ( l == 1)
                                ser->channels[i].setTopic(this->args[l]);
                            else
                                ser->channels[i].setTopic((ser->channels[i].getTopic() + " " +this->args[l]));
                        }
                            std::string msg = ":" + ser->channels[i].getOperators()[j].nickname.substr(1) + " TOPIC " + this->args[0] + " \"" + ser->channels[i].getTopic() +"\"" + "\r\n";
                            if (send(fd, msg.c_str(), msg.length(), 0) < 0)
                            {
                                std::cout << "Failed Send Try Again"<<std::endl;
                            }
                        return;
                    }
                }
                std::string msg = NotOPRT(args[0], ser->hostname);
                if(send(fd, msg.c_str(), msg.length(), 0) < 0)
                {
                    std::cout << "Failed Send Try Again"<<std::endl;
                }
                return;
            }
        }
        throw std::invalid_argument("Channel does not exist");
}
void Command::InviteCommand(server *ser)
{
    if (this->args.size() == 0)
        return ;
    if (this->args[1][0] != '#' || this->args[1][0] != '&')
        return ;
    for (unsigned long i = 0; i < ser->channels.size(); i++)
    {
        if (ser->channels[i].getName() == this->args[1])
        {

                if (ser->channels[i].getOperators().size() == 0)
                {
                    std::string msg = NotOPRT(args[0], ser->hostname);
                if(send(fd, msg.c_str(), msg.length(), 0) < 0)
                {
                    std::cout << "Failed Send Try Again"<<std::endl;
                }
                    return;
                }
                for (unsigned long j = 0; j < ser->channels[i].getOperators().size(); j++)
                {
                    if (ser->channels[i].getOperators()[j].fd == this->fd)
                    {
                        for (unsigned long l = 0; l < ser->clients.size(); l++)
                        {
                            if (this->args[0] == ser->clients[l].nickname)
                            {
                                for (unsigned long c = 0; c < ser->channels[i].getUsers().size(); c++)
                                {
                                    if (ser->channels[i].getUsers()[c].nickname == ser->clients[l].nickname)
                                        return ;
                                }
                                ser->channels[i].addUser(ser->clients[l], 0);
                                return ;
                            }
                        }
                        return;
                    }
                }
        }
    }
}
void Command::ParceInvite(std::vector <std::string> splited, int client_fd)
{
    for (unsigned long i = 1; i < splited.size(); i++)
    {
        this->args.push_back(splited[i]);
    }
     this->fd = client_fd;
}
void Command::ParcePrivmsg(std::vector <std::string> splited, int client_fd)
{
     this->fd = client_fd;
    for (unsigned long i = 1; i < splited.size(); i++) 
    {
        this->args.push_back(splited[i]);
    }
     this->fd = client_fd;
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
void Command::PrivmsgCommand(server *ser)
{
    if (this->args.size() == 0)
        return ;
    if (ser->channels.size() == 0)
    {
        for (unsigned long j = 0; j < ser->clients.size(); j++)
            {
                if (ser->clients[j].nickname == this->args[0])
                {
                    std::string msg;
                    if (this->args[2][0] != ':')
                         msg = ":" + getClientByFd(ser, this->fd)->nickname + " PRIVMSG " + ser->clients[j].nickname + " :";
                    for (unsigned long l = 1; l < this->args.size(); l++)
                    {
                        msg += sendMessage(ser->clients[j].nickname, ser->splited[0], this->args[l]);
                        if (l < this->args.size() - 1)
                            msg += " ";
                    }
                    msg += "\r\n";
                    if(send(ser->clients[j].fd, msg.c_str(), msg.length(), 0) < 0)
                    {
                        std::cout << "Failed Send Try Again"<<std::endl;
                    }
                    return ;
                }
            }
            std::string msg = msg_errpriv(ser->splited[0], ser->hostname);
            if(send(ser->client_fd, msg.c_str(), msg.length(), 0) < 0)
            {
                std::cout << "Failed Send Try Again"<<std::endl;
            }
            return ;
    }
    for (unsigned long i = 0; i < ser->channels.size(); i++)
    {
        if (ser->channels[i].getName() == this->args[0])
        {
            for (unsigned long j = 0; j < ser->channels[i].getUsers().size(); j++)
            {
                    std::string msg;
                    if (this->args[2][0] != ':')
                         msg = ":" + getClientByFd(ser, this->fd)->nickname + " PRIVMSG " + ser->channels[i].getUsers()[j].nickname + " :";
                    for (unsigned long l = 1; l < this->args.size(); l++)
                    {
                        msg += sendMessage(ser->clients[j].nickname, ser->splited[0], this->args[l]);
                        if (l < this->args.size() - 1)
                            msg += " ";
                    }
                    msg += "\r\n";
                if (ser->channels[i].getUsers()[j].fd != this->fd)
                {
                    if(send(ser->channels[i].getUsers()[j].fd, msg.c_str(), msg.length(), 0) < 0)
                    {
                        std::cout << "Failed Send Try Again"<<std::endl;
                    }
                    // return ;
                }
            }
            if (ser->channels[i].getUsers().size() == 0)
            {
                std::string msg = msg_errpriv(ser->splited[0], ser->hostname);
                if(send(ser->client_fd, msg.c_str(), msg.length(), 0) < 0)
                {
                    std::cout << "Failed Send Try Again"<<std::endl;
                }
            }
            // std::string msg = msg_errpriv(ser->splited[0], ser->hostname);
            // if(send(ser->client_fd, msg.c_str(), msg.length(), 0) < 0)
            // {
            //     std::cout << "Failed Send Try Again"<<std::endl;
            // }
        }
        else
        {
            
            for (unsigned long j = 0; j < ser->clients.size(); j++)
            {
                if (ser->clients[j].nickname == this->args[0])
                {
                    std::string msg;
                    if (this->args[2][0] != ':')
                         msg = ":" + getClientByFd(ser, this->fd)->nickname + " PRIVMSG " + ser->clients[j].nickname + " :";
                    for (unsigned long l = 1; l < this->args.size(); l++)
                    {
                        msg += sendMessage(ser->clients[j].nickname, ser->splited[0], this->args[l]);
                        if (l < this->args.size() - 1)
                            msg += " ";
                    }
                    msg += "\r\n";
                    if(send(ser->clients[j].fd, msg.c_str(), msg.length(), 0) < 0)
                    {
                        std::cout << "Failed Send Try Again"<<std::endl;
                    }
                    return ;
                }
            }
            std::string msg = msg_errpriv(ser->splited[0], ser->hostname);
            if(send(ser->client_fd, msg.c_str(), msg.length(), 0) < 0)
            {
                std::cout << "Failed Send Try Again"<<std::endl;
            }

        }
    }
}
void Command::executecmd(server *server) {
    if (server->splited[0] == "JOIN") // need to check nickename exist befor need to check if channel have keys
    {
        if (server->splited.size() != 2 && server->splited.size() != 3)
        {
            std::string msg = msg_err(server->splited[0], server->hostname);
            if(send(server->client_fd, msg.c_str(), msg.length(), 0) < 0)
            {
                std::cout << "Failed Send Try Again"<<std::endl;
            }
        }
        ParceCommand(server->splited, server->client_fd, server->hostname);

        JoinCommand(server);
    }
    else if (server->splited[0] == "KICK")
    {
        if (server->splited.size() < 3)
        {
            std::string msg = msg_err(server->splited[0], server->hostname);
            if(send(server->client_fd, msg.c_str(), msg.length(), 0) < 0)
            {
                std::cout << "Failed Send Try Again"<<std::endl;
            }
        }
        ParceCommandkick(server->splited, server->client_fd, server->channels, server->hostname);
        KickCommand(server);
    }
    else if (server->splited[0] == "MODE")
    {
        ParceModeCommand(server->splited, server->client_fd);
        // std::cout << "hello --------------- " <<std::endl;
        ModeCommand(server);
    }
    else if (server->splited[0] == "TOPIC")
    {
        if (server->splited.size() < 2)
        {
            std::string msg = msg_err(server->splited[0], server->hostname);
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
        ParceInvite(server->splited, server->client_fd);
        InviteCommand(server);
    }
    else if (server->splited[0] == "PRIVMSG")
    {
        // std::cout << "PRIVMSG" << std::endl;
        ParcePrivmsg(server->splited, server->client_fd);
        for (unsigned long i = 0; i < this->args.size(); i++)
        {
            std::cout << "Args: " << this->args[i] << std::endl;
        }
        PrivmsgCommand(server);
    }
    // else
    // {
    //     std::string msg = msg_err(server->splited[0], server->hostname);
    //     if(send(server->client_fd, msg.c_str(), msg.length(), 0) < 0)
    //     {
    //         std::cout << "Failed Send Try Again"<<std::endl;
    //     }
    // }
    this->args.clear();
    // std::cout << "------- "<<this->args.size() << std::endl;
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
                        return;
                    }
                    server->channels[i].notifyUserJoin(server->clients[j].nickname);
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
                    if (checkMode(server->channels[i].getMode(), "+i") == 1)
                    {
                        throw std::invalid_argument("Channel is invite only");
                        return ;
                    }
                    else if (checkMode(server->channels[i].getMode(), "+k") == 1 && this->keys.size() > 0 && server->channels[i].getKey() != this->keys[0])
                    {
                        throw std::invalid_argument("Channel is key protectedddddddd");
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
                    // else if (server->channels[i].getKey().size() > 0 && server->channels[i].getKey() != this->keys[0])
                    // {
                    //     throw std::invalid_argument("Channel is key protected!!!!!!!!!");
                    //     return ;
                    // }
                    // else if (server->channels[i].getKey().size() > 0 && server->channels[i].getKey() == this->keys[0])
                    // {
                    //     addusertoChannel(server, this->args[i], 0);
                    //     return ;
                    // }
                    // else if (server->channels[i].getKey().size() == 0 && this->keys.size() == 0)
                    // {
                    //     addusertoChannel(server, this->args[i], 0);
                    //     return ;
                    // }
                    // else if (server->channels[i].getKey().size() == 0 && this->keys.size() > 0)
                    // {
                    //     throw std::invalid_argument("Channel is key protected------");
                    //     return ;
                    // }
                    // else if (server->channels[i].getKey().size() > 0 && this->keys.size() == 0)
                    // {
                    //     throw std::invalid_argument("Channel is key protected+++++++++");
                    //     return ;
                    // }
                    // else if (server->channels[i].getKey().size() > 0 && server->channels[i].getKey() != this->keys[0])
                    // {
                    //     throw std::invalid_argument("Channel is key protected~~~~~~~~~~~~~~");
                    //     return ;
                    // }
                    // else if (server->channels[i].getKey().size() == 0 && this->keys.size() > 0)
                    // {
                    //     throw std::invalid_argument("Channel is key protected*****************");
                    //     return ;
                    // }
                    // else if (server->channels[i].getKey().size() == 0 && this->keys.size() == 0)
                    // {
                    //     addusertoChannel(server, this->args[i], 0);
                    //     return ;
                    // }
                    // else if (server->channels[i].getKey().size() > 0 && this->keys.size() > 0)
                    // {
                    //     if (server->channels[i].getKey() != this->keys[0])
                    //     {
                    //         throw std::invalid_argument("Channel is key protected[[[[]]]]");
                    //         return ;
                    //     }
                    // }
                    // else if (server->channels[i].getKey().size() == 0 && this->keys.size() > 0)
                    // {
                    //     throw std::invalid_argument("Channel is key protected$$$$$$$$$$$$");
                    //     return ;
                    // }
                    // else if (server->channels[i].getKey().size() > 0 && this->keys.size() == 0)
                    // {
                    //     throw std::invalid_argument("Channel is key protected§§§§§§§§§§§§§§§§");
                    //     return ;
                    // }
                    addusertoChannel(server, this->args[i], 0);
                    return ;
                }
            }
            server->channels.push_back(Channel(this->args[0]));
            if (this->keys.size() > 0)
            {
                server->channels[server->channels.size() - 1].setKey(this->keys[0]);
                server->channels[server->channels.size() - 1].setMode("+k");
            }
        }
        addusertoChannel(server, this->args[0], 1);
    }
    else
    {
        
        for (unsigned long i = 0; i < this->args.size(); i++)
        {
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
                        if (checkMode(server->channels[j].getMode(), "+i") == 1)
                        { 
                            throw std::invalid_argument("Channel is invite only");
                            return;
                        }
                        else if (checkMode(server->channels[j].getMode(), "+k") == 1 && i <= this->keys.size() && server->channels[j].getKey() != this->keys[i])
                        {
                            if (server->channels[j].getKey() != this->args[i + 1]) //need more check and fix this
                            {
                                throw std::invalid_argument("Channel is key protected");
                                return;
                            }
                        }
                        else if (server->channels[i].getKey().size() > 0 && i <= this->keys.size() && server->channels[i].getKey() != this->keys[i])
                    {
                        throw std::invalid_argument("Channel is key protected");
                        return ;
                    }
                    else if (server->channels[i].getKey().size() > 0 && i <= this->keys.size() && server->channels[i].getKey() == this->keys[i])
                    {
                        addusertoChannel(server, this->args[i], 0);
                        return ;
                    }
                    else if (server->channels[i].getKey().size() == 0 && this->keys.size() == 0)
                    {
                        addusertoChannel(server, this->args[i], 0);
                        return ;
                    }
                    else if (server->channels[i].getKey().size() == 0 && this->keys.size() > 0)
                    {
                        throw std::invalid_argument("Channel is key protected");
                        return ;
                    }
                    else if (server->channels[i].getKey().size() > 0 && this->keys.size() == 0)
                    {
                        throw std::invalid_argument("Channel is key protected");
                        return ;
                    }
                    else if (server->channels[i].getKey().size() > 0 && i <= this->keys.size() && server->channels[i].getKey() != this->keys[i])
                    {
                        throw std::invalid_argument("Channel is key protected");
                        return ;
                    }
                    else if (server->channels[i].getKey().size() == 0 && this->keys.size() > 0)
                    {
                        throw std::invalid_argument("Channel is key protected");
                        return ;
                    }
                    else if (server->channels[i].getKey().size() == 0 && this->keys.size() == 0)
                    {
                        addusertoChannel(server, this->args[i], 0);
                        return ;
                    }
                    else if (server->channels[i].getKey().size() > 0 && this->keys.size() > 0)
                    {
                        if (i <= this->keys.size() && server->channels[i].getKey() != this->keys[i])
                        {
                            throw std::invalid_argument("Channel is key protected");
                            return ;
                        }
                    }
                    else if (server->channels[i].getKey().size() == 0 && this->keys.size() > 0)
                    {
                        throw std::invalid_argument("Channel is key protected");
                        return ;
                    }
                    else if (server->channels[i].getKey().size() > 0 && this->keys.size() == 0)
                    {
                        throw std::invalid_argument("Channel is key protected");
                        return ;
                    }
                        addusertoChannel(server, this->args[i], 0);
                        return;
                    }
                }
                server->channels.push_back(Channel(this->args[i]));
                if (this->keys.size() > 0)
                {
                    server->channels[server->channels.size() - 1].setMode("+k");
                    server->channels[server->channels.size() - 1].setKey(this->keys[i]);
                }
            }
            addusertoChannel(server, this->args[i], 1);
        }
    }
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++