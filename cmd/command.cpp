#include "command.hpp"

void Command::ParceCommand(std::vector<std::string> command, int fd)
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
                        // std::cout << command[i].substr(k, j - k) <<std::endl;
                        this->args.push_back(command[i].substr(k, j - k));
                    }
                }
            }
            else if (((command[i][j] >= 'a' && command[i][j] <= 'z') || (command[i][j] >= 'A' && command[i][j] <= 'Z')) && i != 1)
            {
                while (j < command[i].size() && ((command[i][j] >= 'a' && command[i][j] <= 'z') || (command[i][j] >= 'A' && command[i][j] <= 'Z')))
                {
                    j++;
                }
                if (j > k)
                {
                    this->keys.push_back(command[i].substr(k, j - k));
                }
            }
            else if (command[i][j] != '\n')
            {
                while (j < command[i].size() && command[i][j] != ',')
                {
                    j++;
                }
                throw std::invalid_argument("Invalid argument+++");
            }
            else
            {
                std::cout << "man naytak" << std::endl;
            }
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
        
        // std::cout << "Keys size: " << std::endl;
        // std::cout << "Args size: " << this->args.size() << std::endl;
        // for (unsigned long i = 0; i< this->args.size(); i++)
        // {
        //     std::cout << "Args: " << this->args[i] << std::endl;
        // }
        // return ;
        executecmd(server);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return;
    }
    // ParceCommand(server->splited, server->client_fd);
}

void Command::ParceCommandkick(std::vector<std::string> command, int fd, std::vector<Channel>channel)
{
    for (unsigned long i = 1; i < command.size(); i++)
    {
        for (unsigned long j = 1; j < command[i].size(); j++)
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
    for (unsigned long i; i < channel.size(); i++)
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
            throw std::invalid_argument("You are not an operator");
        
        }
    }
}
void Command::KickCommand(server *ser)
{
    (void)ser;
}
void Command::ModeCommand(server *ser)
{
    if (this->args.size() == 0)
        return;
    if (ser->channels.size() == 0)
        throw std::invalid_argument("Channel does not exist");
    // std::cout<< ser->channels.size() <<std::endl;
    if (this->args.size() == 1)
    {
        for (unsigned long i = 0; i < ser->channels.size(); i++)
        {
            if (ser->channels[i].getName() == this->args[0])
            {
                if (ser->channels[i].getOperators().size() == 0)
                {
                    throw std::invalid_argument("You are not an operator");
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
                throw std::invalid_argument("You are not an operator");
                return;
            }
        }
        throw std::invalid_argument("Channel does not exist");
    }
    else if (this->args.size() == 2)
    {
        
        for (unsigned long i = 0; i < ser->channels.size(); i++)
        {
            if (ser->channels[i].getName() == this->args[0])
            {
                if (ser->channels[i].getOperators().size() == 0)
                {
                    throw std::invalid_argument("You are not an operator");
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
                throw std::invalid_argument("You are not an operator");
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
                    throw std::invalid_argument("You are not an operator");
                    return;
                }
                for (unsigned long j = 0; j < ser->channels[i].getOperators().size(); j++)
                {
                    if (ser->channels[i].getOperators()[j].fd == this->fd)
                    {
                        // ser->channels[i].setMode(this->args[1]);
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
                throw std::invalid_argument("You are not an operator");
                return;
            }
            throw std::invalid_argument("Channel does not exist");
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
                    throw std::invalid_argument("You are not an operator");
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
                            // ser->channels[i].setTopic(this->args[2]);
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
                throw std::invalid_argument("You are not an operator");
                return;
            }
            throw std::invalid_argument("Channel does not exist");
        }
    }
            
}
void Command::ParceModeCommand(std::vector <std::string> splited, int client_fd)
{
    std::cout << "hello" << std::endl;
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
        if (splited[i][0] != '#' && i != 1)
             throw std::invalid_argument("Invalid argument****");
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
                    throw std::invalid_argument("You are not an operator");
                    return;
                }
                for (unsigned long j = 0; j < ser->channels[i].getOperators().size(); j++)
                {
                    if (this->args.size() == 1)
                    {
                        if (ser->channels[i].getTopic() == "\0")
                        {
                            write(this->fd, ":No topic is set.", 17);
                            return;
                        }
                        ser->channels[i].PrintTopic(this->fd);
                    }
                    if (ser->channels[i].getOperators()[j].fd == this->fd)
                    {
                        for (unsigned long l = 1; l < this->args.size(); l++)
                        {
                            if (l == 1)
                                ser->channels[i].setTopic(this->args[l]); // need to add mode if not existe
                            else if (l > 1)
                                ser->channels[i].setTopic((ser->channels[i].getTopic() + this->args[l]));
                            // ser->channels[i].setTopic((ser->channels[i].getTopic() + this->args[l]));
                        }
                        return;
                    }
                }
                throw std::invalid_argument("You are not an operator");
                return;
            }
        }
        throw std::invalid_argument("Channel does not exist");
}
void Command::InviteCommand(server *ser)
{
    if (this->args.size() == 0)
        return ;
    for (unsigned long i = 0; i < ser->channels.size(); i++)
    {
        // if ()
    }
}
void Command::ParceInvite(std::vector <std::string> splited, int client_fd)
{
    for (unsigned long i = 1; i < splited.size(); i++)
    {
        // if (splited[i][0] != '#')
        //      throw std::invalid_argument("Invalid argument****");
        this->args.push_back(splited[i]);
    }
     this->fd = client_fd;
}
void Command::executecmd(server *server) {
    // kick, INVITE, getMode(), TOPIC, JOIN, 
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
        ParceCommand(server->splited, server->client_fd);
        // ParceCommandkick(server->splited, server->client_fd, server->channels);
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
        ParceCommandkick(server->splited, server->client_fd, server->channels);
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
        ParceTopic(server->splited, server->client_fd);
        TopicCommand(server);
    }
    else if (server->splited[0] == "INVITE")
    {
        ParceInvite(server->splited, server->client_fd);
        InviteCommand(server);
    }
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
                server->channels[0].setKey(this->keys[0]);
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
                        return;
                    }
                    else if (checkMode(server->channels[i].getMode(), "+k") == 1 && server->channels[i].getKey() != this->keys[0])
                    {
                        throw std::invalid_argument("Channel is key protected");
                        return;
                    }
                    addusertoChannel(server, this->args[i], 0);
                    return;
                }
            }
            server->channels.push_back(Channel(this->args[0]));
            if (this->keys.size() > 0)
                server->channels[server->channels.size() - 1].setKey(this->keys[0]);
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
                    server->channels[0].setKey(this->keys[i]);
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
                        else if (checkMode(server->channels[j].getMode(), "+k") == 1 && server->channels[j].getKey() != this->keys[i])
                        {
                            if (server->channels[j].getKey() != this->args[i + 1]) //need more check and fix this
                            {
                                throw std::invalid_argument("Channel is key protected");
                                return;
                            }
                        }
                        addusertoChannel(server, this->args[i], 0);
                        return;
                    }
                }
                server->channels.push_back(Channel(this->args[i]));
                if (this->keys.size() > 0)
                    server->channels[server->channels.size() - 1].setKey(this->keys[i]);
            }
            addusertoChannel(server, this->args[i], 1);
        }
    }
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++