/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fagr <ael-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 19:13:59 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/02/25 20:23:50 by ael-fagr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Channel.hpp"
#include "../Server.hpp"
#include "../Replies.hpp"
#include <set>
#include <iostream>


bool Check_Channel_Op(Server *My_serv, std::string client_nick, std::string channel, int index, int client_Fd)
{
    std::vector<std::string>::iterator it;
    for (it = My_serv->channels[index].getOperators().begin(); it != My_serv->channels[index].getOperators().end(); it++){
        if (client_nick == (*it))
            return (true);
    }
    std::string str = ERR_CHANOPRIVSNEEDED(channel);
    send(client_Fd, str.c_str(), str.length(), 0);
    return (false);
}

int ADD_client(Server *My_serv, std::string invit_client, std::string channel, int index)
{
    Client* tmp = NULL;

    for (std::vector<Client>::iterator it = My_serv->clients.begin(); it != My_serv->clients.end(); it++){
        if (invit_client == it->getNickname())
        {
            tmp = &(*it);
            break;
        }
    }
    if (tmp)
    {
        My_serv->channels[index].addClient(*tmp);
        return (true);
    }
    std::cerr << invit_client << " " << channel << ":There was no such nickname" << std::endl;
    return (false);
}

int Server::Invite_client(std::string arg, std::string invit_client, std::string channel, int client_Fd)
{
    if (invit_client.empty() || channel.empty())
    {
        std::string str = ERR_NEEDMOREPARAMS(arg);
        send(client_Fd, str.c_str(), str.length(), 0);
        return (false);
    }
    int index = 0;
    //print all users in the channel
    
    if (there_is_channel(channel, index, client_Fd)
        && there_is_user(Get_client_nick(client_Fd), client_Fd)
        // && there_is_user(invit_client, client_Fd)
        && already_on_channel(Get_client_nick(client_Fd), channel, client_Fd, index, 0)
        /*&& !already_on_channel(invit_client, channel, client_Fd, index, 1)*/
        && Check_Channel_Op(this, Get_client_nick(client_Fd), channel, index, client_Fd))
    {
        ADD_client(this, invit_client, channel, index);
    }  
    return(false);
}

int Server::Invite_func(std::string arg, int client_Fd)
{
    std::string channel;
    std::string invit_client;

    for (size_t i = 0; i < this->args.size(); i++){
        if (i == 1)
            invit_client = this->args[1];
        if (i == 2)
            channel = this->args[2];
    }
    Invite_client(arg, invit_client, channel, client_Fd);
    return 0;
}
