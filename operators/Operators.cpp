/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Operators.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fagr <ael-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 21:03:24 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/03/08 22:42:54 by ael-fagr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Operators.hpp"


Operators::Operators(){
    this->Myserv = NULL;
}
Operators::~Operators(){
    
}
Server *Operators::getMyserv() const
{
    return (this->Myserv);
}
void Operators::setMyserv(Server &server)
{
    this->Myserv = &server;
}

std::string getUsers(Channel &channel){
    std::vector<std::string> allUsers;
    std::vector<std::string> operators;
    std::vector<Client>::iterator iter;
    std::vector<Client> clients = channel.getClients();
    std::string name;
    operators = channel.getOperators();
    for(iter = clients.begin(); iter != clients.end(); iter++){
        for(size_t i = 0; i < operators.size(); i++){
            if (iter->getNickname() == operators[i])
                name = "@"+iter->getNickname();
            else
                name = iter->getNickname();
        }
        allUsers.push_back(name);
    }
    std::string usersList = "";
    for (size_t i = 0; i < allUsers.size(); ++i) {
        usersList += allUsers[i];
        if (i < allUsers.size() - 1) usersList += " ";
    }
    return usersList;
}

void Operators::SendMessage(Channel &channel, std::string str)
{
    std::string users = getUsers(channel);
    std::string new_str;
    std::vector<Client>::iterator it;
    std::vector<Client> clients_it = channel.getClients();
    for (it = clients_it.begin(); it != clients_it.end(); it++){
        send(it->getFd(), str.c_str(), str.length(), 0);
        new_str = RPL_NAMREPLY(it->getNickname(), channel.getChannelName(), users);
        send(it->getFd(), new_str.c_str(), new_str.length(), 0);
    }
}


std::string Operators::GetClientNick(Channel &channel, int clientId)
{
    for (std::vector<Client>::iterator it = channel.getClients().begin(); it != channel.getClients().end(); it++){
        if (getMyserv()->polls[clientId].fd == it->getFd())
            return (it->getNickname());
    }
    return ("");
}

bool Operators::ThereIsChannel(std::string channel, int &channel_pos, int clientId)
{
    std::vector<Channel> Channels = getMyserv()->channels;
    for (std::vector<Channel>::iterator it = Channels.begin(); it != Channels.end(); it++)
    {
        if (channel == it->getChannelName())
        {
            channel_pos = std::distance(Channels.begin(), it);
            return (true);
        }
    }
    std::string str = ERR_NOSUCHCHANNEL(channel);
    send(getMyserv()->polls[clientId].fd, str.c_str(), str.length(), 0);
    return (false);
}

bool Operators::AlreadyOnChannel(Channel &channel, std::string client, int clientId, int check)
{
    std::vector<Client>::iterator it;
    std::vector<Client> clients_it = channel.getClients();
    for (it = clients_it.begin(); it != clients_it.end(); it++){
        if (client == it->getNickname())
        {
            if (check == 1)
            {
                std::string str = ERR_USERONCHANNEL(client, client, channel.getChannelName());
                send(getMyserv()->polls[clientId].fd, str.c_str(), str.length(), 0);
            }
            return (true);
        }
    }
    if (!check)
    {
        std::string str = ERR_USERNOTINCHANNEL(client, channel.getChannelName());
        send(getMyserv()->polls[clientId].fd, str.c_str(), str.length(), 0);
    }
    return (false);
}

int Operators::GetChannelClientPos(Channel &channel, const std::string& nickname)
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

bool Operators::ThereIsUser(std::string client_nick, int clientId)
{
    std::vector<Client> client = getMyserv()->clients;
    for (std::vector<Client>::iterator it = client.begin(); it < client.end(); it++) {
        if (client_nick == it->getNickname())
            return true;
    }
    std::string str = ERR_NOSUCHNICK(client_nick);
    send(getMyserv()->polls[clientId].fd, str.c_str(), str.length(), 0);
    return false;
}

bool Operators::CheckChannelOp(Channel &channel, std::string client_nick, int clientId)
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
    send(getMyserv()->polls[clientId].fd, str.c_str(), str.length(), 0);
    return (false);
}