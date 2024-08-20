#include "command.hpp"

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
            while (k < command[i].size() && command[i][k] != ',')
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
        // if (command[3][0] == ':')
        //     this->message = command[3].substr(1);
        // else
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