/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Operators.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakazdao <yakazdao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 21:03:24 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/03/02 22:16:59 by yakazdao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Operators.hpp"


Operators::Operators(){
    
}
Operators::~Operators(){
    
}

void Operators::send_message(Server &My_serv, std::string str, int channel_pos)
{
    if (channel_pos == -1)
        return ;
    std::vector<Client>::iterator it;
    std::vector<Client> clients_it = My_serv.channels[channel_pos].getClients();
    for (it = clients_it.begin(); it != clients_it.end(); it++){
        int fd = (*it).getFd();
        send(fd, str.c_str(), str.length(), 0);
    }
}
bool Operators::there_is_Fd(Server &My_serv, int fd)
{
    std::vector<pollfd>::iterator it;
    for (it = My_serv.polls.begin(); it != My_serv.polls.end(); it++){
        if (fd == (*it).fd)
            return (true);
    }
    return (false);
}


std::string Operators::Get_client_nick(Server &My_serv, int clientId, int channel_pos)
{
    if (channel_pos == -1)
        return ("");
    for (std::vector<Client>::iterator it = My_serv.channels[channel_pos].getClients().begin(); it != My_serv.channels[channel_pos].getClients().end(); it++){
        if (My_serv.polls[clientId].fd == it->getFd())
            return (it->getNickname());
    }
    return ("");
}

bool Operators::there_is_channel(Server &My_serv, std::string channel, int &channel_pos, int clientId)
{
    for (std::vector<Channel>::iterator it = My_serv.channels.begin(); it != My_serv.channels.end(); it++)
    {
        if (channel == it->getChannelName())
        {
            channel_pos = std::distance(My_serv.channels.begin(), it);
            return (true);
        }
    }
    std::string str = ERR_NOSUCHCHANNEL(channel);
    send(My_serv.polls[clientId].fd, str.c_str(), str.length(), 0);
    return (false);
}

bool Operators::already_on_channel(Server &My_serv, std::string client, std::string channel, int clientId, int channel_pos, int check)
{
    if (channel_pos == -1)
        return (false);
    std::vector<Client>::iterator it;
    std::vector<Client> clients_it = My_serv.channels[channel_pos].getClients();
    for (it = clients_it.begin(); it != clients_it.end(); it++){
        if (client == it->getNickname())
        {
            if (check == 1)
            {
                std::string str = ERR_USERONCHANNEL(client, client, channel);
                send(My_serv.polls[clientId].fd, str.c_str(), str.length(), 0);
            }
            return (true);
        }
    }
    if (!check)
    {
        std::string str = ERR_USERNOTINCHANNEL(client, channel);
        send(My_serv.polls[clientId].fd, str.c_str(), str.length(), 0);
    }
    return (false);
}

int Operators::Get_Channel_client_pos(Server &My_serv, const std::string& nickname, int channel_pos)
{
    if (channel_pos == -1)
        return (-1);
    std::vector<Client>& clients = My_serv.channels[channel_pos].getClients();
    std::vector<Client>::iterator it;
    for (it = clients.begin(); it != clients.end(); ++it)
    {
        if (nickname == it->getNickname())
            return std::distance(clients.begin(), it);
    }
    return -1; 
}

bool Operators::there_is_user(Server &My_serv, std::string client, int clientId)
{
    for (std::vector<Client>::iterator it = My_serv.clients.begin(); it < My_serv.clients.end(); it++) {
        if (client == it->getNickname())
            return true;
    }
    std::string str = ERR_NOSUCHNICK(client);
    send(My_serv.polls[clientId].fd, str.c_str(), str.length(), 0);
    return false;
}

bool Operators::Check_Channel_Op(Server &My_serv, std::string client_nick, std::string channel, int channel_pos, int clientId)
{
    if (channel_pos == -1)
        return (false);
    std::cout << "client_nick = " << client_nick<< std::endl;
    std::vector<std::string>::iterator it;
    std::vector<std::string> operators = My_serv.channels[channel_pos].getOperators();
    if (operators.empty())
        return(false);
    for (it = operators.begin(); it != operators.end(); it++){
        if (client_nick == (*it))
            return (true);
    }
    std::string str = ERR_CHANOPRIVSNEEDED(channel);
    send(My_serv.polls[clientId].fd, str.c_str(), str.length(), 0);
    return (false);
}