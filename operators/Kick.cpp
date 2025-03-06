/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fagr <ael-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 15:48:45 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/03/06 17:09:35 by ael-fagr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Operators.hpp"

bool Operators::Check_kick(Server &My_serv, Channel &channel, std::string client, std::string reasen, int Client_id)
{
    if (already_on_channel(My_serv, channel, Get_client_nick(My_serv, channel, Client_id), Client_id, 0)
        && there_is_user(My_serv, client, Client_id)
        && Check_Channel_Op(My_serv, channel, Get_client_nick(My_serv, channel, Client_id), Client_id))
    {
        std::string msg = ":IRCServer KICK " + channel.getChannelName() + " " + client;
        if (reasen.empty())
            msg += POSTFIX;
        else
            msg += ": " + reasen + POSTFIX;
        send_message(channel, msg);
        int Client_index = Get_Channel_client_pos(channel, client);
        if (Client_index != -1)
            channel.removeClient(Client_index);
    }
    return (true);
}

int Operators::Kick_func(Server &My_serv, std::string arg, int Client_id)
{
    Operators op;
    if (My_serv.polls.empty())
        return (false);

    std::string client;
    std::string channel;
    std::string reasen;

    for (size_t i = 0; i < My_serv.args.size(); i++){
        if (i == 1)
            channel = My_serv.args[1];
        if (i == 2)
            client = My_serv.args[2];
        if (i == 3)
            reasen = My_serv.args[3];
    }
    if (My_serv.channels.empty())
    {
        std::string str = ERR_NOSUCHCHANNEL(channel);
        send(My_serv.polls[Client_id].fd, str.c_str(), str.length(), 0);
        return (0);
    }
    else if (channel.empty() || client.empty())
    {
        std::string str = ERR_NEEDMOREPARAMS(arg);
        send(My_serv.polls[Client_id].fd, str.c_str(), str.length(), 0);
    }
    else
    {
        int channel_pos = -1;
        if (op.there_is_channel(My_serv, channel, channel_pos, Client_id)){
            if (channel_pos == -1)
                return (1);
            op.Check_kick(My_serv, My_serv.channels[channel_pos], client, reasen, Client_id);
            if(My_serv.channels[channel_pos].getClients().empty())
                My_serv.channels.erase(My_serv.channels.begin() + channel_pos);
        }
    }

    return (0);
}
