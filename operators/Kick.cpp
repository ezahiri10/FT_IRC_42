/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fagr <ael-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 15:48:45 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/02/28 19:33:53 by ael-fagr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Channel.hpp"
#include "../Server.hpp"
#include "../Replies.hpp"
#include <set>
#include <iostream>

//KICK #&food nickname reason

bool Server::Check_kick(std::string channel, std::string client, std::string reasen, int Client_id)
{
    int channel_pos = 0;
    if (there_is_channel(channel, channel_pos, Client_id)
        && already_on_channel(Get_client_nick(Client_id, channel_pos), channel, Client_id, channel_pos, 0)
        && there_is_user(client, Client_id))
    {
        std::string msg = client + " Kick The Channell " + channel;
        if (!reasen.empty())
            msg += " : " + reasen + '\n';
        else
            msg += '\n';
        send(this->polls[Client_id].fd, msg.c_str(), msg.length(), 0);
        int Client_index = Get_Channel_client_pos(client, channel_pos);
        if (Client_index != -1)
            this->channels[channel_pos].removeClient(Client_index);
        if(this->channels[channel_pos].getClients().empty())
            this->channels.erase(this->channels.begin() + channel_pos);
    }
    else
    {
        std::string str = ERR_NOSUCHCHANNEL(channel);
        send(this->polls[Client_id].fd, str.c_str(), str.length(), 0);
    }

    return (false);
}

int Server::Kick_func(std::string arg, int Client_id)
{
    if (this->polls.empty())
        return (false);

    std::string client;
    std::string channel;
    std::string reasen;

    for (size_t i = 0; i < this->args.size(); i++){
        if (i == 1)
            channel = this->args[1];
        if (i == 2)
            client = this->args[2];
        if (i == 3)
            reasen = this->args[3];
    }
    if (this->channels.empty())
    {
        std::string str = ERR_NOSUCHCHANNEL(channel);
        send(this->polls[Client_id].fd, str.c_str(), str.length(), 0);
        return (0);
    }
    else if (channel.empty() || client.empty())
    {
        std::string str = ERR_NEEDMOREPARAMS(arg);
        send(this->polls[Client_id].fd, str.c_str(), str.length(), 0);
    }
    else
        Check_kick(channel, client, reasen, Client_id);

    return (0);
}
