#include "command.hpp"
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
                    std::string msg = ":" + ser->hostname + " 332 " + ser->client_cmd.nickname + " " + ser->channels[i].getName() + " :" + ser->channels[i].getTopic() + "\r\n";
                    if(send(fd, msg.c_str(), msg.length(), 0) < 0)
                    {
                        std::cout << "Failed Send Try Again"<<std::endl;
                    }
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
                        ser->channels[i].sendMessage(msg);
                        return;
                    }
                }
            }
        }
        std::string msg = ERR_NOSUCHCHANNEL(args[0], ser->hostname);
        if (send(fd, msg.c_str(), msg.length(), 0) < 0)
        {
            std::cout << "Failed Send Try Again"<<std::endl;
        }
}