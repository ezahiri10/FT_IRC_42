/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fagr <ael-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 16:44:48 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/02/27 17:50:23 by ael-fagr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Channel.hpp"
#include "../Server.hpp"
#include "../Replies.hpp"
#include <set>
#include <iostream>

void Set_New_topic(Server *My_serv, std::string channel, std::string topic, int channel_pos, int client_Fd)
{
    if (!My_serv->clients.empty())
    {
        std::string str = RPL_TOPIC(My_serv->clients[client_Fd].getNickname(), channel, topic);
        send(My_serv->polls[client_Fd].fd, str.c_str(), str.length(), 0);
        My_serv->channels[channel_pos].setTopic(topic);
    }
}

void Desplay_topic(Server *My_serv, std::string channel, int channel_pos, int client_Fd){

    if (My_serv->channels.empty() || My_serv->clients.empty())
        return ;

    if (My_serv->channels[channel_pos].getTopic().empty()){
        std::string str = RPL_NOTOPIC(My_serv->clients[client_Fd].getNickname(), channel);
        send(My_serv->polls[client_Fd].fd, str.c_str(), str.length(), 0);
    }
    else{
        std::string str =  My_serv->clients[client_Fd].getNickname() + " " + channel + " " + My_serv->channels[channel_pos].getTopic() + '\n';
        send(My_serv->polls[client_Fd].fd, str.c_str(), str.length(), 0);
    }
}

void Server::Add_topic(std::string channel, std::string topic, int FD)
{
    if (this->channels.empty())
        return ;

    int channel_pos = 0;
    if (there_is_channel(channel, channel_pos, FD)
        && already_on_channel(Get_client_nick(FD, channel_pos), channel, FD, channel_pos, 0))
    {
        if (topic.empty())
            Desplay_topic(this, channel, channel_pos, FD);
        else
        {
            if (this->channels[channel_pos].getistopic()
                && Check_Channel_Op(Get_client_nick(FD, channel_pos), channel, channel_pos, FD))
                Set_New_topic(this, channel, topic, channel_pos, FD);
            else if (!this->channels[channel_pos].getistopic())
                Set_New_topic(this, channel, topic, channel_pos, FD);
        }
    }
}

int Server::Topic_func(std::string arg, int FD)
{
    if (this->polls.empty())
        return (false);

    std::string channel;
    std::string topic;

    for (size_t i = 0; i < this->args.size(); i++){
        if (i == 1)
            channel = this->args[1];
        if (i == 2)
            topic = this->args[2];
    }
    if (channel.empty())
    {
        std::string str = ERR_NEEDMOREPARAMS(arg);
        send(this->polls[FD].fd, str.c_str(), str.length(), 0);
    }
    else
        Add_topic(channel, topic, FD);
    return (true);
}