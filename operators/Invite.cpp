/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fagr <ael-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 19:13:59 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/03/08 20:29:40 by ael-fagr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Operators.hpp"

int Operators::ADD_client(Channel &channel, std::string nick_name, std::string invit_client, int Client_id)
{
    std::string str;
    channel.addInvitations(invit_client);
    str = RPL_INVITE(nick_name, invit_client, channel.getChannelName());
    send(getMyserv()->polls[Client_id - 1].fd, str.c_str(), str.size(), 0);
    return (true);
}

bool Operators::has_invited(Channel &channel, std::string client_nick)
{
    std::vector<std::string>::iterator it;
    std::vector<std::string> clients = channel.getInvitations();
    for (it = clients.begin(); it != clients.end(); it++){
        if (client_nick == (*it))
            return (true);
    }
    return (false);
}

int Operators::Invite_client(Channel &channel, std::string invit_client, int Client_id)
{
    if (there_is_user(invit_client, Client_id)
        && !has_invited(channel, invit_client))
    {
        ADD_client(channel, Get_client_nick(channel, Client_id), invit_client, Client_id);
    }  
    return(false);
}

int Operators::Invite_func(Server &My_serv, std::string arg, int Client_id)
{
    Operators op;
    std::string channel;
    std::string invit_client;

    op.setMyserv(My_serv);
    std::vector<std::string> args = op.getMyserv()->args;
    for (size_t i = 0; i < args.size(); i++){
        if (i == 1)
            invit_client = args[1];
        if (i == 2)
            channel = args[2];
    }
    if (op.getMyserv()->channels.empty())
    {
        std::string str = ERR_NOSUCHCHANNEL(channel);
        send(op.getMyserv()->polls[Client_id].fd, str.c_str(), str.length(), 0);
        return (0);
    }
    else
    {
        if (invit_client.empty() || channel.empty())
        {
            std::string str = ERR_NEEDMOREPARAMS(arg);
            send(op.getMyserv()->polls[Client_id].fd, str.c_str(), str.length(), 0);
            return (false);
        }
        int channel_pos = -1;
        if (op.there_is_channel(channel, channel_pos, Client_id)){
            if (channel_pos == -1)
                return (1);
            op.Invite_client(op.getMyserv()->channels[channel_pos], invit_client, Client_id);
        }
    }
    return 0;
}
