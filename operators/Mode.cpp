/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezahiri <ezahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 22:53:47 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/03/01 19:39:04 by ezahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Channel.hpp"
#include "../Server.hpp"
#include "../Replies.hpp"
#include <set>
#include <iostream>

void Add_Remove_PASS(Server *My_serv, std::string mode, std::string pass, int channel_pos){//k
    if (mode == "+k")
        My_serv->channels[channel_pos].setPassword(pass);
    else
        My_serv->channels[channel_pos].setPassword("");
}

void Server::Add_Remove_OP(std::string mode, std::string op, int channel_pos){//o
    if (mode == "+o") {
        if (there_is_user(op, channel_pos))
        {
            std::vector<std::string> ops = this->channels[channel_pos].getOperators();
            if (std::find(ops.begin(), ops.end(), op) == ops.end())
                ops.push_back(op);
        }
    } else {
        std::vector<std::string> ops = this->channels[channel_pos].getOperators();
        ops.erase(std::remove(ops.begin(), ops.end(), op), ops.end());
    }
}

void Add_Remove_LIMIT(Server *My_serv, std::string mode, int user_limit, int channel_pos){//l
    if (mode == "+l")
        My_serv->channels[channel_pos].setChannelLimit(user_limit);
    else
        My_serv->channels[channel_pos].setChannelLimit(10);
}

void Add_Remove_TOPIC(Server *My_serv, std::string mode, std::string topic, int channel_pos){//t
    if (mode == "+t")
        My_serv->channels[channel_pos].setTopic(topic);
    else
        My_serv->channels[channel_pos].setTopic("");
}

void Add_Remove_INVITE(Server *My_serv, std::string mode, std::string invite, int channel_pos){//i
    (void)invite;
    if (mode == "+i")
        My_serv->channels[channel_pos].setInvited(true);
    else
        My_serv->channels[channel_pos].setInvited(false);
}
bool ft_isdigits(std::string identify)
{
    for (size_t i = 0; i < identify.length(); i++)
    {
        if (!isdigit(identify[i]))
        {
            // std::cout << "Enter digits only" << std::endl;
            return (false);
        }
    }
    return (true);
}

bool Check_valid_Mode(std::string client, std::string mode, int client_Fd){
    const std::string modes[] = { 
        "+i", "+t", "+k", "+o", "+l", 
        "-i", "-t", "-k", "-o", "-l"
    };
    std::set<std::string> valid_modes(modes, modes + 10);

    if (valid_modes.find(mode) != valid_modes.end())
        return true;
    std::string str = ERR_UMODEUNKNOWNFLAG(client);
    send(client_Fd, str.c_str(), str.length(), 0);
    return (false);
}


bool Server::Check_identify(std::string mode, std::string identify, int channel_pos){
    if (mode == "+o" || mode == "-o")
    {
        std::string op = identify;
        Add_Remove_OP(mode, op, channel_pos);
    }
    else if (mode == "+i" || mode == "-i")
    {
        std::string invisit = identify;
        Add_Remove_INVITE(this, mode, invisit, channel_pos);
    }
    else if (mode == "+k" || mode == "-k")
    {
        std::string pass = identify;
        Add_Remove_PASS(this, mode, pass, channel_pos);
    }
    else if (mode == "+t" || mode == "-t")
    {
        std::string topic = identify;
        Add_Remove_TOPIC(this, mode, topic, channel_pos);
    }
    else if (mode == "+l" || mode == "-l")
    {
        if (ft_isdigits(identify))
        {
            int user_limit = std::atoi(identify.c_str());
            Add_Remove_LIMIT(this, mode, user_limit, channel_pos);
        }
    }
    return (true);
}

void Server::Set_mode(std::string channel, std::string mode, std::string identify, int client_Fd)
{
    (void)identify;
    int channel_pos = 0;
    if (there_is_channel(channel, channel_pos, client_Fd)
        && Check_valid_Mode(channel, mode, client_Fd))
    {
        Check_identify(mode, identify, channel_pos);
    }
}

int  Server::Mode_func(std::string arg, int client_Fd)
{
    std::string channel;
    std::string mode;
    std::string identify;

    for (size_t i = 0; i < this->args.size(); i++){
        if (i == 1)
            channel = this->args[1];
        if (i == 2)
            mode = this->args[2];
        if (i == 3)
            identify = this->args[3];
    }
    if (mode.empty() || channel.empty())
    {
        std::string str = ERR_NEEDMOREPARAMS(arg);
        send(this->polls[client_Fd].fd, str.c_str(), str.length(), 0);
    }
    else
        Set_mode(channel, mode, identify, client_Fd);
    return (0);
}