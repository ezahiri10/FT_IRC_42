/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Operators.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fagr <ael-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 21:03:24 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/03/07 01:35:41 by ael-fagr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Operators.hpp"


Operators::Operators(){
    
}
Operators::~Operators(){
    
}

void send_message(Channel &channel, std::string str)
{
    std::vector<Client>::iterator it;
    std::vector<Client> clients_it = channel.getClients();
    for (it = clients_it.begin(); it != clients_it.end(); it++){
        int fd = (*it).getFd();
        send(fd, str.c_str(), str.length(), 0);
    }
}


std::string Operators::Get_client_nick(Server &My_serv, Channel &channel, int clientId)
{
    for (std::vector<Client>::iterator it = channel.getClients().begin(); it != channel.getClients().end(); it++){
        if (My_serv.getPolls()[clientId].fd == it->getFd())
            return (it->getNickname());
    }
    return ("");
}

bool Operators::there_is_channel(Server &My_serv, std::string channel, int &channel_pos, int clientId)
{
    std::vector<Channel> Channels = My_serv.getChannels();
    for (std::vector<Channel>::iterator it = Channels.begin(); it != Channels.end(); it++)
    {
        if (channel == it->getChannelName())
        {
            channel_pos = std::distance(Channels.begin(), it);
            return (true);
        }
    }
    std::string str = ERR_NOSUCHCHANNEL(channel);
    send(My_serv.getPolls()[clientId].fd, str.c_str(), str.length(), 0);
    return (false);
}

bool Operators::already_on_channel(Server &My_serv, Channel &channel, std::string client, int clientId, int check)
{
    std::vector<Client>::iterator it;
    std::vector<Client> clients_it = channel.getClients();
    for (it = clients_it.begin(); it != clients_it.end(); it++){
        if (client == it->getNickname())
        {
            if (check == 1)
            {
                std::string str = ERR_USERONCHANNEL(client, client, channel.getChannelName());
                send(My_serv.getPolls()[clientId].fd, str.c_str(), str.length(), 0);
            }
            return (true);
        }
    }
    if (!check)
    {
        std::string str = ERR_USERNOTINCHANNEL(client, channel.getChannelName());
        send(My_serv.getPolls()[clientId].fd, str.c_str(), str.length(), 0);
    }
    return (false);
}

int Operators::Get_Channel_client_pos(Channel &channel, const std::string& nickname)
{
    std::vector<Client>& clients = channel.getClients();
    std::vector<Client>::iterator it;
    for (it = clients.begin(); it != clients.end(); ++it)
    {
        if (nickname == it->getNickname())
            return std::distance(clients.begin(), it);
    }
    return -1; 
}

bool Operators::there_is_user(Server &My_serv, std::string client_nick, int clientId)
{
    std::vector<Client> client = My_serv.getClients();
    for (std::vector<Client>::iterator it = client.begin(); it < client.end(); it++) {
        if (client_nick == it->getNickname())
            return true;
    }
    std::string str = ERR_NOSUCHNICK(client_nick);
    send(My_serv.getPolls()[clientId].fd, str.c_str(), str.length(), 0);
    return false;
}

bool Operators::Check_Channel_Op(Server &My_serv, Channel &channel, std::string client_nick, int clientId)
{
    std::vector<std::string>::iterator it;
    std::vector<std::string> operators = channel.getOperators();
    if (operators.empty())
        return(false);
    for (it = operators.begin(); it != operators.end(); it++){
        if (client_nick == (*it))
            return (true);
    }
    std::string str = ERR_CHANOPRIVSNEEDED(channel.getChannelName());
    send(My_serv.getPolls()[clientId].fd, str.c_str(), str.length(), 0);
    return (false);
}