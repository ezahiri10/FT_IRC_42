/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fagr <ael-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 16:44:48 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/02/26 17:12:49 by ael-fagr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Channel.hpp"
#include "../Server.hpp"
#include "../Replies.hpp"
#include <set>
#include <iostream>

void Set_New_topic(Server *My_serv, std::string channel, std::string topic, int channel_pos, int client_Fd)
{
    std::string str = RPL_TOPIC(My_serv->clients[client_Fd].getNickname(), channel, topic);
    send(My_serv->polls[client_Fd].fd, str.c_str(), str.length(), 0);
    // RPL_TOPIC(My_serv->clients[client_Fd].getNickname(), channel, topic);
    My_serv->channels[channel_pos].setTopic(topic);
}
void Check_topic(Server *My_serv, std::string channel, std::string topic, int channel_pos, int client_Fd){
    if (topic.empty())
    {
        if (My_serv->channels[channel_pos].getTopic().empty()){
            std::string str = RPL_NOTOPIC(My_serv->clients[client_Fd].getNickname(), channel);
            send(My_serv->polls[client_Fd].fd, str.c_str(), str.length(), 0);
        }
        else{
            std::string str =  My_serv->clients[client_Fd].getNickname() + " " + channel + " " + My_serv->channels[channel_pos].getTopic() + '\n';
            send(My_serv->polls[client_Fd].fd, str.c_str(), str.length(), 0);
        }
    }
    else {
        // RPL_TOPIC();
        std::cout << "Was Here \n";
        Set_New_topic(My_serv, channel, topic, channel_pos, client_Fd);
    }

}

void Server::Add_topic(std::string channel, std::string topic, int FD)
{
    int channel_pos = 0;
    if (there_is_channel(channel, channel_pos, FD)
        && already_on_channel(Get_client_nick(FD, channel_pos), channel, FD, channel_pos, 0))
    {
        Check_topic(this, channel, topic, channel_pos, FD);
    }
}

int Server::Topic_func(std::string arg, int FD)
{
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