/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkh <nkh@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 11:06:05 by saboulal          #+#    #+#             */
/*   Updated: 2024/07/16 11:36:15 by nkh              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

std::string msg_err(std::string nick, std::string hostname)
{
   std::string msg = ":" + hostname + " 461 " +"* "+ nick + " :Not enough parameters\r\n";
    return msg;
}

std::string msg_erroneusnickname(std::string nick,std::string hostname)
{
    std::string msg = ":" + hostname + " 432" + " * " + nick + " :Erroneous Nickname\r\n";
    return msg;
}

std::string msg_welcome(std::string nick,std::string hostname)
{
    std::string msg = ":" + hostname + " 001 " + nick + " Welcome to " + hostname + " Internet Relay Chat Network\r\n";
    return msg;
}

std::string msg_nonicknamegiven(std::string hostname)
{
    std::string msg = ":" + hostname + " 431 " + "* " +":No nickname given\r\n";
    return msg;
}

std::string msg_notregistered(std::string nick,std::string hostname)
{
    std::string msg = ":" + hostname + " 451 " + nick + " :You have not registered !\r\n";
    return msg;
}

std::string msg_nicknameinuse(std::string nick,std::string hostname)
{
    std::string msg = ":" + hostname + " 433 " + nick + " :Nickname is already in use !\r\n";
    return msg;
}

std::string ChannelExist(std::string channel, std::string hostname)
{
    std::string msg = ":" + hostname + " 403 " + channel + " :No such channel\r\n";
    return msg;
}

std::string NotOPRT(std::string nick, std::string hostname)
{
    std::string msg = ":" + hostname + " 482 " + nick + " :You're not channel operator\r\n";
    return msg;
}
std::string msg_errpriv(std::string nick, std::string hostname)
{
    std::string msg = ":" + hostname + " 401 " + nick + " :No such nick/channel\r\n";
    return msg;
}
std::string sendMessage(std::string nick, std::string hostname, std::string message)
{
    (void)hostname;
    (void)nick;
    std::string msg =  message ;
    return msg;
}
std::string RPL_NOTOPIC(std::string nick, std::string hostname)
{
    std::string msg = ":" + hostname + " 331 " + nick + " :No topic is set\r\n";
    return msg;
}
std::string ERR_BADCHANNELKEY(std::string nick, std::string hostname)
{
    std::string msg = ":" + hostname + " 475 " + nick + " :Cannot join channel (+k)\r\n";
    return msg;
}

std::string ERR_NOSUCHCHANNEL(std::string nick, std::string hostname)
{
    std::string msg = ":" + hostname + " 403 " + nick + " :No such channel\r\n";
    return msg;
}
std::string ERR_INVITEONLYCHAN(std::string nick, std::string hostname)
{
    std::string msg = ":" + hostname + " 473 " + nick + " :Cannot join channel (+i)\r\n";
    return msg;
}

// std::strin noSUCHFILE(std::string nick, std::string hostname)
// {
//     std::string msg = ":" + hostname + " 492 " + nick + " :No such file or directory\r\n";
//     return msg;
// }