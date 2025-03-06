/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fagr <ael-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 16:44:48 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/03/06 17:03:56 by ael-fagr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Operators.hpp"

void Operators::Set_New_topic(Server &My_serv, Channel  &channel, std::string topic, int Client_id)
{
    if (!My_serv.clients.empty())
    {
        std::string str = RPL_TOPIC(My_serv.clients[Client_id - 1].getNickname(), channel.getChannelName(), topic);
        send_message(channel, str);
        channel.setTopic(topic);
    }
}

void Desplay_topic(Server &My_serv, Channel  &channel, int Client_id){

    if (My_serv.channels.empty() || My_serv.clients.empty())
        return ;
    std::string str =  RPL_TOPIC(My_serv.clients[Client_id - 1].getNickname(), channel.getChannelName(), channel.getTopic());
    send(My_serv.polls[Client_id].fd, str.c_str(), str.length(), 0);
}

void Operators::Add_topic(Server &My_serv, Channel  &channel, std::string topic, int Client_id)
{
    if (already_on_channel(My_serv, channel, Get_client_nick(My_serv, channel, Client_id), Client_id, 0))
    {
        if (topic.empty())
            Desplay_topic(My_serv, channel, Client_id);
        else
        {
            if (channel.getistopic()
                && Check_Channel_Op(My_serv, channel, Get_client_nick(My_serv, channel, Client_id), Client_id))
                Set_New_topic(My_serv, channel, topic, Client_id);
            else if (!channel.getistopic())
                Set_New_topic(My_serv, channel, topic, Client_id);
        }
    }
}

int Operators::Topic_func(Server &My_serv, std::string arg, int Client_id)
{
    Operators op;

    std::string channel;
    std::string topic;

    for (size_t i = 0; i < My_serv.args.size(); i++){
        if (i == 1)
            channel = My_serv.args[1];
        if (i == 2)
            topic = My_serv.args[2];
    }
    if (My_serv.channels.empty())
    {
        std::string str = ERR_NOSUCHCHANNEL(channel);
        send(My_serv.polls[Client_id].fd, str.c_str(), str.length(), 0);   
        return (0);
    }
    else if (channel.empty())
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
            op.Add_topic(My_serv, My_serv.channels[channel_pos], topic, Client_id);
        }

    }
    return (true);
}