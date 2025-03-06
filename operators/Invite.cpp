/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fagr <ael-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 19:13:59 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/03/06 19:39:41 by ael-fagr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Operators.hpp"

int ADD_client(Server &My_serv, Channel &channel, std::string nick_name, std::string invit_client, int Client_id)
{
    Client* tmp = NULL;

    for (std::vector<Client>::iterator it = My_serv.clients.begin(); it != My_serv.clients.end(); it++){
        if (invit_client == it->getNickname())
        {
            tmp = &(*it);
            break;
        }
    }
    std::string str;
    if (tmp)
    {
        channel.addClient(*tmp);
        str = RPL_INVITING(nick_name, invit_client, channel.getChannelName());
        send_message(channel, str);
        return (true);
    }
    str = ERR_NOSUCHNICK(invit_client);
    send(My_serv.polls[Client_id].fd, str.c_str(), str.length(), 0);
    return (false);
}

int Operators::Invite_client(Server &My_serv, Channel &channel, std::string invit_client, int Client_id)
{
    if (there_is_user(My_serv, invit_client, Client_id)
        && already_on_channel(My_serv, channel, Get_client_nick(My_serv, channel, Client_id), Client_id, 0)
        && !already_on_channel(My_serv, channel, invit_client, Client_id, 1)
        && Check_Channel_Op(My_serv, channel, Get_client_nick(My_serv, channel, Client_id), Client_id))
    {
        ADD_client(My_serv, channel, Get_client_nick(My_serv, channel, Client_id), invit_client, Client_id);
    }  
    return(false);
}

int Operators::Invite_func(Server &My_serv, std::string arg, int Client_id)
{
    Operators op;
    std::string channel;
    std::string invit_client;

    for (size_t i = 0; i < My_serv.args.size(); i++){
        if (i == 1)
            invit_client = My_serv.args[1];
        if (i == 2)
            channel = My_serv.args[2];
    }
    if (My_serv.channels.empty())
    {
        std::string str = ERR_NOSUCHCHANNEL(channel);
        send(My_serv.polls[Client_id].fd, str.c_str(), str.length(), 0);
        return (0);
    }
    else
    {
        if (invit_client.empty() || channel.empty())
        {
            std::string str = ERR_NEEDMOREPARAMS(arg);
            send(My_serv.polls[Client_id].fd, str.c_str(), str.length(), 0);
            return (false);
        }
        int channel_pos = -1;
        if (op.there_is_channel(My_serv, channel, channel_pos, Client_id)){
            if (channel_pos == -1)
                return (1);
            op.Invite_client(My_serv, My_serv.channels[channel_pos], invit_client, Client_id);
        }
    }
    return 0;
}
