/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakazdao <yakazdao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 15:48:45 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/02/25 11:43:05 by yakazdao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "operators.hpp"

//KICK #&food nickname reason
//Test Write kick command in irc server
int Get_client_pos(Server *My_serv, const std::string& nickname, int index)
{
    std::vector<Client>& clients = My_serv->channels[index].getClients();
    std::vector<Client>::iterator it;
    if (clients.empty())
        std::cout << "WAS HERE\n";
    for (it = clients.begin(); it != clients.end(); ++it)
    {
        if (nickname == it->getNickname())
            return std::distance(clients.begin(), it);
    }
    return -1; 
}

bool Check_send(Server *My_serv, std::string channel, std::string client, std::string reasen, int client_Fd)
{

    int index = 0;
    if (there_is_Fd(My_serv, client_Fd)
        && there_is_channel(My_serv, channel, index, client_Fd)
        && already_on_channel(My_serv, Get_client_nick(My_serv, client_Fd), channel, client_Fd, index, 0)
        && there_is_user(My_serv, client, client_Fd))
    {
        std::string msg = client + " Kick The Channell " + channel;
        if (!reasen.empty())
            msg += " : " + reasen + '\n';
        else
            msg += '\n';
        send_message(My_serv, msg, client_Fd);
        int Client_index = Get_client_pos(My_serv, client, index);
        std::cout << "Clien index = " << Client_index << std::endl;
        if (Client_index != -1)
            My_serv->channels[index].removeClient(Client_index);
        // if(My_serv->channels[index].getClients().empty())
        //     My_serv->channels.erase(My_serv->channels.begin() + index);
    }
    else
    {
        std::string str = ERR_NOSUCHCHANNEL(channel);
        send_message(My_serv, str, client_Fd);
    }

    return (false);
}

int Kick_func(Server *My_serv, std::string arg, int client_Fd)
{
    Channel My_channel;
    Client My_client;
    std::string client;
    std::string channel;
    std::string reasen;

    My_channel.setChannelName("#MY_CHANNEL");
    My_serv->channels.push_back(My_channel);

    My_client.setFd(client_Fd);
    My_client.setNickname("CHIDORI");
    My_serv->clients.push_back(My_client);

    My_serv->channels.back().addClient(My_serv->clients.back());

    if (arg.empty())
        return (false);
    
    split_4_arguments(arg, channel, client, reasen);
    if (channel.empty() || client.empty())
    {
        std::string str = ERR_NEEDMOREPARAMS(arg);
        send_message(My_serv, str, client_Fd);
    }
    else
        Check_send(My_serv, channel, client, reasen, client_Fd);

    return (0);
}
