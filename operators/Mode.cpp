/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fagr <ael-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 22:53:47 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/02/25 19:37:40 by ael-fagr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Channel.hpp"
#include "../Server.hpp"
#include "../Replies.hpp"
#include <set>
#include <iostream>

void Add_Remove_PASS(Server *My_serv, std::string mode, std::string pass, int index){//k
    if (mode == "+k")
        My_serv->channels[index].setPassword(pass);
    else
        My_serv->channels[index].setPassword("");
}

void Server::Add_Remove_OP(std::string mode, std::string op, int index){//o
    if (mode == "+o") {
        if (there_is_user(op, index))
        {
            std::vector<std::string> ops = this->channels[index].getOperators();
            if (std::find(ops.begin(), ops.end(), op) == ops.end())
                ops.push_back(op);
        }
    } else {
        std::vector<std::string> ops = this->channels[index].getOperators();
        ops.erase(std::remove(ops.begin(), ops.end(), op), ops.end());
    }
}

void Add_Remove_LIMIT(Server *My_serv, std::string mode, int user_limit, int index){//l
    if (mode == "+l")
        My_serv->channels[index].setChannelLimit(user_limit);
    else
        My_serv->channels[index].setChannelLimit(10);
}

void Add_Remove_TOPIC(Server *My_serv, std::string mode, std::string topic, int index){//t
    if (mode == "+t")
        My_serv->channels[index].setTopic(topic);
    else
        My_serv->channels[index].setTopic("");
}

void Add_Remove_INVITE(Server *My_serv, std::string mode, std::string invite, int index){//i
    (void)invite;
    if (mode == "+i")
        My_serv->channels[index].setInvited(true);
    else
        My_serv->channels[index].setInvited(false);
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


bool Server::Check_identify(std::string mode, std::string identify, int index){
    if (mode == "+o" || mode == "-o")
    {
        std::string op = identify;
        Add_Remove_OP(mode, op, index);
    }
    else if (mode == "+i" || mode == "-i")
    {
        std::string invisit = identify;
        Add_Remove_INVITE(this, mode, invisit, index);
    }
    else if (mode == "+k" || mode == "-k")
    {
        std::string pass = identify;
        Add_Remove_PASS(this, mode, pass, index);
    }
    else if (mode == "+t" || mode == "-t")
    {
        std::string topic = identify;
        Add_Remove_TOPIC(this, mode, topic, index);
    }
    else if (mode == "+l" || mode == "-l")
    {
        if (ft_isdigits(identify))
        {
            int user_limit = std::atoi(identify.c_str());
            Add_Remove_LIMIT(this, mode, user_limit, index);
        }
    }
    return (true);
}

void Server::Set_mode(std::string channel, std::string mode, std::string identify, int client_Fd)
{
    (void)identify;
    int index = 0;
    if (there_is_Fd(client_Fd)
        && there_is_channel(channel, index, client_Fd)
        && Check_valid_Mode(channel, mode, client_Fd))
    {
        Check_identify(mode, identify, index);
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
        send(client_Fd, str.c_str(), str.length(), 0);
    }
    else
        Set_mode(channel, mode, identify, client_Fd);
    return (0);
}