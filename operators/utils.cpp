/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezahiri <ezahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 21:03:24 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/03/03 14:30:33 by ezahiri          ###   ########.fr       */
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


std::string Server::Get_client_nick(int client_Fd, int channel_pos)
{
    for (std::vector<Client>::iterator it = this->channels[channel_pos].getClients().begin(); it != this->channels[channel_pos].getClients().end(); it++){
        if (client_Fd == it->getFd())
            return (it->getNickname());
    }
    return ("");
}

bool Server::there_is_channel(std::string channel, int &channel_pos, int clientFD)
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
    send(this->polls[clientFD].fd, str.c_str(), str.length(), 0);
    return (false);
}

bool Server::already_on_channel(std::string client, std::string channel, int client_Fd, int channel_pos, int check)
{
    std::vector<Client>::iterator it;
    std::vector<Client> clients_it = this->channels[channel_pos].getClients();
    for (it = clients_it.begin(); it != clients_it.end(); it++){
        if (client == it->getNickname())
        {
            if (check == 1)
            {
                std::string str = ERR_USERONCHANNEL(client, client, channel);
                send(this->polls[client_Fd].fd, str.c_str(), str.length(), 0);
            }
            return (true);
        }
    }
    if (!check)
    {
        std::string str = ERR_USERNOTINCHANNEL(client, channel);
        send(this->polls[client_Fd].fd, str.c_str(), str.length(), 0);
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

bool Server::there_is_user(std::string client, int client_Fd)
{
    for (std::vector<Client>::iterator it = this->clients.begin(); it < this->clients.end(); it++) {
        if (client == it->getNickname())
            return true;
    }
    std::string str = ERR_NOSUCHNICK(client);
    send(this->polls[client_Fd].fd, str.c_str(), str.length(), 0);
    return false;
}