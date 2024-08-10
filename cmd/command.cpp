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
    // std::cout << this->args.size() << std::endl;
    // for (unsigned long i = 0 ; i < this->args.size(); i++)
    // {
    //     std::cout << this->args[i] << std::endl;
    // }
    // return ;
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
                        std::string msg = ":" + ser->hostname + " 324 " + ser->channels[i].getName() + " ";
                for (unsigned long j = 0; j < ser->channels[i].getMode().size(); j++)
                {
                        msg = msg + ser->channels[i].getMode()[j] + " ";
                        if(send(fd, msg.c_str(), msg.length(), 0) < 0)
                        {
                            std::cout << "Failed Send Try Again"<<std::endl;
                        }
                }
                if (ser->channels[i].getMode().size() == 0)
                {
                   msg += "t\r\n";

                }
                ser->channels[i].sendMessage(msg);
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
                for (unsigned long j = 0; j < ser->channels[i].getOperators().size(); j++)
                {
                    if (ser->channels[i].getOperators()[j].fd == this->fd)
                    {
                        if (this->args[1] == "+i")
                        {
                            ser->channels[i].setMode(this->args[1]);
                            ser->channels[i].setInviteOnly(1);
                            std::string msg = (ser->channels[i].getOperators()[j].nickname.substr(1)  +"!~"+ser->channels[i].getOperators()[j].user_name + "@"+ser->hostname + " MODE " + this->args[0] + " " + this->args[1] ) + "\r\n";
                            ser->channels[i].sendMessage(msg);
                            return;
                        }
                        else if (this->args[1] == "-i")
                        {
                            if (ser->channels[i].checkModeexist(ser->channels[i], "+i"))
                            {
                                ser->channels[i].setMode(this->args[1]);
                                ser->channels[i].setInviteOnly(0);
                                std::string msg = (ser->channels[i].getOperators()[j].nickname.substr(1)  +"!~"+ser->channels[i].getOperators()[j].user_name + "@"+ser->hostname + " MODE " + this->args[0] + " " + this->args[1] ) + "\r\n";
                                ser->channels[i].sendMessage(msg);
                                return;
                            }
                            else
                            {
                                // std::string msg = notenghparam(args[0], ser->hostname);
                                // if(send(fd, msg.c_str(), msg.length(), 0) < 0)
                                // {
                                //     std::cout << "Failed Send Try Again"<<std::endl;
                                // }
                                return;
                            }
                            // ser->channels[i].setMode(this->args[1]);
                            // ser->channels[i].setInviteOnly(0);
                            // std::string msg = (ser->channels[i].getOperators()[j].nickname.substr(1)  +"!~"+ser->channels[i].getOperators()[j].user_name + "@"+ser->hostname + " MODE " + this->args[0] + " " + this->args[1] ) + "\r\n";
                            // ser->channels[i].sendMessage(msg);
                            return;
                        }
                        else if (this->args[1] == "-t")
                        {
                            if (ser->channels[i].checkModeexist(ser->channels[i], "+t"))
                            {
                                ser->channels[i].setMode(this->args[1]);
                                ser->channels[i].setTopic("\0");
                                std::string msg = (ser->channels[i].getOperators()[j].nickname.substr(1)  +"!~"+ser->channels[i].getOperators()[j].user_name + "@"+ser->hostname + " MODE " + this->args[0] + " " + this->args[1] ) + "\r\n";
                                ser->channels[i].sendMessage(msg);
                                return;
                            }
                            else
                            {
                                // std::string msg = notenghparam(args[0], ser->hostname);
                                // if(send(fd, msg.c_str(), msg.length(), 0) < 0)
                                // {
                                //     std::cout << "Failed Send Try Again"<<std::endl;
                                // }
                                return;
                            }
                            // ser->channels[i].setMode(this->args[1]);
                            // ser->channels[i].setTopic("\0");
                            // std::string msg = (ser->channels[i].getOperators()[j].nickname.substr(1)  +"!~"+ser->channels[i].getOperators()[j].user_name + "@"+ser->hostname + " MODE " + this->args[0] + " " + this->args[1] ) + "\r\n";
                            // if(send(fd, msg.c_str(), msg.length(), 0) < 0)
                            // {
                            //     std::cout << "Failed Send Try Again"<<std::endl;
                            // }
                            return;
                        }
                        else if (this->args[1] == "+t")
                        {
                            if (ser->channels[i].checkModeexist(ser->channels[i], "+t"))
                            {
                                ser->channels[i].setMode(this->args[1]);
                                ser->channels[i].setTopic(this->args[2]);
                                std::string msg = (ser->channels[i].getOperators()[j].nickname.substr(1)  +"!~"+ser->channels[i].getOperators()[j].user_name + "@"+ser->hostname + " MODE " + this->args[0] + " " + this->args[1] + " " + this->args[2]) + "\r\n";
                                ser->channels[i].sendMessage(msg);
                                return;
                            }
                            else
                            {
                                // std::string msg = notenghparam(args[0], ser->hostname);
                                // if(send(fd, msg.c_str(), msg.length(), 0) < 0)
                                // {
                                //     std::cout << "Failed Send Try Again"<<std::endl;
                                // }
                                return;
                            }
                            // ser->channels[i].setMode(this->args[1]);
                            // ser->channels[i].setTopic(this->args[2]);
                            // std::string msg = (ser->channels[i].getOperators()[j].nickname.substr(1)  +"!~"+ser->channels[i].getOperators()[j].user_name + "@"+ser->hostname + " MODE " + this->args[0] + " " + this->args[1] ) + "\r\n";
                            // ser->channels[i].sendMessage(msg);
                            return;
                        }
                        else if (this->args[1] == "-k" && ser->channels[i].checkModeexist(ser->channels[i], "+k"))
                        {
                            ser->channels[i].setMode(this->args[1]); // need to add mode if not existe
                            ser->channels[i].setKey("\0");
                            std::string msg = (ser->channels[i].getOperators()[j].nickname.substr(1)  +"!~"+ser->channels[i].getOperators()[j].user_name + "@"+ser->hostname + " MODE " + this->args[0] + " " + this->args[1] ) + "\r\n";
                            ser->channels[i].sendMessage(msg);
                            return;
                        }
                        else if (this->args[1] == "-k" && this->args.size() > 2)
                        {
                            //need to check in command and return   
                            return ;
                        }
                        else if (this->args[1] == "+k")
                        {
                            // std::cout << "hhhhhhhhh" << std::endl;
                            if (this->args.size() > 2)
                            {
                                ser->channels[i].setMode(this->args[1]);
                                ser->channels[i].setKey(this->args[2]);
                                std::string msg = (ser->channels[i].getOperators()[j].nickname.substr(1)  +"!~"+ser->channels[i].getOperators()[j].user_name + "@"+ser->hostname + " MODE " + this->args[0] + " " + this->args[1] + " " + this->args[2]) + "\r\n";
                                ser->channels[i].sendMessage(msg);
                                return;
                            }
                            else
                            {
                                // std::string msg = notenghparam(args[0], ser->hostname);
                                // if(send(fd, msg.c_str(), msg.length(), 0) < 0)
                                // {
                                //     std::cout << "Failed Send Try Again"<<std::endl;
                                // }
                                return;
                            }
                            // ser->channels[i].setMode(this->args[1]);
                            // std::string msg = (ser->channels[i].getOperators()[j].nickname.substr(1)  +"!~"+ser->channels[i].getOperators()[j].user_name + "@"+ser->hostname + " MODE " + this->args[0] + " " + this->args[1] ) + "\r\n";
                            // ser->channels[i].sendMessage(msg);
                            return;
                        }
                        else if (this->args[1] == "-l")
                        {
                            ser->channels[i].setMode(this->args[1]);
                            ser->channels[i].setMaxUsers(-1);
                            std::string msg = (ser->channels[i].getOperators()[j].nickname.substr(1)  +"!~"+ser->channels[i].getOperators()[j].user_name + "@"+ser->hostname + " MODE " + this->args[0] + " " + this->args[1]) + "\r\n";
                            ser->channels[i].sendMessage(msg);
                            return;
                        }
                        else
                        {
                            std::string msg = notenghparam(args[0], ser->hostname);
                            if(send(fd, msg.c_str(), msg.length(), 0) < 0)
                            {
                                std::cout << "Failed Send Try Again"<<std::endl;
                            }
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
        }
        std::string msg = ERR_NOSUCHCHANNEL(args[0], ser->hostname);
        if(send(fd, msg.c_str(), msg.length(), 0) < 0)
        {
            std::cout << "Failed Send Try Again"<<std::endl;
        }
        // throw std::invalid_argument("Channel does not exist");
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
                        if (this->args[1] == "+l")
                        {
                            ser->channels[i].setMode(this->args[1]);
                            ser->channels[i].setMaxUsers(std::atoi(this->args[2].c_str()));
                            std::string msg = (ser->channels[i].getOperators()[j].nickname.substr(1)  +"!~"+ser->channels[i].getOperators()[j].user_name + "@"+ser->hostname + " MODE " + this->args[0] + " " + this->args[1] + " " + this->args[2]) + "\r\n";
                            ser->channels[i].sendMessage(msg);
                            return;
                        } 
                        else if (this->args[1] == "+o")
                        {
                            ser->channels[i].setMode(this->args[1]);
                            for (unsigned long l = 1; l < ser->channels[i].getUsers().size(); l++)
                            {
                                if (ser->channels[i].getUsers()[l].nickname == this->args[2])
                                {
                                    ser->channels[i].addOperator(ser->channels[i].getUsers()[l]);
                                    std::string msg = (ser->channels[i].getOperators()[j].nickname.substr(1)  +"!~"+ser->channels[i].getOperators()[j].user_name + "@"+ser->hostname + " MODE " + this->args[0] + " " + this->args[1] + " " + this->args[2]);
                                    ser->channels[i].sendMessage(msg);
                                    return;
                                }
                            }
                            if (ser->channels[i].getUsers()[0].nickname == this->args[2])
                                return ;
                            std::string msg = msg_errpriv(args[0], ser->hostname);
                            if(send(fd, msg.c_str(), msg.length(), 0) < 0)
                            {
                                std::cout << "Failed Send Try Again"<<std::endl;
                            }
                            return;
                        }
                        else if (this->args[1] == "-o")
                        {
                            ser->channels[i].setMode(this->args[1]);
                            for (unsigned long l = 1; l < ser->channels[i].getUsers().size(); l++)
                            {
                                if (ser->channels[i].getUsers()[l].nickname == this->args[2])
                                {
                                    ser->channels[i].removeOperator(ser->channels[i].getUsers()[l].nickname);
                                    ser->channels[i].addOperator(ser->channels[i].getUsers()[l]);
                                    std::string msg = (ser->channels[i].getOperators()[j].nickname.substr(1)  +"!~"+ser->channels[i].getOperators()[j].user_name + "@"+ser->hostname + " MODE " + this->args[0] + " " + this->args[1] + " " + this->args[2]) + "\r\n";
                                    ser->channels[i].sendMessage(msg);
                                    return;
                                }
                            }
                            if (ser->channels[i].getUsers()[0].nickname == this->args[2])
                                return ;
                            std::string msg = msg_errpriv(args[0], ser->hostname);
                            if(send(fd, msg.c_str(), msg.length(), 0) < 0)
                            {
                                std::cout << "Failed Send Try Again"<<std::endl;
                            }
                            return;
                        }
                        else if (this->args[1] == "-l" )
                        {
                            ser->channels[i].setMode(this->args[1]);
                            ser->channels[i].setMaxUsers(-1);
                            std::string msg = (ser->channels[i].getOperators()[j].nickname.substr(1)  +"!~"+ser->channels[i].getOperators()[j].user_name + "@"+ser->hostname + " MODE " + this->args[0] ) + "\r\n";
                            ser->channels[i].sendMessage(msg);
                            return;
                        }
                        if (this->args[1] == "+t")
                        {
                            ser->channels[i].setMode(this->args[1]);
                            ser->channels[i].setTopic(this->args[2]);
                            std::string msg = (ser->channels[i].getOperators()[j].nickname.substr(1)  +"!~"+ser->channels[i].getOperators()[j].user_name + "@"+ser->hostname + " MODE " + this->args[0] + this->args[2]) + "\r\n";
                            ser->channels[i].sendMessage(msg);
                            return;
                        }
                        else if (this->args[1] == "-t")
                        {
                            ser->channels[i].setMode(this->args[1]);
                            ser->channels[i].setTopic("\0");
                            std::string msg = (ser->channels[i].getOperators()[j].nickname.substr(1)  +"!~"+ser->channels[i].getOperators()[j].user_name + "@"+ser->hostname + " MODE " + this->args[0] )+ "\r\n";
                            ser->channels[i].sendMessage(msg);
                            return;
                        }
                        if (this->args[1] == "+k" ) //delete key
                        {
                            ser->channels[i].setMode(this->args[1]);
                            ser->channels[i].setKey(this->args[2]);
                            std::string msg = (ser->channels[i].getOperators()[j].nickname.substr(1)  +"!~"+ser->channels[i].getOperators()[j].user_name + "@"+ser->hostname + " MODE " + this->args[0] )+   "\r\n";
                            ser->channels[i].sendMessage(msg); 
                            return;
                        }
                        else if (this->args[1] == "-k")
                        {
                            ser->channels[i].setMode(this->args[1]);
                            ser->channels[i].setKey("\0");
                            std::string msg = (ser->channels[i].getOperators()[j].nickname.substr(1)  +"!~"+ser->channels[i].getOperators()[j].user_name + "@"+ser->hostname + " MODE " + this->args[0]  ) + "\r\n";
                            ser->channels[i].sendMessage(msg);
                            return;
                        }
                        else
                        {
                            std::string msg = NotMode(args[0], ser->hostname);
                            if(send(fd, msg.c_str(), msg.length(), 0) < 0)
                            {
                                std::cout << "Failed Send Try Again"<<std::endl;
                            }
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
            return;
        }
    }
    return ;
    // else
    // {
    //     for (unsigned long i = 0; i < ser->channels.size(); i++)
    //     {
    //         if (ser->channels[i].getName() == this->args[0])
    //         {
    //             if (ser->channels[i].getOperators().size() == 0)
    //             {
    //                 std::string msg = NotOPRT(args[0], ser->hostname);
    //             if(send(fd, msg.c_str(), msg.length(), 0) < 0)
    //             {
    //                 std::cout << "Failed Send Try Again"<<std::endl;
    //             }
    //                 return;
    //             }
    //             for (unsigned long j = 0; j < ser->channels[i].getOperators().size(); j++)
    //             {
    //                 if (ser->channels[i].getOperators()[j].fd == this->fd)
    //                 {
                        
    //                     if (this->args[1][0] == '+' && this->args[1][1] == 't')
    //                     {
    //                         ser->channels[i].setMode(this->args[1]);
    //                         for (unsigned long l = 2; l < ser->channels[i].getOperators().size(); l++)
    //                         {
                                
    //                         if (l == 2)
    //                             ser->channels[i].setTopic(this->args[l]); // need to add mode if not existe
    //                         else if (l > 1)
    //                             ser->channels[i].setTopic((ser->channels[i].getTopic() + this->args[l]));
    //                         }
    //                         std::string msg = ":" + ser->channels[i].getOperators()[j].nickname.substr(1) + " MODE " + this->args[0] + " " + this->args[1] + " " + this->args[2] + "\r\n";
    //                         if (send(fd, msg.c_str(), msg.length(), 0) < 0)
    //                         {
    //                             std::cout << "Failed Send Try Again"<<std::endl;
    //                         }
    //                         return;
    //                     }
    //                     else if (this->args[1][0] == '-' && this->args[1][1] == 't')
    //                     {
    //                         ser->channels[i].setMode(this->args[1]);
    //                         ser->channels[i].setTopic("\0");
    //                         std::string msg = ":" + ser->channels[i].getOperators()[j].nickname.substr(1) + " MODE " + this->args[0] + " " + this->args[1] + "\r\n";
    //                         if (send(fd, msg.c_str(), msg.length(), 0) < 0)
    //                         {
    //                             std::cout << "Failed Send Try Again"<<std::endl;
    //                         }
    //                         return;
    //                     }
    //                 }
    //             }
    //             std::string msg = NotOPRT(args[0], ser->hostname);
    //             if(send(fd, msg.c_str(), msg.length(), 0) < 0)
    //             {
    //                 std::cout << "Failed Send Try Again"<<std::endl;
    //             }
    //             return;
    //         }
    //         std::string msg = ChannelExist(args[0], ser->hostname);
    //         if(send(fd, msg.c_str(), msg.length(), 0) < 0)
    //         {
    //             std::cout << "Failed Send Try Again"<<std::endl;
    //         }
    //     }
    // }
            
}
void Command::ParceModeCommand(std::vector <std::string> splited, int client_fd)
{
    // for (unsigned long i = 1; i < splited.size(); i++)
    // {
    //     if (splited[1][0] != '#' && splited[1][0] != '&' && splited[1][0] != '!')
    //     {
    //         throw std::invalid_argument("Invalid argument--------------------------------------------");
    //     }
    //     // if (i == 1)
    //     // {
    //     //     for (unsigned long j = 1; j < splited[1].size(); j++)
    //     //     {
    //     //         if (!((splited[i][j] >= 'a' && splited[i][j] <= 'z') || (splited[i][j] >= 'A' && splited[i][j] <= 'Z')  || (splited[i][j] == '_')))
    //     //         {
    //     //             throw std::invalid_argument("Invalid argument++++++++++++++++++++++++++++++++++++++++++");
    //     //         }
    //     //     }
    //     //     this->args.push_back(splited[i]);
    //     // }
    //     if (splited[i][0] == '+')
    //     {
    //         for (unsigned long j = 1; j < splited[i].size(); j++)
    //         {
    //             if (splited[i][j] == 'o' || splited[i][j] == 't' || splited[i][j] == 'i' || splited[i][j] == 'k' || splited[i][j] == 'l')
    //             {
    //                 this->args.push_back(splited[i]);
    //             }
    //             else
    //             {
    //                 throw std::invalid_argument("Invalid argument................................");
    //             }
    //         }
    //     }
    //     else if (splited[i][0] == '-')
    //     {
    //         for (unsigned long j = 1; j < splited[i].size(); j++)
    //         {
    //             if (splited[i][j] == 'o' || splited[i][j] == 't' || splited[i][j] == 'i' || splited[i][j] == 'k' || splited[i][j] == 'l')
    //             {
    //                 this->args.push_back(splited[i]);
    //             }
    //             else
    //             {
    //                 throw std::invalid_argument("Invalid argument~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    //             }
    //         }
    //     }
    //     else if (i == 4)
    //     {
    //         this->args.push_back(splited[i]);
    //     }
    //     else if (splited[i][0] == '#')
    //     {
    //         this->args.push_back(splited[i]);
    //     }
    //     else if (splited[2] == "+t")
    //     {
    //         this->args.push_back(splited[i]);
    //     }
    //     else
    //     {
    //         throw std::invalid_argument("Invalid argument___________________________________________");
    //     }
    // }
    if (splited.size() == 0)
        return;
    for (unsigned long i = 1; i < splited.size(); i++)
    {
        // if (i == 1)
        this->args.push_back(splited[i]);
        // else if (i == 2)
        //     this->args.push_back(splited[i]);
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
                            if (l == 1)
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
        std::string msg = ERR_NOSUCHCHANNEL(args[0], ser->hostname);
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
                    if (ser->channels[i].getOperators()[j].fd == this->fd)
                    {
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
                                ser->channels[i].addUser(ser->clients[l], 0);
                                std::string msg = ":" + ser->channels[i].getOperators()[j].nickname.substr(1) +"!~"+ser->channels[i].getOperators()[j].user_name + "@"+ser->hostname + " INVITE " + this->args[0] + " " + this->args[1] + "\r\n";
                                // std::string msg = ":" + ser->channels[i].getOperators()[j].nickname.substr(1) + " INVITE " + this->args[0] + " " + this->args[1] + "\r\n";
                                if (send(fd, msg.c_str(), msg.length(), 0) < 0)
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
void Command::ParcePrivmsg(std::vector <std::string> splited, int client_fd)
{
     this->fd = client_fd;
    for (unsigned long i = 1; i < splited.size(); i++) 
    {
        // std::cout << "splited[i]: " << splited[i] << std::endl;
        // std::cout << "size: " << splited[].size() << std::endl;
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
    // std::cout << "args[0]: " << this->args[0] << std::endl; 
    if (ser->channels.size() == 0)
    {
        for (unsigned long j = 0; j < ser->clients.size(); j++)
            {
                if (ser->clients[j].nickname == this->args[0])
                {
                    std::string msg;
                    // std::cout << "hello"<<this->args[2].size() << std::endl;
                    // std::cout << "args: "<<getClientByFd(ser, this->fd)->nickname  << std::endl;
                    // std::cout << " getClientByFd(ser, this->fd)->user: " << getClientByFd(ser, this->fd)->user_name << std::endl;
                    // std::cout <<  "ser->clients[j].nickname: " << ser->clients[j].nickname << std::endl;
                    // std::cout << 

                    if (this->args.size() > 0 && this->args[1][0] != ':')
                         msg = ":" + getClientByFd(ser, this->fd)->nickname + "!~"+ getClientByFd(ser, this->fd)->user_name +"@"+ser->hostname+" PRIVMSG " + ser->clients[j].nickname + " :";
                    else if (this->args.size() > 0 && this->args[1][0] == ':')
                        msg = ":" + getClientByFd(ser, this->fd)->nickname + "!~"+ getClientByFd(ser, this->fd)->user_name +"@"+ser->hostname+" PRIVMSG " + ser->clients[j].nickname + " ";
                    // std::cout << "args: " << std::endl;
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
    // std::cout << "args[0]: " << this->args[0] << std::endl;
    if (this->args.size() == 0)
        return ;
    if (this->args[0][0] == '#')
    {

        for (unsigned long i = 0; i < ser->channels.size(); i++)
        {
            if (ser->channels[i].getName() == this->args[0])
            {
                for (unsigned long j = 0; j < ser->channels[i].getUsers().size(); j++)
                {
                        std::string msg;
                        if (this->args.size() > 1 && this->args[1][0] != ':')
                             msg = ":" + getClientByFd(ser, this->fd)->nickname + "!~" + getClientByFd(ser, this->fd)->user_name + "@" + ser->hostname + " PRIVMSG " + ser->channels[i].getUsers()[j].nickname + " :";
                        else if (this->args.size() > 1 && this->args[1][0] == ':')
                            msg = ":" + getClientByFd(ser, this->fd)->nickname + "!~" + getClientByFd(ser, this->fd)->user_name + "@" + ser->hostname + " PRIVMSG " + ser->channels[i].getUsers()[j].nickname + " ";
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
                return ;
            }
    }
    }
    // for (unsigned long i = 0; i < ser->channels.size(); i++)
    // {
            
            for (unsigned long j = 0; j < ser->clients.size(); j++)
            {
                if (ser->clients[j].nickname == this->args[0])
                {
                    std::string msg;
                    if (this->args.size() > 0 && this->args[1][0] != ':')
                         msg = ":" + getClientByFd(ser, this->fd)->nickname + "!~"+ getClientByFd(ser, this->fd)->user_name +"@"+ser->hostname+" PRIVMSG " + ser->clients[j].nickname + " :";
                    else if (this->args.size() > 0 && this->args[1][0] == ':')
                        msg = ":" + getClientByFd(ser, this->fd)->nickname + "!~"+ getClientByFd(ser, this->fd)->user_name +"@"+ser->hostname+" PRIVMSG " + ser->clients[j].nickname + " ";
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

        // }
}
void Command::executecmd(server *server) {
    if (server->splited[0] == "QUIT")
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
        if (server->splited.size() < 2)
        {
            std::string msg = msg_err(server->splited[0], server->hostname);
            if(send(server->client_fd, msg.c_str(), msg.length(), 0) < 0)
            {
                std::cout << "Failed Send Try Again"<<std::endl;
            }
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
                        return;
                    }
                    server->channels[i].notifyUserJoin(server->clients[j].nickname, server->hostname, o);
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
            server->channels[0].setMode("+t");
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
        addusertoChannel(server, this->args[0], 1);
    }
    else
    {
        
        for (unsigned long i = 0; i < this->args.size(); i++)
        {
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
                        if (checkMode(server->channels[j].getMode(), "+i") == 1)
                        {  
                            std::string msg = ERR_INVITEONLYCHAN(args[0], server->hostname);
                        if (send(fd, msg.c_str(), msg.length(), 0) < 0)
                        {
                            std::cout << "Failed Send Try Again"<<std::endl;
                        }
                            return;
                        }
                        else if (checkMode(server->channels[j].getMode(), "+k") == 1 && i <= this->keys.size() && server->channels[j].getKey() != this->keys[i])
                        {
                            if (server->channels[j].getKey() != this->args[i + 1]) //need more check and fix this
                            {
                                 std::string msg = ERR_BADCHANNELKEY(args[0], server->hostname);
                            if (send(fd, msg.c_str(), msg.length(), 0) < 0)
                            {
                                std::cout << "Failed Send Try Again"<<std::endl;
                            }
                                // throw std::invalid_argument("Channel is key protected");
                                return;
                            }
                        }
                        else if (server->channels[i].getKey().size() > 0 && i <= this->keys.size() && server->channels[i].getKey() != this->keys[i])
                        {
                             std::string msg = ERR_BADCHANNELKEY(args[0], server->hostname);
                            if (send(fd, msg.c_str(), msg.length(), 0) < 0)
                            {
                                std::cout << "Failed Send Try Again"<<std::endl;
                            }
                            // throw std::invalid_argument("Channel is key protected");
                            return ;
                        }
                        addusertoChannel(server, this->args[i], 0);
                        break ;
                    }
                }
                server->channels.push_back(Channel(this->args[i]));
                server->channels[server->channels.size() - 1].setMode("+t");
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
  