/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fagr <ael-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 21:03:24 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/02/25 20:05:47 by ael-fagr         ###   ########.fr       */
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

std::string Server::Get_client_nick(int client_Fd)
{
    std::vector<Client>::iterator it;
    for (it = this->clients.begin(); it != this->clients.end(); it++){
        if (client_Fd == (*it).getFd())
        {
            return ((*it).getNickname());
        }
    }
    return ("");
}

bool Server::there_is_channel(std::string channel, int &index, int clientFD)
{
    for (std::vector<Channel>::iterator it = this->channels.begin(); it != this->channels.end(); it++)
    {
        std::cout << "Channel = " << it->getChannelName() << std::endl;
        if (channel == it->getChannelName())
        {
            index = std::distance(this->channels.begin(), it);
            return (true);
        }
    }
    std::string str = ERR_NOSUCHCHANNEL(channel);
    send(clientFD, str.c_str(), str.length(), 0);
    return (false);
}

bool Server::already_on_channel(std::string client, std::string channel, int client_Fd, int index, int check)
{
    std::vector<Client>::iterator it;
    for (it = this->channels[index].getClients().begin(); it != this->channels[index].getClients().end(); it++){
        if (client == it->getNickname())
        {
            if (check == 1)
            {
                std::string str = ERR_USERONCHANNEL(client, client, channel);
                send(client_Fd, str.c_str(), str.length(), 0);
            }
            return (true);
        }
    }
    std::string str = ERR_USERNOTINCHANNEL(client, channel);
    send(client_Fd, str.c_str(), str.length(), 0);
    return (false);
}

int Server::Get_client_pos(const std::string& nickname, int index)
{
    std::vector<Client>& clients = this->channels[index].getClients();
    std::vector<Client>::iterator it;
    if (clients.empty())
        std::cout << "WAS HERE\n";
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
    send(client_Fd, str.c_str(), str.length(), 0);
    return false;
}