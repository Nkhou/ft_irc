#include "command.hpp"

void Command::ParceCommand(std::vector<std::string> command, int fd, std::string hostname)
{
    for (unsigned long i = 1; i < command.size(); i++)
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
        }
    }
    this->fd = fd;
}

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

void Command::ParceCommandkick(std::vector<std::string> command, int fd, std::vector<Channel>channel, std::string hostname)
{
    (void)channel;
    if (command.size() < 3)
    {
        std::string msg = kickerr(command[0], hostname);
        if(send(fd, msg.c_str(), msg.length(), 0) < 0)
        {
            std::cout << "Failed Send Try Again"<<std::endl;
        }
        return;
    }
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
                j = k;
                // else if (i == 3)
                //     message = message + " " + command[i].substr(j,k);
            }
        }
    }
    // for (size_t i = 0 ; i < this->args.size(); i++ )
    //     std::cout << "ha anna kant3awd  "<<this->args[i] << std::endl;
    // for (size_t i = 0 ; i < this->keys.size(); i++ )
    //     std::cout << "ha anna kant3awd keys "<<this->keys[i] << std::endl;
    if (command.size() == 4)
    {
        if (command[3][0] == ':')
            this->message = command[3].substr(1);
        else
            this->message = command[3];
    }
    // for (unsigned long i = 1; i < channel.size(); i++)
    // {
    //     if (channel[i].getName() == command[1])
    //     {
    //         for (unsigned long j = 0; j < channel[i].getOperators().size(); j++)
    //         {
    //             if (channel[i].getOperators()[j].fd == fd)
    //             {
    //                 // channel[i] = 
    //                 return ;
    //             }
    //         }
    //         std::string msg = NotOPRT(args[0], hostname);
    //         if(send(fd, msg.c_str(), msg.length(), 0) < 0)
    //         {
    //             std::cout << "Failed Send Try Again"<<std::endl;
    //         }
        
    //     }
    // }
    this->fd = fd;
}
void Command::KickCommand(server *ser)
{
    int existchannel = 0;
    int op = 0;
    int exist = 0;
    if (this->args.size() == 0)
        return;
    for (size_t l = 0; l < this->args.size() ; l++)
    {
        existchannel = 0;
        for (unsigned long i = 0; i < ser->channels.size(); i++)
        {
            op = 0;

            if (ser->channels[i].getName() == this->args[l])
            {
                existchannel = 1;
                for (unsigned long j = 0; j < ser->channels[i].getOperators().size(); j++)
                {
                    if (ser->channels[i].getOperators()[j].fd == this->fd)
                    {
                        // std::cout << "hello" << std::endl;
                        op = 1;
                        for (size_t c = 0; c < this->keys.size(); c++)
                        {
                            for (unsigned long o = 0; o < ser->channels[i].getUsers().size(); o++)
                            {
                                if (ser->channels[i].getUsers()[o].nickname == this->keys[c])
                                {
                                    exist = 1;
                                    std::string msg = ":" +ser->channels[i].getOperators()[j].nickname.substr(1) + "~!"+ ser->channels[i].getOperators()[j].user_name +"@" + ser->hostname +" KICK " + this->args[l] + " " + this->keys[c];
                                    if (this->message != "")
                                        msg = msg + " :" + this->message + "\r\n";
                                    else
                                        msg = msg + "\r\n";
                                    + " " + this->message + "\r\n";
                                    for (size_t k = 0; k < ser->channels[i].getUsers().size(); k++)
                                    {
                                        // if (ser->channels[i].getUsers()[k].nickname == this->keys[0])
                                        // {
                                            if (send(ser->channels[i].getUsers()[k].fd, msg.c_str(), msg.length(), 0) < 0)
                                            {
                                                std::cout << "Failed Send Try Again"<<std::endl;
                                            }
                                            // ser->channels[i].removeUser(this->keys[0]);
                                            // return;
                                        // }
                                    }
                                    // if (send(fd, msg.c_str(), msg.length(), 0) < 0)
                                    // {
                                    //     std::cout << "Failed Send Try Again"<<std::endl;
                                    // }
                                    ser->channels[i].removeUser(this->keys[c]);
                                    if (ser->channels[i].getUsers().size() == 0)
                                    {
                                        ser->channels.erase(ser->channels.begin() + i);
                                    }
                                    // break;
                                    // return;
                                }
                            }
                            if (exist == 0)
                            {
                                std::string msg = ":" + ser->hostname + " 441 " + ser->channels[i].getOperators()[j].nickname.substr(1) + " " + this->keys[0] + " " + this->args[l] + " :They aren't on that channel\r\n";
                                if (send(fd, msg.c_str(), msg.length(), 0) < 0)
                                {
                                    std::cout << "Failed Send Try Again"<<std::endl;
                                }
                                // return;
                            }
                        }
                            // return;
                    }
                }
                if (op == 0)
                {
                    std::string msg = NotOPRT(args[l], ser->hostname);
                    if(send(fd, msg.c_str(), msg.length(), 0) < 0)
                    {
                        std::cout << "Failed Send Try Again"<<std::endl;
                    }
                    // return;
                }
                // if (existchannel == 0)
                // {
                //     std::string msg = NotOPRT(args[l], ser->hostname);
                //     if(send(fd, msg.c_str(), msg.length(), 0) < 0)
                //     {
                //         std::cout << "Failed Send Try Again"<<std::endl;
                //     }
                //     return;
                // }
                    // std::string msg = NotOPRT(args[0], ser->hostname);
                    // if(send(fd, msg.c_str(), msg.length(), 0) < 0)
                    // {
                    //     std::cout << "Failed Send Try Again //////"<<std::endl;
                    // }
                    // return;
            }
        }

        if (existchannel == 0)
        {
            std::string msg = ERR_NOSUCHCHANNEL(args[l], ser->hostname);
            if(send(fd, msg.c_str(), msg.length(), 0) < 0)
            {
                std::cout << "Failed Send Try Again"<<std::endl;
            }
            // return;
        }
    }
    // std::string msg = ERR_NOSUCHCHANNEL(args[0], ser->hostname);
    // if(send(fd, msg.c_str(), msg.length(), 0) < 0)
    // {
    //     std::cout << "Failed Send Try Again"<<std::endl;
    // }
}
void Command::ModeCommand(server *ser)
{
    if (this->args.size() == 0)
        return;
    
    for (size_t i = 0; i < ser->channels.size(); i++)
    {
        if (ser->channels[i].getName() == this->args[0])
        {
            if (ser->channels[i].getUser(this->fd) == NULL)
            {
                std::string msg = ERR_NOTONCHANNEL(args[0], ser->hostname);
                if(send(fd, msg.c_str(), msg.length(), 0) < 0)
                {
                    std::cout << "Failed Send Try Again"<<std::endl;
                }
                return;
            }
        }
    }
    for (size_t p = 0; p < this->keys.size(); p++)
    {
        std::cout << "---args : " << this->keys[p] << std::endl;
    }
    if (ser->channels.size() == 0)
    {
        std::string msg = ChannelExist(args[0], ser->hostname);
        if(send(fd, msg.c_str(), msg.length(), 0) < 0)
        {
            std::cout << "Failed Send Try Again"<<std::endl;
        }
        return;
    }
    if (ser->splited.size() == 2)
    {
        // std::cout << "hello1111" << std::endl;
        for (unsigned long i = 0; i < ser->channels.size(); i++)
        {
            if (ser->channels[i].getName() == this->args[0])
            {
                std::string msg = ":" +ser->client_cmd.nickname + "!~" + ser->client_cmd.user_name + "@" + ser->hostname + " MODE " + this->args[0] + " ";
                // std::string msg = ":" + ser->hostname + " 324 " + ser->channels[i].getName() + " ";
                for (unsigned long j = 0; j < ser->channels[i].getMode().size(); j++)
                {
                        msg = msg + ser->channels[i].getMode()[j] + " ";
                }
                for (unsigned long j = 0; j < ser->channels[i].getMode().size(); j++)
                {
                        if (ser->channels[i].getMode()[j] == "+l")
                        {
                            msg = msg + std::to_string(ser->channels[i].getMaxUsers()) + " ";
                        }
                        else if (ser->channels[i].getMode()[j] == "+k")
                        {
                            msg = msg + ser->channels[i].getKey() + " ";
                        }
                        else if (ser->channels[i].getMode()[j] == "+t")
                        {
                            msg = msg + ser->channels[i].getTopic() + " ";
                        }
                }
                // if (ser->channels[i].getMode().size() == 0)
                // {
                //    msg += "t\r\n";

                // }
                ser->channels[i].sendMessage(msg.substr(0, msg.size() - 1) + "\r\n");
                return;
            }
        }
        std::string msg = ChannelExist(args[0], ser->hostname);
        if(send(fd, msg.c_str(), msg.length(), 0) < 0)
        {
            std::cout << "Failed Send Try Again"<<std::endl;
        }
    }
    else if (ser->splited.size() == 3)
    {
        // std::cout << "hello" << std::endl;
        for (unsigned long i = 0; i < ser->channels.size(); i++)
        {
            if (ser->channels[i].getName() == this->args[0])
            {
                for (unsigned long j = 0; j < ser->channels[i].getOperators().size(); j++)
                {
                    if (ser->channels[i].getOperators()[j].fd == this->fd)
                    {
                        for (size_t o = 0; o < this->keys.size(); o++)
                        {
                            std::cout << "hello2" << this->keys[o]<< std::endl;
                            if (this->keys[o] == "+i" && !ser->channels[i].checkModeexist(ser->channels[i], "+i"))
                            {
                                ser->channels[i].setMode(this->keys[o]);
                                ser->channels[i].setInviteOnly(1);
                                std::string msg = ":" + (ser->channels[i].getOperators()[j].nickname.substr(1)  +"!~"+ser->channels[i].getOperators()[j].user_name + "@"+ser->hostname + " MODE " + this->args[0] + " " + this->keys[o] ) + "\r\n";
                                ser->channels[i].sendMessage(msg);
                            }
                            else if (this->keys[o] == "-i")
                            {
                                if (ser->channels[i].checkModeexist(ser->channels[i], "+i") )
                                {
                                    // std::cout << "keys : " << ser->channels[i].checkModeexist(ser->channels[i], "+i") << std::endl;
                                    ser->channels[i].setMode(this->keys[o]);
                                    ser->channels[i].setInviteOnly(0);
                                    std::string msg = ":" + (ser->channels[i].getOperators()[j].nickname.substr(1)  +"!~"+ser->channels[i].getOperators()[j].user_name + "@"+ser->hostname + " MODE " + this->args[0] + " " + this->keys[o] ) + "\r\n";
                                    ser->channels[i].sendMessage(msg);
                                }
                            }
                            else if (this->keys[o] == "-t" && ser->channels[i].checkModeexist(ser->channels[i], "+t"))
                            {
                                    ser->channels[i].setMode(this->keys[o]);
                                    ser->channels[i].setTopic("\0");
                                    std::string msg = ":" + (ser->channels[i].getOperators()[j].nickname.substr(1)  +"!~"+ser->channels[i].getOperators()[j].user_name + "@"+ser->hostname + " MODE " + this->args[0] + " " + this->keys[o] ) + "\r\n";
                                    ser->channels[i].sendMessage(msg);
                            }
                            else if (this->keys[o] == "+t" && !ser->channels[i].checkModeexist(ser->channels[i], "+t"))
                            {
                                    ser->channels[i].setMode(this->keys[o]);
                                    // ser->channels[i].setTopic("\0");
                                    std::string msg = ":" + (ser->channels[i].getOperators()[j].nickname.substr(1)  +"!~"+ser->channels[i].getOperators()[j].user_name + "@"+ser->hostname + " MODE " + this->args[0] + " " + this->keys[o] ) + "\r\n";
                                    ser->channels[i].sendMessage(msg);
                            }
                            else if (this->keys[o] == "+t" || this->keys[o] == "+l" || this->keys[o] == "+o" || this->keys[o] == "+k")
                            {
                                std::string msg = notenghparam(args[0], ser->hostname);
                                if(send(fd, msg.c_str(), msg.length(), 0) < 0)
                                {
                                    std::cout << "Failed Send Try Again"<<std::endl;
                                }
                            }
                            else if (this->keys[o] == "-k" && ser->channels[i].checkModeexist(ser->channels[i], "+k"))
                            {
                                ser->channels[i].setMode(this->keys[o]);
                                ser->channels[i].setKey("\0");
                                std::string msg = ":" + (ser->channels[i].getOperators()[j].nickname.substr(1)  +"!~"+ser->channels[i].getOperators()[j].user_name + "@"+ser->hostname + " MODE " + this->args[0] + " " + this->keys[o] ) + "\r\n";
                                ser->channels[i].sendMessage(msg);
                            }
                            else if (this->keys[o] == "-l" && ser->channels[i].checkModeexist(ser->channels[i], "+l"))
                            {
                                ser->channels[i].setMode(this->keys[o]);
                                ser->channels[i].setLimits(0);
                                ser->channels[i].setMaxUsers(-1);
                                std::string msg = ":" + (ser->channels[i].getOperators()[j].nickname.substr(1)  +"!~"+ser->channels[i].getOperators()[j].user_name + "@"+ser->hostname + " MODE " + this->args[0]  + " " + this->keys[o]) + "\r\n";
                                ser->channels[i].sendMessage(msg);
                                // return;
                            }
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
        std::string msg = ERR_NOSUCHCHANNEL(args[0], ser->hostname);
        if(send(fd, msg.c_str(), msg.length(), 0) < 0)
        {
            std::cout << "Failed Send Try Again"<<std::endl;
        }
    }
    else 
    {
        std::cout << "hello2" << std::endl;
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
                        for (size_t o = 0; o < this->keys.size(); o++)
                        {
                            if (this->keys[o] == "+i" && !ser->channels[i].checkModeexist(ser->channels[i], "+i"))
                            {
                                ser->channels[i].setMode(this->keys[o]);
                                ser->channels[i].setInviteOnly(1);
                                std::string msg = ":" + (ser->channels[i].getOperators()[j].nickname.substr(1)  +"!~"+ser->channels[i].getOperators()[j].user_name + "@"+ser->hostname + " MODE " + this->args[0] + " " + this->keys[o] ) + "\r\n";
                                ser->channels[i].sendMessage(msg);
                            }
                            else if (this->keys[o] == "-i")
                            {
                                if (ser->channels[i].checkModeexist(ser->channels[i], "+i") )
                                {
                                    // std::cout << "keys : " << ser->channels[i].checkModeexist(ser->channels[i], "+i") << std::endl;
                                    ser->channels[i].setMode(this->keys[o]);
                                    ser->channels[i].setInviteOnly(0);
                                    std::string msg = ":" + (ser->channels[i].getOperators()[j].nickname.substr(1)  +"!~"+ser->channels[i].getOperators()[j].user_name + "@"+ser->hostname + " MODE " + this->args[0] + " " + this->keys[o] ) + "\r\n";
                                    ser->channels[i].sendMessage(msg);
                                }
                            }
                            else if (this->keys[o] == "+l" && ((o + 1) < this->args.size()))
                            {
                                size_t d = 0;
                                size_t z = 0;
                                while (d  < keys.size())
                                {
                                    if (this->keys[d] == "+i" || this->keys[d] == "+t")
                                        z++;
                                    if (this->keys[d] == "+l")
                                        break;
                                    d++;
                                }
                                size_t c = 0;
                                // std::cout << "args : " << this->args[o + 1 - z] << std::endl;
                                while (c  < this->args[o + 1 - z].length())
                                {
                                    if (this->args[o + 1 - z][c] < '0' || this->args[o + 1 - z][c] > '9')
                                        break;
                                    c++;
                                }
                                if (c ==  this->args[o + 1 - z].size())
                                {
                                    if (std::atoi(this->args[o + 1 - z].c_str()) > 0)
                                    {
                                        ser->channels[i].setMode(this->keys[o]);
                                        ser->channels[i].setLimits(1);
                                        ser->channels[i].setMaxUsers(std::atoi(this->args[o + 1 - z].c_str()));
                                        std::string msg = ":" +(ser->channels[i].getOperators()[j].nickname.substr(1)  +"!~"+ser->channels[i].getOperators()[j].user_name + "@"+ser->hostname + " MODE " + this->args[0] + " " + this->keys[o] + " " + this->args[o + 1 - z]) + "\r\n";
                                        ser->channels[i].sendMessage(msg);
                                    }
                                }
                            } 
                            else if (this->keys[o] == "+o" && ((o + 1) < this->args.size()))
                            {
                                size_t d = 0;
                                size_t z = 0;
                                while (d  < keys.size())
                                {
                                    if (this->keys[d] == "+i" || this->keys[d] == "+t")
                                        z++;
                                    if (this->keys[d] == "+o")
                                        break;
                                    d++;
                                }
                                for (unsigned long l = 1; l < ser->channels[i].getUsers().size(); l++)
                                {
                                    if (ser->channels[i].getUsers()[l].nickname == this->args[o + 1 - z])
                                    {
                                        size_t c = 0;
                                        for (unsigned long k = 0; k < ser->channels[i].getOperators().size(); k++)
                                        {
                                            if (ser->channels[i].getOperators()[k].nickname == this->args[o + 1 - z])
                                                break ;
                                            c++;
                                        }
                                        if (c != 0 && c == ser->channels[i].getOperators().size())
                                        {
                                            ser->channels[i].addOperator(ser->channels[i].getUsers()[l]);
                                            std::string msg = ":" + (ser->channels[i].getOperators()[j].nickname.substr(1)  +"!~"+ser->channels[i].getOperators()[j].user_name + "@"+ser->hostname + " MODE " + this->args[0] + " " + this->keys[o] + " " + this->args[o + 1 - z])+ "\r\n";
                                            ser->channels[i].sendMessage(msg);
                                        }
                                    }
                                }
                            }
                            else if (this->keys[o] == "-o")
                            {
                                // ser->channels[i].setMode(this->keys[o]);
                                for (unsigned long l = 1; l < ser->channels[i].getUsers().size(); l++)
                                {
                                    if (ser->channels[i].getUsers()[l].nickname == this->args[1])
                                    {
                                        size_t c = 0;
                                        for (unsigned long k = 0; k < ser->channels[i].getOperators().size(); k++)
                                        {
                                            if (ser->channels[i].getOperators()[k].nickname == this->args[1])
                                                break ;
                                            c++;
                                        }
                                        if (c < ser->channels[i].getOperators().size())
                                        {
                                            ser->channels[i].removeOperator(ser->channels[i].getUsers()[l].nickname);
                                            std::string msg = ":" + (ser->channels[i].getOperators()[j].nickname.substr(1)  +"!~"+ser->channels[i].getOperators()[j].user_name + "@"+ser->hostname + " MODE " + this->args[0] + " " + this->keys[o] + " " + this->args[1]) + "\r\n";
                                            ser->channels[i].sendMessage(msg);
                                        }
                                        // else 
                                        // {
                                        //     std::string msg = ERR_NOTOPERATOR(ser->channels[i].getOperators()[j].nickname.substr(1), ser->hostname);
                                        //     if(send(fd, msg.c_str(), msg.length(), 0) < 0)
                                        //     {
                                        //         std::cout << "Failed Send Try Again"<<std::endl;
                                        //     }
                                        //     break ;
                                        // }
                                    }
                                }

                            }
                            else if (this->keys[o] == "-l" && ser->channels[i].checkModeexist(ser->channels[i], "+l"))
                            {
                                // std::cout << "hello" << std::endl;
                                ser->channels[i].setMode(this->keys[o]);
                                ser->channels[i].setLimits(0);
                                ser->channels[i].setMaxUsers(-1);
                                std::string msg = ":" + (ser->channels[i].getOperators()[j].nickname.substr(1)  +"!~"+ser->channels[i].getOperators()[j].user_name + "@"+ser->hostname + " MODE " + this->args[0] + " " + keys[o]) + "\r\n";
                                ser->channels[i].sendMessage(msg);
                                // return;
                            }
                            else if (this->keys[o] == "+t" &&  ((o + 1) < this->args.size()))
                            {
                                // size_t d = 0;
                                // int z = 0;
                                // while (d  < keys.size())
                                // {
                                //     if (this->keys[d] == "+i")
                                //         z++;
                                //     if (this->keys[d] == "+t")
                                //         break;
                                //     d++;
                                // }
                                ser->channels[i].setMode(this->keys[o]);
                                // ser->channels[i].setTopic(this->args[o + 1 - z]);
                                std::string msg = ":" + (ser->channels[i].getOperators()[j].nickname.substr(1)  +"!~"+ser->channels[i].getOperators()[j].user_name + "@"+ser->hostname + " MODE " + this->args[0] ) + "\r\n";
                                ser->channels[i].sendMessage(msg);
                                // return;
                            }
                            else if (this->keys[o] == "-t" && ser->channels[i].checkModeexist(ser->channels[i], "+t"))
                            {
                                ser->channels[i].setMode(this->keys[o]);
                                ser->channels[i].setTopic("\0");
                                std::string msg = ":" + (ser->channels[i].getOperators()[j].nickname.substr(1)  +"!~"+ser->channels[i].getOperators()[j].user_name + "@"+ser->hostname + " MODE " + this->args[0] )+ "\r\n";
                                ser->channels[i].sendMessage(msg);
                                // return;
                            }
                            else if (this->keys[o] == "+k" &&  ((o + 1) < this->args.size())) //delete key
                            {
                                size_t d = 0;
                                int z = 0;
                                while (d  < keys.size())
                                {
                                    if (this->keys[d] == "+i" || this->keys[d] == "+t")
                                        z++;
                                    if (this->keys[d] == "+k")
                                        break;
                                    d++;
                                }
                                if (!ser->channels[i].checkModeexist(ser->channels[i], "+k"))
                                {
                                    // std::cout << "hello " << this->keys[o]<< std::endl;
                                    ser->channels[i].setMode(this->keys[o]);
                                    ser->channels[i].setKey(this->args[o + 1 - z]);
                                    std::string msg = ":" + (ser->channels[i].getOperators()[j].nickname.substr(1)  +"!~"+ser->channels[i].getOperators()[j].user_name + "@"+ser->hostname + " MODE " + this->args[0] +" " + this->keys[o] +" "+ args[o + 1 - z])+   "\r\n";
                                    ser->channels[i].sendMessage(msg); 
                                }
                                else
                                {
                                    std::string msg = ERR_KEYSET(ser->channels[i].getOperators()[j].nickname.substr(1), ser->hostname);
                                    if(send(fd, msg.c_str(), msg.length(), 0) < 0)
                                    {
                                        std::cout << "Failed Send Try Again"<<std::endl;
                                    }
                                }
                            }
                            else if (this->keys[o] == "-k" && ser->channels[i].checkModeexist(ser->channels[i], "+k"))
                            {
                                ser->channels[i].setMode(this->keys[o]);
                                ser->channels[i].setKey("\0");
                                std::string msg = ":" + (ser->channels[i].getOperators()[j].nickname.substr(1)  +"!~"+ser->channels[i].getOperators()[j].user_name + "@"+ser->hostname + " MODE " + this->args[0]  + this->keys[o]) + "\r\n";
                                ser->channels[i].sendMessage(msg);
                                
                            }

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
            std::string msg = ChannelExist(args[0], ser->hostname);
            if(send(fd, msg.c_str(), msg.length(), 0) < 0)
            {
                std::cout << "Failed Send Try Again"<<std::endl;
            }
            return;
        }
    }
void Command::ParceModeCommand(std::vector <std::string> splited, int client_fd)
{
    if (splited.size() < 2)
        return;
    
    for (unsigned long i = 1; i < splited.size(); i++)
    {
        // if ()
        // if (i == 1)

        if (i != 2)
            this->args.push_back(splited[i]);
        else if (splited[2][0] == '+' || splited[2][0] == '-')
        {
            for (size_t j = 1; j < splited[2].length(); j++)
            {
                std::string g = "";
                g += splited[2][0];
                g += splited[2][j];
                this->keys.push_back(g);
            }
        }
        else {
            this->args.push_back(splited[i]);
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
    for (size_t i = 0; i < ser->channels.size(); i++)
    {
        if (ser->channels[i].getUserfd(this->fd)== 0)
        {
            std::string msg = ERR_NOTONCHANNEL(args[0], ser->hostname);
            if(send(fd, msg.c_str(), msg.length(), 0) < 0)
            {
                std::cout << "Failed Send Try Again"<<std::endl;
            }
            return;
        }
    }
    for (unsigned long i = 0; i < ser->channels.size(); i++)
        {
            if (ser->channels[i].getName() == this->args[0])
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
                    return;
                }
                else 
                {
                    if (ser->channels[i].checkModeexist(ser->channels[i], "+t"))
                    {
                        for (unsigned long j = 0; j < ser->channels[i].getOperators().size(); j++)
                        {
                            if (ser->channels[i].getOperators()[j].fd == this->fd)
                            {
                                ser->channels[i].setTopic(this->args[1]);
                                std::string msg = ":" + ser->channels[i].getOperators()[j].nickname.substr(1) + "!~" + ser->channels[i].getOperators()[j].user_name + "@" + ser->hostname + " TOPIC " + this->args[0] + " :" + this->args[1] + "\r\n";
                                // std::string msg = ":" + ser->channels[i].getOperators()[j].nickname.substr(1) + +" TOPIC " + this->args[0] + " :" + ser->channels[i].getTopic() + "\r\n";
                                ser->channels[i].sendMessage(msg);
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
                    else
                    {
                        ser->channels[i].setTopic(this->args[1]);
                        std::string msg = ":" + ser->client_cmd.nickname + "!~" + ser->client_cmd.user_name + "@" + ser->hostname + " TOPIC " + this->args[0] + " :" + this->args[1] + "\r\n";
                        // std::string msg = ":" + ser->channels[i].getOperators()[j].nickname.substr(1) + +" TOPIC " + this->args[0] + " :" + ser->channels[i].getTopic() + "\r\n";
                        ser->channels[i].sendMessage(msg);
                        return;
                    }
                }
                // return;
            }
        }
        std::string msg = ERR_NOSUCHCHANNEL(args[0], ser->hostname);
        if (send(fd, msg.c_str(), msg.length(), 0) < 0)
        {
            std::cout << "Failed Send Try Again"<<std::endl;
        }
        // throw std::invalid_argument("Channel does not exist");
}
void Command::InviteCommand(server *ser)
{
    if (this->args.size() == 0)
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
                    if (ser->channels[i].getUsers()[j].fd == this->fd)
                    {
                        // std::cout << "args: " << this->args[0] << std::endl;
                        // std::cout << "args: " << this->args[0] << std::endl;
                        for (unsigned long l = 0; l < ser->clients.size(); l++)
                        {
                            if (this->args[0] == ser->clients[l].nickname)
                            {
                                for (unsigned long c = 0; c < ser->channels[i].getUsers().size(); c++)
                                {
                                    if (ser->channels[i].getUsers()[c].nickname == ser->clients[l].nickname)
                                        return ;
                                }
                                ser->channels[i].setFd(ser->clients[l].fd);
                                // ser->channels[i].addUser(ser->clients[l], 0);
                                std::string msg = ":" + ser->channels[i].getOperators()[j].nickname.substr(1) +"!~"+ser->channels[i].getOperators()[j].user_name + "@"+ser->hostname + " INVITE " + this->args[0] + " " + this->args[1] + "\r\n";
                                ser->channels[i].sendMessage(msg);
                                if (send(ser->clients[l].fd, msg.c_str(), msg.length(), 0) < 0)
                                {
                                    std::cout << "Failed Send Try Again"<<std::endl;
                                }
                                return ;
                            }
                        }
                        std::string msg = msg_errpriv(this->args[1], ser->hostname);
                        if(send(ser->client_fd, msg.c_str(), msg.length(), 0) < 0)
                        {
                            std::cout << "Failed Send Try Again"<<std::endl;
                        }
                        return;
                    }
                }
        }
    }
    std::string msg = msg_errpriv(this->args[1], ser->hostname);
    if(send(ser->client_fd, msg.c_str(), msg.length(), 0) < 0)
    {
        std::cout << "Failed Send Try Again"<<std::endl;
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
void Command::ParcePrivmsg(std::vector <std::string> command, int client_fd)
{
    //  this->fd = client_fd;
    // for (unsigned long i = 1; i < command.size(); i++)
    // {
        int i = 1;
        for (unsigned long j = 0; j < command[i].size(); j++)
        {
            unsigned long k = j;
            while (k < command[i].size() && command[i][k] != ',')
                k++;
            if (k > j)
            {
                // if (i == 1)
                    args.push_back(command[i].substr(j, k));
                // else if (i == 2)
                //     keys.push_back(command[i].substr(j, k));
                j = k;
                // else if (i == 3)
                //     message = message + " " + command[i].substr(j,k);
            }
        }
    // }
    if (command.size() == 3)
        this->message = command[command.size() - 1];
    else
        this->message = command[i];
    // for (unsigned long j = 0; j < this->args.size(); j++)
    // {
    //     std::cout << "args: " << this->args[j] << std::endl;
    // }
    // for (unsigned long i = 1; i < splited.size(); i++) 
    // {
    //     // std::cout << "splited[i]: " << splited[i] << std::endl;
    //     // std::cout << "size: " << splited[].size() << std::endl;
    //     this->args.push_back(splited[i]);
    // }
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
    // std::cout << "args[0]: " << this->args[0] << std::endl; 
    int flag = 0;
    if (ser->channels.size() == 0)
    {
        // std::cout << "No channels" << std::endl;
        for (size_t o = 0; o < this->args.size(); o++)
        {
            for (unsigned long j = 0; j < ser->clients.size(); j++)
            {
                if (ser->clients[j].nickname == this->args[o])
                {
                    flag = 1;
                    std::string msg;
                    if (this->args.size() > 0 && this->args[o][0] != ':')
                         msg = ":" + getClientByFd(ser, this->fd)->nickname + "!~"+ getClientByFd(ser, this->fd)->user_name +"@"+ser->hostname+" PRIVMSG " + ser->clients[j].nickname + " :";
                    else if (this->args.size() > 0 && this->args[o][0] == ':')
                        msg = ":" + getClientByFd(ser, this->fd)->nickname + "!~"+ getClientByFd(ser, this->fd)->user_name +"@"+ser->hostname+" PRIVMSG " + ser->clients[j].nickname + " ";
                    // std::cout << "args: " << std::endl;
                    msg += sendMessage(ser->clients[j].nickname, ser->splited[0], this->message);
                            msg += "\r\n";
                    if(send(ser->clients[j].fd, msg.c_str(), msg.length(), 0) < 0)
                    {
                        std::cout << "Failed Send Try Again"<<std::endl;
                    }
                    // return ;
                }
            }
            if (flag == 0)
            {
                std::string msg = msg_errpriv(ser->splited[0], ser->hostname);
                if(send(ser->client_fd, msg.c_str(), msg.length(), 0) < 0)
                {
                    std::cout << "Failed Send Try Again"<<std::endl;
                }
            };
        }
        return ;
    }
        for (size_t o = 0; o < this->args.size(); o++)
        {
            flag = 0;
            if (this->args[o][0] == '#')
            {
                for (unsigned long i = 0; i < ser->channels.size(); i++)
                {
                    if (ser->channels[i].getName() == this->args[o])
                    {
                            flag = 1;
                        int c = 0;
                        for (size_t j = 0; j < ser->channels[i].getUsers().size(); j++)
                        {
                            if (ser->channels[i].getUsers()[j].fd == this->fd)
                            {
                                c = 1;
                                break;
                            }
                        }
                        if (c == 1)
                        {
                            for (unsigned long j = 0; j < ser->channels[i].getUsers().size(); j++)
                            {
                                std::string msg;
                                if (this->args.size() > 1 && this->args[o][0] != ':')
                                         msg = ":" + getClientByFd(ser, this->fd)->nickname + "!~" + getClientByFd(ser, this->fd)->user_name + "@" + ser->hostname + " PRIVMSG " + ser->channels[i].getUsers()[j].nickname + " :";
                                else if (this->args.size() > 1 && this->args[o][0] == ':')
                                    msg = ":" + getClientByFd(ser, this->fd)->nickname + "!~" + getClientByFd(ser, this->fd)->user_name + "@" + ser->hostname + " PRIVMSG " + ser->channels[i].getUsers()[j].nickname + " ";
                                msg += sendMessage(ser->clients[j].nickname, ser->splited[0], this->message);
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
                        }
                        else
                        {
                            std::string msg = ERR_NOTONCHANNEL(ser->splited[0], ser->hostname);
                            if(send(ser->client_fd, msg.c_str(), msg.length(), 0) < 0)
                            {
                                std::cout << "Failed Send Try Again"<<std::endl;
                            }
                        }
                    // if (ser->channels[i].getUsers().size() == 0)
                    // {
                    //     std::string msg = msg_errpriv(ser->splited[0], ser->hostname);
                    //     if(send(ser->client_fd, msg.c_str(), msg.length(), 0) < 0)
                    //     {
                    //         std::cout << "Failed Send Try Again"<<std::endl;
                    //     }
                    // }
                    // std::string msg = msg_errpriv(ser->splited[0], ser->hostname);
                    // if(send(ser->client_fd, msg.c_str(), msg.length(), 0) < 0)
                    // {
                    //     std::cout << "Failed Send Try Again"<<std::endl;
                    // }
                    // return ;
                }
            }
        }
            else
            {
                for (unsigned long j = 0; j < ser->clients.size(); j++)
                {
                    if (ser->clients[j].nickname == this->args[o])
                    {
                        flag = 1;
                        std::string msg;
                        if (this->args.size() > 0 && this->args[o][0] != ':')
                             msg = ":" + getClientByFd(ser, this->fd)->nickname + "!~"+ getClientByFd(ser, this->fd)->user_name +"@"+ser->hostname+" PRIVMSG " + ser->clients[j].nickname + " :";
                        else if (this->args.size() > 0 && this->args[o][0] == ':')
                            msg = ":" + getClientByFd(ser, this->fd)->nickname + "!~"+ getClientByFd(ser, this->fd)->user_name +"@"+ser->hostname+" PRIVMSG " + ser->clients[j].nickname + " ";
                        msg += sendMessage(ser->clients[j].nickname, ser->splited[0], this->message);
                            //     if (l < this->args.size() - 1)
                            //         msg += " ";
                            // }
                            msg += "\r\n";
                        if(send(ser->clients[j].fd, msg.c_str(), msg.length(), 0) < 0)
                        {
                            std::cout << "Failed Send Try Again"<<std::endl;
                        }
                        // return ;
                    }
                }
                }
                if (flag == 0)
                {
                    std::string msg = msg_errpriv(ser->splited[0], ser->hostname);
                    if(send(ser->client_fd, msg.c_str(), msg.length(), 0) < 0)
                    {
                        std::cout << "Failed Send Try Again"<<std::endl;
                    }
                }
            }
            

        // }
}
void Command::nick_auth(std::vector<std::string> split,server *server,int client_fd)
{
    (void)split;
    (void)client_fd;

     if( server->splited.size() < 2)
     {
       std::string msg = message_err_nick_name(server->hostname, ERR_NONICKNAMEGIVEN_CODE,"*", server->client_cmd.nickname, ERR_NONICKNAMEGIVEN_MSG);
             if(send(server->client_cmd.fd, msg.c_str(), msg.length(), 0) < 0)
                 std::cout << "Failed Send Try Again"<<std::endl;
     }
     if(server->splited.size() >= 2)
     {
        int c = 0;
            
    std::string  msg = ":" + server->client_cmd.nickname +"!~"+server->client_cmd.user_name+"@"+server->hostname+ " NICK " + server->splited[1] + "\r\n";
    std::cout << server->splited.size() << std::endl;
    if(server->splited.size() >= 2)
    {
        for (unsigned long i = 0; i < server->channels.size(); i++)
        {
           for (unsigned long j = 0; j < server->channels[i].getUsers().size(); j++)
           {
               if(server->channels[i].getUsers()[j].nickname == server->client_cmd.nickname)
               {
                    c = 1;
                   server->channels[i].getUsers()[j].nickname = server->splited[1];
                   for (unsigned long k = 0; k < server->channels[i].getOperators().size(); k++)
                   {
                       if(server->channels[i].getOperators()[k].nickname == server->client_cmd.nickname)
                       {
                           server->channels[i].getOperators()[k].nickname = server->splited[1];
                       }
                   }
                   server->channels[i].sendMessagenick(msg, server->client_cmd.fd);
               }
           }
        }
        // if (c == 0)
        if(send(server->client_cmd.fd, msg.c_str(), msg.length(), 0) < 0)
            std::cout << "Failed Send Try Again"<<std::endl;
        server->client_cmd.nickname= server->splited[1];
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
    std::cout << "pass_auth ***********" << std::endl;
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
    }
    else if(server->splited[0] == "USER")
    { 
        std::cout << "USER6777777" << std::endl;
        user_auth(server->splited,server,server->client_fd);
    }
    else if(server->splited[0] == "PASS")
    {
         pass_auth(server);
         return;
    }
    else if (server->splited[0] == "QUIT")
    {
        std::cout << "QUIT" << std::endl;
        for (unsigned long i = 0; i < server->clients.size(); i++)
        {
            if (server->clients[i].fd == server->client_fd)
            {
                server->clients.erase(server->clients.begin() + i);
                break;
            }
        }
        for (unsigned long i = 0; i < server->channels.size(); i++)
        {
            for (unsigned long j = 0; j < server->channels[i].getUsers().size(); j++)
            {
                if (server->channels[i].getUsers()[j].fd == server->client_fd)
                {
                    server->channels[i].removeUser(server->channels[i].getUsers()[j].nickname);
                    break;
                }
            }
        }
        for (unsigned long i = 0; i < server->channels.size(); i++)
        {
            for (unsigned long j = 0; j < server->channels[i].getOperators().size(); j++)
            {
                if (server->channels[i].getOperators()[j].fd == server->client_fd)
                {
                    server->channels[i].removeOperator(server->channels[i].getOperators()[j].nickname);
                    break;
                }
            }
        }
        close(server->client_fd);
        return;
    }

    if (server->splited[0] == "JOIN") // need to check nickename exist befor need to check if channel have keys
    {
        if (server->splited.size() != 2 && server->splited.size() != 3)
        {
            std::string msg = msg_err(server->splited[0], server->hostname);
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
            for (unsigned long i = 0; i < server->clients.size(); i++)
            {
                if (server->clients[i].fd == server->client_fd)
                {
                    std::string msg = kickerr(server->clients[i].nickname, server->hostname);
                    if(send(server->client_fd, msg.c_str(), msg.length(), 0) < 0)
                    {
                        std::cout << "Failed Send Try Again"<<std::endl;
                    }
                    return;
                }
            }
            // std::string msg = kickerr(server->splited[0], server->hostname);
            // if(send(server->client_fd, msg.c_str(), msg.length(), 0) < 0)
            // {
            //     std::cout << "Failed Send Try Again"<<std::endl;
            // }
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
            std::string msg = msg_err(server->splited[0], server->hostname);
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
        // std::cout << server->splited[2][0]<<"************" << std::endl;
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
            // std::string msg = ":" + ser + splited[2] + " :is unknown mode char to me\r\n";
        }
        ParceModeCommand(server->splited, server->client_fd);
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
        if (server->splited.size() < 3)
        {
            std::string msg = msg_err(server->splited[0], server->hostname);
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
        if (server->splited.size() == 1)
        {
            std::string msg = msg_errsend(server->splited[0], server->hostname);
            if(send(server->client_fd, msg.c_str(), msg.length(), 0) < 0)
            {
                std::cout << "Failed Send Try Again"<<std::endl;
            }
            return;
        }
        else if (server->splited.size() == 2)
        {
            std::string msg = msg_err(server->splited[0], server->hostname);
            if(send(server->client_fd, msg.c_str(), msg.length(), 0) < 0)
            {
                std::cout << "Failed Send Try Again"<<std::endl;
            }
            return;
        }
        // std::cout << "PRIVMSG" << std::endl;
        ParcePrivmsg(server->splited, server->client_fd);
        
        // for (unsigned long i = 0; i < this->args.size(); i++)
        // {
        //     std::cout << "Args: " << this->args[i] << std::endl;
        // }
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
                        // std::cout << "He" << std::endl;
                        server->channels[i].removefd(server->client_fd);
                        return;
                    }
                    server->channels[i].notifyUserJoin(server->clients[j].nickname, server->hostname, o);
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
void Command::JoinCommand(server *server) {
    if (this->args.size() == 0)
        return;
    if (this->args.size() == 1)
    {
        if (server->channels.size() == 0){
            // server->channels[0].setMaxUsers(SIZE_MAX);
            server->channels.push_back(Channel(this->args[0]));
            // server->channels[0].setMode("+t");
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
                        // throw std::invalid_argument("Channel is invite only");
                        return ;
                    }
                    else if (checkMode(server->channels[i].getMode(), "+k") == 1 && this->keys.size() > 0 && server->channels[i].getKey() != this->keys[0])
                    {
                        if (server->channels[i].getKey() != this->args[1] || checkMode(server->channels[i].getMode(), "+k") == 1) //need more check and fix this
                        {
                            std::string msg = ERR_BADCHANNELKEY(args[0], server->hostname);
                            if (send(fd, msg.c_str(), msg.length(), 0) < 0)
                            {
                                std::cout << "Failed Send Try Again"<<std::endl;
                            }
                            // throw std::invalid_argument("Channel is key protected");
                            return ;
                        }
                        // throw std::invalid_argument("Channel is key protectedddddddd");
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
            server->channels.push_back(Channel(this->args[0])); // add topic 
            server->channels[server->channels.size() - 1].setMode("+k");
            if (this->keys.size() > 0)
            {
                server->channels[server->channels.size() - 1].setKey(this->keys[0]);
                server->channels[server->channels.size() - 1].setMode("+k");
            }
        }
        std::cout << "Here" << std::endl;
        std::cout << "args: " << this->args[0] << std::endl;
        addusertoChannel(server, this->args[0], 1);
        return ;
        // removefd(server->client_fd);
    }
    else
    {
        
        for (unsigned long i = 0; i < this->args.size(); i++)
        {
            int c = 0;
            // for (unsigned long j = 0; j < server->channels[i].getUsers().size(); j++)
            //         {
            //             if (server->channels[i].getUsers()[j].fd == this->fd)
            //             {
            //                 return ;
            //             }
            //         }
            if (server->channels.size() == 0)
            {
                server->channels.push_back(Channel(this->args[i]));
                server->channels[0].setMode("+t");
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
                                // return;
                            }
                            else if (checkMode(server->channels[j].getMode(), "+k") == 1 && i <= this->keys.size() && server->channels[j].getKey() != this->keys[i])
                            {
                                c = 1;
                                if (server->channels[j].getKey() != this->args[i + 1]) //need more check and fix this
                                {
                                     std::string msg = ERR_BADCHANNELKEY(args[0], server->hostname);
                                    if (send(fd, msg.c_str(), msg.length(), 0) < 0)
                                    {
                                        std::cout << "Failed Send Try Again"<<std::endl;
                                    }
                                    // throw std::invalid_argument("Channel is key protected");
                                    // return;
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
                                // throw std::invalid_argument("Channel is key protected");
                                // return ;
                            }
                            else if (server->channels[i].getLimits() && server->channels[j].getUsers().size() + 1 > server->channels[j].getMaxUsers())
                            {
                                c = 1;
                                std::string msg = ERR_CHANNELISFULL(args[0], server->hostname);
                                if (send(fd, msg.c_str(), msg.length(), 0) < 0)
                                {
                                    std::cout << "Failed Send Try Again"<<std::endl;
                                }
                                // return ;
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
  