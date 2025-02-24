/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fagr <ael-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 21:03:24 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/02/24 04:57:50 by ael-fagr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "operators.hpp"

int split_3_arguments(std::string arg, std::string &p1, std::string &p2)
{
    std::stringstream ss(arg);
	
    std::string t;

    int i = 0;
    while (ss >> t)
    {
        if (i == 1)
            p1 = t;
        else if (i == 2)
        {
            p2 = t;
            break;
        }
        i++;
    }
    return 1;
}

int split_4_arguments(std::string arg, std::string &channel, std::string &client, std::string &p3)
{
    std::stringstream ss(arg);
	
    std::string t;

    int i = 0;
    while (ss >> t)
    {
        if (i == 1)
            channel = t;
        else if (i == 2)
            client = t;
        else if (i == 3)
        {
            p3 = t;
            break;
        }
        i++;
    }
    return 1;
}

void send_message(Server *My_serv, std::string message, int clientFD)
{
    (void)My_serv;
    // int sock_fd;
    // for (std::vector<Client>::iterator it = My_serv->clients.begin(); it != My_serv->clients.end(); it++)
    // {
    //     sock_fd = (*it).getFd();
        // send(sock_fd, message.c_str(), message.length(), 0);
    // }
    send(clientFD, message.c_str(), message.length(), 0);
}

bool there_is_Fd(Server *My_serv, int fd)
{
    std::vector<pollfd>::iterator it;
    for (it = My_serv->polls.begin(); it != My_serv->polls.end(); it++){
        if (fd == (*it).fd)
            return (true);
    }
    return (false);
}

std::string Get_client_nick(Server *My_serv, int client_Fd)
{
    std::vector<Client>::iterator it;
    for (it = My_serv->clients.begin(); it != My_serv->clients.end(); it++){
        if (client_Fd == (*it).getFd())
        {
            return ((*it).getNickname());
        }
    }
    return ("");
}

bool there_is_channel(Server *My_serv, std::string channel, int &index, int clientFD)
{
    for (std::vector<Channel>::iterator it = My_serv->channels.begin(); it != My_serv->channels.end(); it++)
    {
        if (channel == it->getName())
        {
            index = std::distance(My_serv->channels.begin(), it);
            return (true);
        }
    }
    std::string str = ERR_NOSUCHCHANNEL(channel);
    send(clientFD, str.c_str(), str.length(), 0);
    return (false);
}

bool already_on_channel(Server *My_serv, std::string client, std::string channel, int client_Fd, int index, int check)
{
    if (client.empty())
        return (false);
    std::vector<Client>::iterator it;
    for (it = My_serv->channels[index].getClients().begin(); it != My_serv->channels[index].getClients().end(); it++){
        if (client == it->getNickname())
        {
            if (check == 1)
            {
                std::string str = ERR_USERONCHANNEL(Get_client_nick(My_serv, client_Fd), client, channel);
                send_message(My_serv, str, client_Fd);
            }
            return (true);
        }
    }
    return (false);
}

bool there_is_user(Server *My_serv, std::string client, int client_Fd)
{
    if (!My_serv)
        return false;

    for (std::vector<Client>::iterator it = My_serv->clients.begin(); it < My_serv->clients.end(); it++) {
        if (client == it->getNickname())
            return true;
    }
    std::string str = ERR_NOSUCHNICK(Get_client_nick(My_serv, client_Fd));
    send_message(My_serv, str, client_Fd);
    return false;
}