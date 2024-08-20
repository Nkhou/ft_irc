#include "command.hpp"

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
    // for (size_t p = 0; p < this->keys.size(); p++)
    // {
    //     std::cout << "---args : " << this->keys[p] << std::endl;
    // }
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
                std::string msg = ":" +ser->hostname + " 324 " + ser->channels[i].getName() + " ";
                if (ser->channels[i].getMode().size() == 0)
                {
                    msg += "+\r\n";
                    ser->channels[i].sendMessage(msg);
                    return;
                }
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
                            else if (this->keys[o] =="-o")
                            {
                                for (unsigned long l = 0; l < ser->channels[i].getUsers().size(); l++)
                                {
                                    if (ser->channels[i].getUsers()[l].nickname == this->args[1])
                                    {
                                        size_t c = 0;
                                        for (unsigned long k = 0; k < ser->channels[i].getOperators().size(); k++)
                                        {
                                            if (ser->channels[i].getOperators()[k].nickname.substr(1) == this->args[1])
                                                break ;
                                            c++;
                                        }
                                        std::cout << "c : " << c << std::endl;
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
                            else if (this->keys[o] == "+l" )
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
                                std::cout << "args : " << this->args[o + 1 - z].length() << std::endl;
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
                                for (unsigned long l = 0; l < ser->channels[i].getUsers().size(); l++)
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
                                for (unsigned long l = 0; l < ser->channels[i].getUsers().size(); l++)
                                {
                                    if (ser->channels[i].getUsers()[l].nickname == this->args[1])
                                    {
                                        size_t c = 0;
                                        for (unsigned long k = 0; k < ser->channels[i].getOperators().size(); k++)
                                        {
                                            if (ser->channels[i].getOperators()[k].nickname.substr(1)  == this->args[1])
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
                            else if (this->keys[o] == "+t")
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
                                std::string msg = ":" + (ser->channels[i].getOperators()[j].nickname.substr(1)  +"!~"+ser->channels[i].getOperators()[j].user_name + "@"+ser->hostname + " MODE " + this->args[0] ) + " " +this->keys[o] + "\r\n";
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