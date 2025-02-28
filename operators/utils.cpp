/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fagr <ael-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 21:03:24 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/02/28 19:46:06 by ael-fagr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Channel.hpp"
#include "../Server.hpp"
#include "../Replies.hpp"
#include <set>
#include <iostream>

bool Server::there_is_Fd(int fd)
{
    std::vector<pollfd>::iterator it;
    for (it = this->polls.begin(); it != this->polls.end(); it++){
        if (fd == (*it).fd)
            return (true);
    }
    return (false);
}


std::string Server::Get_client_nick(int clientId, int channel_pos)
{
    std::cout << "Clientid = " << clientId << std::endl;
    for (std::vector<Client>::iterator it = this->channels[channel_pos].getClients().begin(); it != this->channels[channel_pos].getClients().end(); it++){
        if (this->polls[clientId].fd == it->getFd())
            return (it->getNickname());
    }
    return ("");
}

bool Server::there_is_channel(std::string channel, int &channel_pos, int clientId)
{
    for (std::vector<Channel>::iterator it = this->channels.begin(); it != this->channels.end(); it++)
    {
        if (channel == it->getChannelName())
        {
            channel_pos = std::distance(this->channels.begin(), it);
            return (true);
        }
    }
    std::string str = ERR_NOSUCHCHANNEL(channel);
    send(this->polls[clientId].fd, str.c_str(), str.length(), 0);
    return (false);
}

bool Server::already_on_channel(std::string client, std::string channel, int clientId, int channel_pos, int check)
{
    std::vector<Client>::iterator it;
    std::vector<Client> clients_it = this->channels[channel_pos].getClients();
    for (it = clients_it.begin(); it != clients_it.end(); it++){
        if (client == it->getNickname())
        {
            if (check == 1)
            {
                std::string str = ERR_USERONCHANNEL(client, client, channel);
                send(this->polls[clientId].fd, str.c_str(), str.length(), 0);
            }
            return (true);
        }
    }
    if (!check)
    {
        std::string str = ERR_USERNOTINCHANNEL(client, channel);
        send(this->polls[clientId].fd, str.c_str(), str.length(), 0);
    }
    return (false);
}

int Server::Get_Channel_client_pos(const std::string& nickname, int channel_pos)
{
    std::vector<Client>& clients = this->channels[channel_pos].getClients();
    std::vector<Client>::iterator it;
    for (it = clients.begin(); it != clients.end(); ++it)
    {
        if (nickname == it->getNickname())
            return std::distance(clients.begin(), it);
    }
    return -1; 
}

bool Server::there_is_user(std::string client, int clientId)
{
    for (std::vector<Client>::iterator it = this->clients.begin(); it < this->clients.end(); it++) {
        if (client == it->getNickname())
            return true;
    }
    std::string str = ERR_NOSUCHNICK(client);
    send(this->polls[clientId].fd, str.c_str(), str.length(), 0);
    return false;
}

bool Server::Check_Channel_Op(std::string client_nick, std::string channel, int channel_pos, int clientId)
{
    std::vector<std::string>::iterator it;
    for (it = this->channels[channel_pos].getOperators().begin(); it != this->channels[channel_pos].getOperators().end(); it++){
        if (client_nick == (*it))
            return (true);
    }
    std::string str = ERR_CHANOPRIVSNEEDED(channel);
    send(this->polls[clientId].fd, str.c_str(), str.length(), 0);
    return (false);
}
