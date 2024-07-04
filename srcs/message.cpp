/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saboulal <saboulal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 11:06:05 by saboulal          #+#    #+#             */
/*   Updated: 2024/07/04 14:45:41 by saboulal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

std::string msg_err(std::string nick,std::string hostname)
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