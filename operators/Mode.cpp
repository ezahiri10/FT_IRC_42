/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fagr <ael-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 22:53:47 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/02/28 10:02:55 by ael-fagr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Channel.hpp"
#include "../Server.hpp"
#include "../Replies.hpp"
#include <set>
#include <iostream>

void Server::Print_Channel_Modes(std::string channel, int channel_pos, int client_Fd)
{
    std::string msg = "+";
    std::vector<std::string>::iterator it;
    for (it = this->channels[channel_pos].getModes().begin(); it != this->channels[channel_pos].getModes().end(); it++){
        if ((*it) == "+t")
            msg += "t";
        else if ((*it) == "+k")
            msg += "k";
        else if ((*it) == "+o")
            msg += "o";
        else if ((*it) == "+l")
            msg += "l";
        else if ((*it) == "+i")
            msg += "i";
    }

    std::string hostname = "hostname";
    std::string str = RPL_UMODEIS(hostname, channel, msg, Get_client_nick(client_Fd, channel_pos));
    send(this->polls[client_Fd].fd, str.c_str(), str.length(), 0);

    // std::string str = ERR_UMODEUNKNOWNFLAG(client);
    // send(client_Fd, str.c_str(), str.length(), 0);
}

void Add_Mode(Server *My_serv, std::string mode, int channel_pos)
{
    std::vector<std::string> Modes = My_serv->channels[channel_pos].getModes();
    if (std::find(Modes.begin(), Modes.end(), mode) != Modes.end())
        return ;
    else
        My_serv->channels[channel_pos].addMode(mode);
}
void Remove_Mode(Server *My_serv, std::string mode, int channel_pos)
{
    std::vector<std::string> Modes = My_serv->channels[channel_pos].getModes();

    std::vector<std::string>::iterator it = std::find(Modes.begin(), Modes.end(), mode);

    if (it != Modes.end())
        Modes.erase(it);
}

void Add_Remove_PASS(Server *My_serv, std::string mode, std::string pass, int channel_pos){//k
    if (mode == "+k")
    {
        if (!pass.empty())
            My_serv->channels[channel_pos].setPassword(pass);
        My_serv->channels[channel_pos].setIsprivate(true);
        Add_Mode(My_serv, mode, channel_pos);
    }
    else
    {
        My_serv->channels[channel_pos].setIsprivate(false);
        Remove_Mode(My_serv, mode, channel_pos);
    }
}

void Server::Add_Remove_OP(std::string mode, std::string op, int channel_pos){//o
    if (mode == "+o") {
        if (there_is_user(op, channel_pos))
        {
            std::vector<std::string> ops = this->channels[channel_pos].getOperators();
            if (std::find(ops.begin(), ops.end(), op) == ops.end())
                ops.push_back(op);
            Add_Mode(this, mode, channel_pos);
        }
    } else {
        std::vector<std::string> ops = this->channels[channel_pos].getOperators();
        ops.erase(std::remove(ops.begin(), ops.end(), op), ops.end());
        Remove_Mode(this, mode, channel_pos);
    }
}

void Add_Remove_LIMIT(Server *My_serv, std::string mode, int user_limit, int channel_pos){//l
    if (mode == "+l")
    {
        My_serv->channels[channel_pos].setChannelLimit(user_limit);
        Add_Mode(My_serv, mode, channel_pos);
    }
    else
    {
        My_serv->channels[channel_pos].setChannelLimit(10);
        Remove_Mode(My_serv, mode, channel_pos);
    }
}

void Add_Remove_TOPIC(Server *My_serv, std::string mode, int channel_pos){//t
    if (mode == "+t")
    {
        My_serv->channels[channel_pos].setIstopic(true);
        Add_Mode(My_serv, mode, channel_pos);
    }
    else
    {
        My_serv->channels[channel_pos].setIstopic(false);
        Remove_Mode(My_serv, mode, channel_pos);
    }
}

void Add_Remove_INVITE(Server *My_serv, std::string mode, int channel_pos){//i
    if (mode == "+i")
    {
        My_serv->channels[channel_pos].setInvited(true);
        Add_Mode(My_serv, mode, channel_pos);
    }
    else
    {
        My_serv->channels[channel_pos].setInvited(false);
        Remove_Mode(My_serv, mode, channel_pos);
    }
}
bool ft_isdigits(std::string identify)
{
    for (size_t i = 0; i < identify.length(); i++)
    {
        if (!isdigit(identify[i]))
        {
            // ERR_INVALIDKEY (525)
            return (false);
        }
    }
    return (true);
}

bool Check_valid_Mode(Server *My_serv,std::string client, std::string mode, int client_Fd){

    const std::string modes[] = { 
        "+i", "+t", "+k", "+o", "+l", 
        "-i", "-t", "-k", "-o", "-l"
    };
    std::set<std::string> valid_modes(modes, modes + 10);

    if (valid_modes.find(mode) != valid_modes.end())
        return true;
    std::string str = ERR_UMODEUNKNOWNFLAG(client);
    send(My_serv->polls[client_Fd].fd, str.c_str(), str.length(), 0);
    return (false);
}


bool Server::Check_identify(std::string mode, std::string identify, int channel_pos, int FD){
    if (this->channels.empty())
        return (false);

    if (mode == "+o" || mode == "-o")
    {
        if (identify.empty())
        {
            std::string str = ERR_INVALIDMODEPARM(this->channels[channel_pos].getChannelName(), mode);
            send(this->polls[FD].fd, str.c_str(), str.length(), 0);
        }
        else{
            std::string op = identify;
            Add_Remove_OP(mode, op, channel_pos);
        }
    }
    else if (mode == "+i" || mode == "-i")//movet
    {
        Add_Remove_INVITE(this, mode, channel_pos);
    }
    else if (mode == "+k" || mode == "-k")//moved
    {
        std::string pass = identify;
        Add_Remove_PASS(this, mode, pass, channel_pos);
    }
    else if (mode == "+t" || mode == "-t")////movet
    {
        Add_Remove_TOPIC(this, mode, channel_pos);
    }
    else if (mode == "+l" || mode == "-l")//moved
    {
        if (ft_isdigits(identify))
        {
            int user_limit = std::atoi(identify.c_str());
            Add_Remove_LIMIT(this, mode, user_limit, channel_pos);
        }
    }
    return (true);
}

void Server::Set_mode(std::string channel, std::string mode, std::string identify, int FD)
{
    int channel_pos = 0;
    if (there_is_channel(channel, channel_pos, FD)
        && Check_valid_Mode(this, channel, mode, FD)
        && Check_Channel_Op(Get_client_nick(FD, channel_pos), channel, channel_pos, FD))
    {
        Check_identify(mode, identify, channel_pos, FD);
    }
}

std::string getMood(std::string pass, int i) {
    std::vector<std::string> moodList;
    std::stringstream ss(pass);
    std::string token;
    while (getline(ss, token, ',')){
        moodList.push_back(token);
    }
    if (i >= 0 && i < (int)moodList.size()) {
        return moodList[i];
    }
    return "";
}

int  Server::Mode_func(std::string arg, int FD)
{
    if (this->polls.empty())
        return (false);

    std::string namePart;
    std::string moodPart;
    std::string identify_Part;
    (void)arg;
    for (size_t i = 0; i < this->args.size(); i++){
        if (i == 1)
            namePart = this->args[1];
        if (i == 2)
            moodPart = this->args[2];
        if (i == 3)
            identify_Part = this->args[3];
    }
    std::stringstream ss(namePart);
    std::string channel;
    int i = 0;
    while (getline(ss, channel, ',')) {
        std::string mode = getMood(moodPart, i);
        std::string identify = getMood(identify_Part, i);
        if (!channel.empty() && mode.empty())
        {
            int channel_pos = 0;
            if (there_is_channel(channel, channel_pos, FD))
                Print_Channel_Modes(channel, channel_pos, FD);
        }
        else
            Set_mode(channel, mode, identify, FD);
        i++;
    }
    return (0);
}
