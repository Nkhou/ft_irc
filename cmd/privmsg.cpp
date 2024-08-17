// #include"srcs/server.hpp"
#include"command.hpp"
void Command::ParcePrivmsg(std::vector <std::string> command, int client_fd)
{
    // for (unsigned long i = 0; i < command.size(); i++)
    //     std::cout << "command[1]: " << command[i] << std::endl;
        int i = 1;
        for (unsigned long j = 0; j < command[i].size(); j++)
        {
            unsigned long k = j;
            while (k < command[i].size() && command[i][k] != ',')
                k++;
            if (k > j)
            {
                    args.push_back(command[i].substr(j, k));
                j = k;
            }
        }
    if (command.size() == 3)
        this->message = command[command.size() - 1];
    else if (command.size() > 3)
        this->message = command[2];
    else
        this->message = "\0";
    this->fd = client_fd;
}
void Command::PrivmsgCommand(server *ser)
{
    if (this->args.size() == 0)
        return ;
    // std::cout << "args[0]: " << this->args[0] << std::endl; 
    int flag = 0;
    // for (size_t i = 0; i < this->args.size(); i++)
    //     std::cout << "args: " << this->args[i] << std::endl;
    // if (ser->channels.size() == 0)
    // {
    //     for (size_t o = 0; o < this->args.size(); o++)
    //     {
    //         for (unsigned long j = 0; j < ser->clients.size(); j++)
    //         {
    //             if (ser->clients[j].nickname == this->args[o])
    //             {
    //                 flag = 1;
    //                 std::string msg;
    //                 msg = ":" + getClientByFd(ser, this->fd)->nickname + "!"+ getClientByFd(ser, this->fd)->user_name +"@" +ser->hostname+" PRIVMSG " + ser->clients[j].nickname + " :" + this->message + "\r\n";
    //                 std::cout << "msg: " << msg << std::endl;

    //                 if(send(ser->clients[j].fd, msg.c_str(), msg.length(), 0) < 0)
    //                 {
    //                     std::cout << "Failed Send Try Again"<<std::endl;
    //                 }
    //             }
    //         }
    //         if (flag == 0)
    //         {
    //             std::string msg = msg_errpriv(ser->splited[0], ser->hostname);
    //             if(send(ser->client_fd, msg.c_str(), msg.length(), 0) < 0)
    //             {
    //                 std::cout << "Failed Send Try Again"<<std::endl;
    //             }
    //         };
    //     }
    //     return ;
    // }
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
                                msg = ":" + getClientByFd(ser, this->fd)->nickname + "!~" + getClientByFd(ser, this->fd)->user_name + "@" + ser->hostname + " PRIVMSG " + ser->channels[i].getUsers()[j].nickname + " :" + this->message + "\r\n";
                                if (ser->channels[i].getUsers()[j].fd != this->fd)
                                {
                                    if(send(ser->channels[i].getUsers()[j].fd, msg.c_str(), msg.length(), 0) < 0)
                                    {
                                        std::cout << "Failed Send Try Again"<<std::endl;
                                    }
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
                    }
                }
            }
            else
            {
                std::cout << "args[o]: " << this->args[o] << std::endl;
                std::cout << this->message << std::endl;
                for (unsigned long j = 0; j < ser->clients.size(); j++)
                {
                    if (ser->clients[j].nickname == this->args[o])
                    {
                        flag = 1;
                        std::string msg;
                        //:test!~u@qk3i8byd6tfyg.irc PRIVMSG 1337 :hello
                        msg = ":" + ser->client_cmd.nickname + "!~"+ ser->client_cmd.user_name + "@localhost PRIVMSG " + ser->clients[j].nickname + " :" + this->message + "\r\n";
                       
                        std::cout << msg << std::endl;
                        std::cout << ser->client_cmd.ip_addr << std::endl;

                        if(send(ser->clients[j].fd, msg.c_str(), msg.length(), 0) < 0)
                        {
                            std::cout << "Failed Send Try Again"<<std::endl;
                        }
                        break;
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

}