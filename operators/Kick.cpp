/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fagr <ael-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 15:48:45 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/02/25 19:45:29 by ael-fagr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Channel.hpp"
#include "../Server.hpp"
#include "../Replies.hpp"
#include <set>
#include <iostream>

//KICK #&food nickname reason

bool Server::Check_kick(std::string channel, std::string client, std::string reasen, int client_Fd)
{

    int index = 0;
    // std::cout << "Channel = " << channel << " Client = " << client << " Reasen = " << reasen << std::endl;
    if (there_is_Fd(client_Fd)
        && there_is_channel(channel, index, client_Fd)
        && already_on_channel(Get_client_nick(client_Fd), channel, client_Fd, index, 0)
        && there_is_user(client, client_Fd))
    {
        std::string msg = client + " Kick The Channell " + channel;
        if (!reasen.empty())
            msg += " : " + reasen + '\n';
        else
            msg += '\n';
        send(client_Fd, msg.c_str(), msg.length(), 0);
        int Client_index = Get_client_pos(client, index);
        std::cout << "Clien index = " << Client_index << std::endl;
        if (Client_index != -1)
            this->channels[index].removeClient(Client_index);
        // if(My_serv->channels[index].getClients().empty())
        //     My_serv->channels.erase(My_serv->channels.begin() + index);
    }
    else
    {
        std::string str = ERR_NOSUCHCHANNEL(channel);
        send(client_Fd, str.c_str(), str.length(), 0);
    }

    return (false);
}

int Server::Kick_func(std::string arg, int client_Fd)
{
    std::string client;
    std::string channel;
    std::string reasen;

    for (size_t i = 0; i < this->args.size(); i++){
        if (i == 1)
            channel = this->args[1];
        if (i == 2)
            client = this->args[2];
        if (i == 3)
            reasen = this->args[3];
    }
    // std::cout << "Channel = " << channel << " Client = " << client << " Reasen = " << reasen << std::endl;
    if (channel.empty() || client.empty())
    {
        std::string str = ERR_NEEDMOREPARAMS(arg);
        send(client_Fd, str.c_str(), str.length(), 0);
    }
    else
        Check_kick(channel, client, reasen, client_Fd);

    return (0);
}
