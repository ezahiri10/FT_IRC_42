/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakazdao <yakazdao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 22:53:47 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/03/03 00:42:31 by yakazdao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Operators.hpp"

void Operators::Print_Channel_Modes(Server &My_serv, std::string channel, int channel_pos, int Client_id)
{
    (void)channel;
    std::string msg = "+";
    std::vector<std::string>::iterator it;
    std::vector<std::string> MODES = My_serv.channels[channel_pos].getModes();
    if (!MODES.empty())
    {
        for (it = MODES.begin(); it != MODES.end(); it++){
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
    }

    std::string str = RPL_UMODEIS(Get_client_nick(My_serv, Client_id, channel_pos), msg);
    send(My_serv.polls[Client_id].fd, str.c_str(), str.length(), 0);
}

void Add_Mode(Server &My_serv, std::string mode, int channel_pos)
{
    std::vector<std::string> Modes = My_serv.channels[channel_pos].getModes();
    if (std::find(Modes.begin(), Modes.end(), mode) != Modes.end())
        return ;
    else
        My_serv.channels[channel_pos].addMode(mode);
}
void Remove_Mode(Server &My_serv, std::string mode, int channel_pos)
{
    std::vector<std::string> Modes = My_serv.channels[channel_pos].getModes();

    std::vector<std::string>::iterator it = std::find(Modes.begin(), Modes.end(), mode);

    if (it != Modes.end())
        Modes.erase(it);
}

void Add_Remove_PASS(Server &My_serv, std::string mode, std::string pass, int channel_pos, int Client_id){//k
    if (mode == "+k")
    {
        if (pass.empty())
        {
            std::string str = ERR_INVALIDMODEPARM(My_serv.channels[channel_pos].getChannelName(), mode);
            send(My_serv.polls[Client_id].fd, str.c_str(), str.length(), 0);
        }
        else{
            My_serv.channels[channel_pos].setPassword(pass);
            My_serv.channels[channel_pos].setIsprivate(true);
            Add_Mode(My_serv, mode, channel_pos);
        }
    }
    else
    {
        My_serv.channels[channel_pos].setIsprivate(false);
        Remove_Mode(My_serv, mode, channel_pos);
    }
}

void Operators::Add_Remove_OP(Server &My_serv, std::string mode, std::string op, int channel_pos, int clientId){//o
    if (mode == "+o")
    {
        if (there_is_user(My_serv, op, channel_pos)
            && already_on_channel(My_serv, op, My_serv.channels[channel_pos].getChannelName(), clientId, channel_pos, 0))
        {
            std::vector<std::string> ops = My_serv.channels[channel_pos].getOperators();
            if (std::find(ops.begin(), ops.end(), op) == ops.end())
                My_serv.channels[channel_pos].addOperator(op);
            Add_Mode(My_serv, mode, channel_pos);
        }
    } else
    {
        std::vector<std::string> ops = My_serv.channels[channel_pos].getOperators();
        My_serv.channels[channel_pos].getOperators().erase(std::remove(ops.begin(), ops.end(), op), ops.end());
        Remove_Mode(My_serv, mode, channel_pos);
    }
}

void Add_Remove_LIMIT(Server &My_serv, std::string mode, int user_limit, int channel_pos){//l
    if (mode == "+l")
    {
        My_serv.channels[channel_pos].setChannelLimit(user_limit);
        Add_Mode(My_serv, mode, channel_pos);
    }
    else
    {
        My_serv.channels[channel_pos].setChannelLimit(10);
        Remove_Mode(My_serv, mode, channel_pos);
    }
}

void Add_Remove_TOPIC(Server &My_serv, std::string mode, int channel_pos){//t
    if (mode == "+t")
    {
        My_serv.channels[channel_pos].setIstopic(true);
        Add_Mode(My_serv, mode, channel_pos);
    }
    else
    {
        My_serv.channels[channel_pos].setIstopic(false);
        Remove_Mode(My_serv, mode, channel_pos);
    }
}

void Add_Remove_INVITE(Server &My_serv, std::string mode, int channel_pos){//i
    if (mode == "+i")
    {
        My_serv.channels[channel_pos].setInvited(true);
        Add_Mode(My_serv, mode, channel_pos);
    }
    else
    {
        My_serv.channels[channel_pos].setInvited(false);
        Remove_Mode(My_serv, mode, channel_pos);
    }
}
bool ft_isdigits(Server &My_serv, std::string identify, std::string client, std::string channel, int Client_id)
{
    for (size_t i = 0; i < identify.length(); i++)
    {
        if (!isdigit(identify[i]))
        {
            std::string str = ERR_INVALIDKEY(client, channel);
            send(My_serv.polls[Client_id].fd, str.c_str(), str.length(), 0);
            return (false);
        }
    }
    return (true);
}

bool Check_valid_Mode(Server &My_serv, std::string client, std::string mode, int Client_id){

    const std::string modes[] = { 
        "+i", "+t", "+k", "+o", "+l", 
        "-i", "-t", "-k", "-o", "-l"
    };
    std::set<std::string> valid_modes(modes, modes + 10);

    if (valid_modes.find(mode) != valid_modes.end())
        return true;
    std::string str = ERR_UMODEUNKNOWNFLAG(client);
    send(My_serv.polls[Client_id].fd, str.c_str(), str.length(), 0);
    return (false);
}


bool Operators::Check_identify(Server &My_serv, std::string mode, std::string identify, std::string channel_name, int channel_pos, int Client_id){
    if (My_serv.channels.empty())
        return (false);

    if (mode == "+o" || mode == "-o")
    {
        if (identify.empty())
        {
            std::string str = ERR_INVALIDMODEPARM(My_serv.channels[channel_pos].getChannelName(), mode);
            send(My_serv.polls[Client_id].fd, str.c_str(), str.length(), 0);
        }
        else{
            std::string op = identify;
            std::cout << "operator = " << op << std::endl;
            Add_Remove_OP(My_serv, mode, op, channel_pos, Client_id);
        }
    }
    else if (mode == "+i" || mode == "-i")//movet
    {
        Add_Remove_INVITE(My_serv, mode, channel_pos);
    }
    else if (mode == "+k" || mode == "-k")//moved
    {
        std::string pass = identify;
        Add_Remove_PASS(My_serv, mode, pass, channel_pos, Client_id);
    }
    else if (mode == "+t" || mode == "-t")////movet
    {
        Add_Remove_TOPIC(My_serv, mode, channel_pos);
    }
    else if (mode == "+l" || mode == "-l")//moved
    {
        if (identify.empty())
        {
            std::string str = ERR_INVALIDMODEPARM(My_serv.channels[channel_pos].getChannelName(), mode);
            send(My_serv.polls[Client_id].fd, str.c_str(), str.length(), 0);
        }
        else if (ft_isdigits(My_serv, identify, Get_client_nick(My_serv, Client_id, channel_pos), channel_name, Client_id))
        {
            int user_limit = std::atoi(identify.c_str());
            Add_Remove_LIMIT(My_serv, mode, user_limit, channel_pos);
        }
    }
    return (true);
}

void Operators::Set_mode(Server &My_serv, std::string channel, std::string mode, std::string identify, int Client_id)
{
    int channel_pos = -1;
    if (there_is_channel(My_serv, channel, channel_pos, Client_id)
        && Check_valid_Mode(My_serv, channel, mode, Client_id)
        && Check_Channel_Op(My_serv, Get_client_nick(My_serv, Client_id, channel_pos), channel, channel_pos, Client_id))
    {
        Check_identify(My_serv, mode, identify, channel, channel_pos, Client_id);
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

int  Operators::Mode_func(Server &My_serv, std::string arg, int Client_id)
{
    Operators op;
    std::string namePart;
    std::string moodPart;
    std::string identify_Part;
    (void)arg;
    for (size_t i = 0; i < My_serv.args.size(); i++){
        if (i == 1)
            namePart = My_serv.args[1];
        if (i == 2)
            moodPart = My_serv.args[2];
        if (i == 3)
            identify_Part = My_serv.args[3];
    }
    std::stringstream ss(namePart);
    std::string channel;
    int i = 0;
    while (getline(ss, channel, ',')) {
        if (My_serv.channels.empty())
        {
            std::string str = ERR_NOSUCHCHANNEL(channel);
            send(My_serv.polls[Client_id].fd, str.c_str(), str.length(), 0);
            continue ;
        }
        std::string mode = getMood(moodPart, i);
        std::string identify = getMood(identify_Part, i);
        if (!channel.empty() && mode.empty())
        {
            int channel_pos = 0;
            if (op.there_is_channel(My_serv, channel, channel_pos, Client_id))
                op.Print_Channel_Modes(My_serv, channel, channel_pos, Client_id);
        }
        else
            op.Set_mode(My_serv, channel, mode, identify, Client_id);
        i++;
    }
    return (0);
}
