#include "command.hpp"

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
                    for (unsigned long l = 0; l < ser->clients.size(); l++)
                    {
                        if (this->args[0] == ser->clients[l].nickname)
                        {
                            for (unsigned long c = 0; c < ser->channels[i].getUsers().size(); c++)
                            {
                                if (ser->channels[i].getUsers()[c].nickname == ser->clients[l].nickname)
                                {
                                    std::string msg = ERR_USERONCHANNEL(ser->clients[l].nickname, ser->hostname);
                                    if(send(ser->client_fd, msg.c_str(), msg.length(), 0) < 0)
                                    {
                                        std::cout << "Failed Send Try Again"<<std::endl;
                                    }
                                    return ;
                                }
                            }
                            ser->channels[i].setFd(ser->clients[l].fd);
                            std::string msg = ":" + ser->channels[i].getOperators()[j].nickname.substr(1) +"!~"+ser->channels[i].getOperators()[j].user_name + "@"+ser->hostname + " INVITE " + this->args[0] + " " + this->args[1] + "\r\n";
                            ser->channels[i].sendMessage(msg);
                            if (send(ser->clients[l].fd, msg.c_str(), msg.length(), 0) < 0)
                            {
                                std::cout << "Failed Send Try Again"<<std::endl;
                            }
                            return ;
                        }
                    }
                    std::string msg =  ERR_NOSUCHNICK(this->args[1], ser->hostname);
                    if(send(ser->client_fd, msg.c_str(), msg.length(), 0) < 0)
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
    std::string msg = ERR_NOSUCHCHANNEL(this->args[1], ser->hostname);
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