/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezahiri <ezahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 15:48:45 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/03/13 17:35:36 by ezahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Operators.hpp"

bool Operators::IsAnOwner(Channel &channel, std::string nick_name, int Client_id)
{
    if (channel.getOperators()[0] == nick_name)
    {
        std::stringstream ss;
        ss << ":IRCServer Owner cannot KICK the Channel\r\n";
        std::string msg = ss.str();
        send(getMyserv()->polls[Client_id].fd, msg.c_str(), msg.size(), 0);
        return (true);
    }
    return (false);
}

bool Operators::Check_kick(Channel &channel, std::string client, std::string reasen, int Client_id)
{
    std::string kicker_nick = getClientNick(channel, Client_id);
    if (alreadyOnChannel(channel, kicker_nick, Client_id, 0)
        && thereIsUser(client, Client_id)
        && alreadyOnChannel(channel, client, Client_id, 0)
        && CheckChannelOp(channel, kicker_nick, Client_id)
        && !IsAnOwner(channel, client, Client_id))
    {
        std::string ss = ":IRCServer KICK " + channel.getChannelName() + " " + client;

        if (reasen.empty())
            ss += "\r\n";
        else
            ss += " :" + reasen + "\r\n";
        int Client_index = getChannelClientPos(channel, client);
        int Op_index = getChannelOpPos(channel, client);
        if (Client_index != -1)
        {
            send(GetClientFd(client), ss.c_str(), ss.size(), 0); 
            if (Op_index != -1)  
              channel.removeOperator(Op_index);
            channel.removeClient(Client_index);
            std::string nick_name = getMyserv()->clients[Client_id - 1].getNickname();
            sendMessage(channel, nick_name, ss);
        }
        return (true);
    }
    return (false);
}

int Operators::KickFunc(Server &My_serv, std::string arg, int Client_id)
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
        if (op.thereIsChannel(channel, channel_pos, Client_id)){
            if (channel_pos == -1)
                return (1);
            op.Check_kick(op.getMyserv()->channels[channel_pos], client, reasen, Client_id);
            if(op.getMyserv()->channels[channel_pos].getClients().empty())
                op.getMyserv()->channels.erase(op.getMyserv()->channels.begin() + channel_pos);
        }
    }

    return (0);
}
