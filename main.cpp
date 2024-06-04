
#include<iostream>
#include"srcs/server.hpp"
int main(int argc,char **argv)
{
   int port_num;
   int password;
   Server ser;
    
    //  and 65535 are reserved ports
    if (argc != 3)
    {
        std::cout <<"Error Try Again"<<std::endl;
        exit (0);
    }
    port_num = atoi(argv[1]);
    password = atoi(argv[2]);
    if (port_num < 0 || port_num > 65535)
    {
        std::cout <<"Error Try Again"<<std::endl;
        exit (0);
    }
    ser.Server_init(port_num);
    return (0);
}