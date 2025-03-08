/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fagr <ael-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 15:48:45 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/03/08 20:44:29 by ael-fagr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Operators.hpp"

bool Operators::Check_kick(Channel &channel, std::string client, std::string reasen, int Client_id)
{
    if (already_on_channel(channel, Get_client_nick(channel, Client_id), Client_id, 0)
        && there_is_user(client, Client_id)
        && Check_Channel_Op(channel, Get_client_nick(channel, Client_id), Client_id))
    {
        std::string msg = ":IRCServer KICK " + channel.getChannelName() + " " + client;
        if (reasen.empty())
            msg += POSTFIX;
        else
            msg += ": " + reasen + POSTFIX;
        
        int Client_index = Get_Channel_client_pos(channel, client);
        if (Client_index != -1)
        {
            send_message(channel, msg);
            channel.removeClient(Client_index);
        }
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

    op.setMyserv(My_serv);
    std::vector<std::string> args = op.getMyserv()->args;
    for (size_t i = 0; i < args.size(); i++){
        if (i == 1)
            channel = args[1];
        if (i == 2)
            client = args[2];
        if (i == 3)
            reasen = args[3];
    }
    if (op.getMyserv()->channels.empty())
    {
        std::string str = ERR_NOSUCHCHANNEL(channel);
        send(op.getMyserv()->polls[Client_id].fd, str.c_str(), str.length(), 0);
        return (0);
    }
    else if (channel.empty() || client.empty())
    {
        std::string str = ERR_NEEDMOREPARAMS(arg);
        send(op.getMyserv()->polls[Client_id].fd, str.c_str(), str.length(), 0);
    }
    else
    {
        int channel_pos = -1;
        if (op.there_is_channel(channel, channel_pos, Client_id)){
            if (channel_pos == -1)
                return (1);
            op.Check_kick(op.getMyserv()->channels[channel_pos], client, reasen, Client_id);
            if(op.getMyserv()->channels[channel_pos].getClients().empty())
                op.getMyserv()->channels.erase(op.getMyserv()->channels.begin() + channel_pos);
        }
    }

    return (0);
}
