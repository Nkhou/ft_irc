/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saboulal <saboulal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 11:06:05 by saboulal          #+#    #+#             */
/*   Updated: 2024/08/15 08:44:37 by saboulal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

std::string message_err_nick_name(std::string hostname, std::string code, std::string nickname, std::string command, std::string msg)
{
	std::string message =  ":" + hostname + " " + code + " " + nickname + " " + command +" "+msg + "\r\n";
	return message;
}

std::string msg_err(std::string nick, std::string hostname)
{
   std::string msg = ":" + hostname + " 461 " +"* "+ nick + " :Not enough parameters\r\n";
    return msg;
}

std::string msg_err_one(std::string nick, std::string hostname)
{
   std::string msg = ":" + hostname + " 461 " +"* "+ nick + " just testing wax khadam\r\n";
    return msg;
}

std::string msg_erroneusnickname(std::string nick,std::string hostname)
{
    std::string msg = ":" + hostname + " 432" + " * " + nick + " Erroneous Nickname\r\n";
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
std::string msg_may_registered(std::string nick,std::string hostname)
{
    std::string msg = ":" + hostname + " 462 " + nick + " :You may not reregister !\r\n";
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
std::string msg_errsend(std::string nick, std::string hostname)
{
    std::string msg = ":" + hostname + "412" + nick + " :No text to send\r\n";
    return msg;
}
std::string NotMode(std::string nick, std::string hostname)
{
    std::string msg = ":" + hostname + " 472 " + nick + " :is unknown mode char to me\r\n";
    return msg;
}
std::string notfyMessage(std::string nick, std::string hostname, std::string message, std::string name)
{
    std::string msg = ":" + nick + "!~" + name + hostname + " " + message + "\r\n";
    return msg;
}

std::string notenghparam(std::string nick, std::string hostname)
{
    std::string msg = ":" + hostname + " 461 " + nick + " :Not enough parameters\r\n";
    return msg;
}
std::string unknowncommand(std::string nick, cli client ,std::string hostname)
{
    std::string msg = ":" + hostname + " 421 " + client.nickname +" "+nick + " :Unknown command\r\n";
    return msg;
}

std::string kickerr(std::string nick, std::string hostname)
{
    std::string msg = ":" + hostname + " 461 " + nick+  " KICK :Not enough parameters\r\n";
    return msg;
}
std::string ERR_KEYSET(std::string nick, std::string hostname)
{
    std::string msg = ":" + hostname + " 467 " + nick + " :Channel key already set\r\n";
    return msg;
}
std::string ERR_CHANNELISFULL(std::string nick, std::string hostname)
{
    std::string msg = ":" + hostname + " 471 " + nick + " :Cannot join channel (+l)\r\n";
    return msg;
}
std::string ERR_NOTONCHANNEL(std::string nick, std::string hostname)
{
    std::string msg = ":" + hostname + " 442 " + nick + " :You're not on that channel\r\n";
    return msg;
}
std::string ERR_NOTOPERATOR(std::string nick, std::string hostname)
{
    std::string msg = ":" + hostname + " 482 " + nick + " :You're not channel operator\r\n";
    return msg;
}