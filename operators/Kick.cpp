/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakazdao <yakazdao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 15:48:45 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/03/03 00:54:11 by yakazdao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Operators.hpp"

//KICK #&food nickname reason

bool Operators::Check_kick(Server &My_serv, std::string channel, std::string client, std::string reasen, int Client_id)
{
    int channel_pos = -1;
    if (there_is_channel(My_serv, channel, channel_pos, Client_id)
        && already_on_channel(My_serv, Get_client_nick(My_serv, Client_id, channel_pos), channel, Client_id, channel_pos, 0)
        && there_is_user(My_serv, client, Client_id)
        && Check_Channel_Op(My_serv, Get_client_nick(My_serv, Client_id, channel_pos), channel, channel_pos, Client_id))
    {
        std::string msg = client + ": Kick The Channell " + channel;
        if (!reasen.empty())
            msg += " : " + reasen + "\r\n";
        else
            msg += "\r\n";
        send_message(My_serv, msg, channel_pos);
        int Client_index = Get_Channel_client_pos(My_serv, client, channel_pos);
        if (Client_index != -1)
            My_serv.channels[channel_pos].removeClient(Client_index);
        if(My_serv.channels[channel_pos].getClients().empty())
            My_serv.channels.erase(My_serv.channels.begin() + channel_pos);
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
        op.Check_kick(My_serv, channel, client, reasen, Client_id);

    return (0);
}
