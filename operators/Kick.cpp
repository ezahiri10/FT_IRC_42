/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fagr <ael-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 15:48:45 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/02/27 18:07:59 by ael-fagr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Channel.hpp"
#include "../Server.hpp"
#include "../Replies.hpp"
#include <set>
#include <iostream>

//KICK #&food nickname reason

bool Server::Check_kick(std::string channel, std::string client, std::string reasen, int FD)
{
    int channel_pos = 0;
    if (there_is_channel(channel, channel_pos, FD)
        && already_on_channel(Get_client_nick(FD, channel_pos), channel, FD, channel_pos, 0)
        && there_is_user(client, FD))
    {
        std::string msg = client + " Kick The Channell " + channel;
        if (!reasen.empty())
            msg += " : " + reasen + '\n';
        else
            msg += '\n';
        send(this->polls[FD].fd, msg.c_str(), msg.length(), 0);
        int Client_index = Get_Channel_client_pos(client, channel_pos);
        if (Client_index != -1)
            this->channels[channel_pos].removeClient(Client_index);
        if(this->channels[channel_pos].getClients().empty())
            this->channels.erase(this->channels.begin() + channel_pos);
    }
    else
    {
        std::string str = ERR_NOSUCHCHANNEL(channel);
        send(this->polls[FD].fd, str.c_str(), str.length(), 0);
    }

    return (false);
}

int Server::Kick_func(std::string arg, int FD)
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
    if (channel.empty() || client.empty())
    {
        std::string str = ERR_NEEDMOREPARAMS(arg);
        send(this->polls[FD].fd, str.c_str(), str.length(), 0);
    }
    else
        Check_kick(channel, client, reasen, FD);

    return (0);
}
