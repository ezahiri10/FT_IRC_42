/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fagr <ael-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 19:13:59 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/02/28 19:34:13 by ael-fagr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Channel.hpp"
#include "../Server.hpp"
#include "../Replies.hpp"
#include <set>
#include <iostream>

int ADD_client(Server *My_serv, std::string invit_client, std::string channel, int channel_pos)
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
        My_serv->channels[channel_pos].addClient(*tmp);
        return (true);
    }
    std::cerr << invit_client << " " << channel << ":There was no such nickname" << std::endl;
    return (false);
}

int Server::Invite_client(std::string arg, std::string invit_client, std::string channel, int Client_id)
{
    if (invit_client.empty() || channel.empty())
    {
        std::string str = ERR_NEEDMOREPARAMS(arg);
        send(this->polls[Client_id].fd, str.c_str(), str.length(), 0);
        return (false);
    }
    int channel_pos = 0;
    //print all users in the channel
    
    if (there_is_channel(channel, channel_pos, Client_id)
        && there_is_user(invit_client, Client_id)
        && already_on_channel(Get_client_nick(Client_id, channel_pos), channel, Client_id, channel_pos, 0)
        && !already_on_channel(invit_client, channel, Client_id, channel_pos, 1)
        && Check_Channel_Op(Get_client_nick(Client_id, channel_pos), channel, channel_pos, Client_id))
    {
        ADD_client(this, invit_client, channel, channel_pos);
    }  
    return(false);
}

int Server::Invite_func(std::string arg, int Client_id)
{
    std::string channel;
    std::string invit_client;

    for (size_t i = 0; i < this->args.size(); i++){
        if (i == 1)
            invit_client = this->args[1];
        if (i == 2)
            channel = this->args[2];
    }
    if (this->channels.empty())
    {
        std::string str = ERR_NOSUCHCHANNEL(channel);
        send(this->polls[Client_id].fd, str.c_str(), str.length(), 0);
        return (0);
    }
    else
        Invite_client(arg, invit_client, channel, Client_id);
    return 0;
}
