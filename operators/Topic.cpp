/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fagr <ael-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 16:44:48 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/02/28 19:45:05 by ael-fagr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Channel.hpp"
#include "../Server.hpp"
#include "../Replies.hpp"
#include <set>
#include <iostream>

void Set_New_topic(Server *My_serv, std::string channel, std::string topic, int channel_pos, int Client_id)
{
    if (!My_serv->clients.empty())
    {
        std::string str = RPL_TOPIC(My_serv->clients[Client_id].getNickname(), channel, topic);
        send(My_serv->polls[Client_id].fd, str.c_str(), str.length(), 0);
        My_serv->channels[channel_pos].setTopic(topic);
    }
}

void Desplay_topic(Server *My_serv, std::string channel, int channel_pos, int Client_id){

    if (My_serv->channels.empty() || My_serv->clients.empty())
        return ;

    if (My_serv->channels[channel_pos].getTopic().empty()){
        std::string str = RPL_NOTOPIC(My_serv->clients[Client_id].getNickname(), channel);
        send(My_serv->polls[Client_id].fd, str.c_str(), str.length(), 0);
    }
    else{
        std::string str =  My_serv->clients[Client_id].getNickname() + " " + channel + " " + My_serv->channels[channel_pos].getTopic() + '\n';
        send(My_serv->polls[Client_id].fd, str.c_str(), str.length(), 0);
    }
}

void Server::Add_topic(std::string channel, std::string topic, int Client_id)
{
    int channel_pos = 0;
    std::cout << "Was here\n";
    if (there_is_channel(channel, channel_pos, Client_id)
        && already_on_channel(Get_client_nick(Client_id, channel_pos), channel, Client_id, channel_pos, 0))
    {
        if (topic.empty())
            Desplay_topic(this, channel, channel_pos, Client_id);
        else
        {
            if (this->channels[channel_pos].getistopic()
                && Check_Channel_Op(Get_client_nick(Client_id, channel_pos), channel, channel_pos, Client_id))
                Set_New_topic(this, channel, topic, channel_pos, Client_id);
            else if (!this->channels[channel_pos].getistopic())
                Set_New_topic(this, channel, topic, channel_pos, Client_id);
        }
    }
}

int Server::Topic_func(std::string arg, int Client_id)
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
    if (this->channels.empty())
    {
        std::string str = ERR_NOSUCHCHANNEL(channel);
        send(this->polls[Client_id].fd, str.c_str(), str.length(), 0);   
        return (0);
    }
    else if (channel.empty())
    {
        std::string str = ERR_NEEDMOREPARAMS(arg);
        send(this->polls[Client_id].fd, str.c_str(), str.length(), 0);
    }
    else
        Add_topic(channel, topic, Client_id);
    return (true);
}