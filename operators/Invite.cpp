/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fagr <ael-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 19:13:59 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/02/24 04:42:01 by ael-fagr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "operators.hpp"

bool Check_Channel_Op(Server *My_serv, std::string channel, int index, int client_Fd)
{
    std::string client_nick = Get_client_nick(My_serv, client_Fd);
    std::vector<std::string>::iterator it;
    for (it = My_serv->channels[index].getOperators().begin(); it != My_serv->channels[index].getOperators().end(); it++){
        if (client_nick == (*it))
            return (true);
    }
    std::string str = ERR_CHANOPRIVSNEEDED(channel);
    send_message(My_serv, str, client_Fd);
    return (false);
}

int ADD_client(Server *My_serv, std::string invit_client, std::string channel, int index)
{
    Client* tmp = NULL;

    for (std::vector<Client>::iterator it = My_serv->clients.begin(); it != My_serv->clients.end(); it++){
        if (invit_client == it->getNickname())
        {
            tmp = &(*it);
            break;
        }
    }
    if (tmp)
    {
        My_serv->channels[index].addClient(*tmp);
        return (true);
    }
    std::cerr << invit_client << " " << channel << ":There was no such nickname" << std::endl;
    return (false);
}

int Invite_client(Server *My_serv, std::string arg, std::string invit_client, std::string channel, int client_Fd)
{
    if (invit_client.empty() || channel.empty())
    {
        std::string str = ERR_NEEDMOREPARAMS(arg);
        send_message(My_serv, str, client_Fd);
        return (false);
    }
    int index = 0;
    if (there_is_channel(My_serv, channel, index, client_Fd)
        && already_on_channel(My_serv, Get_client_nick(My_serv, client_Fd), channel, client_Fd, index, 0)
        && there_is_user(My_serv, Get_client_nick(My_serv, client_Fd), client_Fd)
        && there_is_user(My_serv, invit_client, client_Fd)
        && !already_on_channel(My_serv, invit_client, channel, client_Fd, index, 1)
        && Check_Channel_Op(My_serv, channel, index, client_Fd))
    {
        ADD_client(My_serv, invit_client, channel, index);
    }  
    return(false);
}

int Invite_func(Server *My_serv, std::string arg, int client_Fd)
{
    std::string channel;
    std::string invit_client;
    std::vector<std::string> ops;

    ops.push_back("kkkk");

    My_serv->channels.push_back(Channel());
    Channel &My_channel = My_serv->channels.back();
    My_channel.setName("#CHANNEL");

    // Create client
    My_serv->clients.push_back(Client());
    Client &My_client = My_serv->clients.back();
    My_client.setFd(client_Fd);
    My_client.setNickname("CHIDORI");

    // Add client to the channel
    My_channel.addClient(My_client);
    My_channel.setOperators(ops);

    split_3_arguments(arg, invit_client, channel);
    Invite_client(My_serv, arg, invit_client, channel, client_Fd);

    return 0;
}
