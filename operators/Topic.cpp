/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fagr <ael-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 16:44:48 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/03/08 22:44:06 by ael-fagr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Operators.hpp"

void Operators::SetNewTopic(Channel  &channel, std::string topic, int Client_id)
{
    if (!getMyserv()->clients.empty())
    {
        std::string str = RPL_TOPIC(getMyserv()->clients[Client_id - 1].getNickname(), channel.getChannelName(), topic);
        SendMessage(channel, str);
        channel.setTopic(topic);
    }
}

void Operators::DesplayTopic(Channel  &channel, int Client_id){

    if (getMyserv()->channels.empty() || getMyserv()->clients.empty())
        return ;
    std::string str =  RPL_TOPIC(getMyserv()->clients[Client_id - 1].getNickname(), channel.getChannelName(), channel.getTopic());
    send(getMyserv()->polls[Client_id].fd, str.c_str(), str.length(), 0);
}

void Operators::AddTopic(Channel  &channel, std::string topic, int Client_id)
{
    if (AlreadyOnChannel(channel, GetClientNick(channel, Client_id), Client_id, 0))
    {
        if (topic.empty())
            DesplayTopic(channel, Client_id);
        else
        {
            if (channel.getIstopic()
                && CheckChannelOp(channel, GetClientNick(channel, Client_id), Client_id))
                SetNewTopic(channel, topic, Client_id);
            else if (!channel.getIstopic())
                SetNewTopic(channel, topic, Client_id);
        }
    }
}

int Operators::TopicFunc(Server &My_serv, std::string arg, int Client_id)
{
    Operators op;

    std::string channel;
    std::string topic;

    op.setMyserv(My_serv);
    std::vector<std::string> args = op.getMyserv()->args;
    for (size_t i = 0; i < args.size(); i++){
        if (i == 1)
            channel = args[1];
        if (i == 2)
            topic = args[2];
    }
    if (op.getMyserv()->channels.empty())
    {
        std::string str = ERR_NOSUCHCHANNEL(channel);
        send(op.getMyserv()->polls[Client_id].fd, str.c_str(), str.length(), 0);   
        return (0);
    }
    else if (channel.empty())
    {
        std::string str = ERR_NEEDMOREPARAMS(arg);
        send(op.getMyserv()->polls[Client_id].fd, str.c_str(), str.length(), 0);
    }
    else
    {
        int channel_pos = -1;
        if (op.ThereIsChannel(channel, channel_pos, Client_id)){
            if (channel_pos == -1)
                return (1);
            op.AddTopic(op.getMyserv()->channels[channel_pos], topic, Client_id);
        }

    }
    return (true);
}