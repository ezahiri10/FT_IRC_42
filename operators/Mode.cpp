/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fagr <ael-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 22:53:47 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/03/06 20:59:13 by ael-fagr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Operators.hpp"

bool ft_isdigits(Server &My_serv, Channel &channel, std::string identify, std::string client, int Client_id)
{
    for (size_t i = 0; i < identify.length(); i++)
    {
        if (!isdigit(identify[i]))
        {
            std::string str = ERR_INVALIDKEY(client, channel.getChannelName());
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

void Add_Mode(Channel &channel, std::string mode, std::string client_nick)
{
    std::vector<std::string> Modes = channel.getModes();
    if (std::find(Modes.begin(), Modes.end(), mode) != Modes.end())
        return ;
    else
    {
        std::string channel_name = channel.getChannelName();
        std::string server_name = ":IRCServer";
        std::string str = RPL_UMODEIS(server_name, channel_name, mode, client_nick);
        send_message(channel, str);
        channel.addMode(mode);
    }
}

void Remove_Mode(Channel &channel, std::string mode, std::string client_nick)
{
    std::vector<std::string> Modes = channel.getModes();

    std::vector<std::string>::iterator it = std::find(Modes.begin(), Modes.end(), mode);

    if (it != Modes.end())
        Modes.erase(it);
    std::string channel_name = channel.getChannelName();
    std::string server_name = ":IRCServer";
    std::string str = RPL_UMODEIS(server_name, channel_name, mode, client_nick);
    send_message(channel, str);
}

void Add_Remove_LIMIT(Channel &channel, std::string mode, int user_limit, std::string client_nick){
    if (mode == "+l")
    {
        channel.setChannelLimit(user_limit);
        Add_Mode(channel, mode, client_nick);
    }
    else
    {
        channel.setChannelLimit(10);
        Remove_Mode(channel, mode, client_nick);
    }
}

void Add_Remove_TOPIC(Channel &channel, std::string mode, std::string client_nick){
    if (mode == "+t")
    {
        channel.setIstopic(true);
        Add_Mode(channel, mode, client_nick);
    }
    else
    {
        channel.setIstopic(false);
        Remove_Mode(channel, mode, client_nick);
    }
}

void Add_Remove_INVITE(Channel &channel, std::string mode, std::string client_nick){
    if (mode == "+i")
    {
        channel.setInvited(true);
        Add_Mode(channel, mode, client_nick);
    }
    else
    {
        channel.setInvited(false);
        Remove_Mode(channel, mode, client_nick);
    }
}

void Operators::Add_Remove_PASS(Server &My_serv, Channel &channel, std::string mode, std::string pass, int Client_id){
    if (mode == "+k")
    {
        if (pass.empty())
        {
            std::string str = ERR_INVALIDMODEPARM(channel.getChannelName(), mode);
            send(My_serv.polls[Client_id].fd, str.c_str(), str.length(), 0);
        }
        else{
            channel.setPassword(pass);
            channel.setIsprivate(true);
            Add_Mode(channel, mode, Get_client_nick(My_serv, channel, Client_id));
        }
    }
    else
    {
        channel.setIsprivate(false);
        Remove_Mode(channel,  mode, Get_client_nick(My_serv, channel, Client_id));
    }
}

void Operators::Add_Remove_OP(Server &My_serv, Channel &channel, std::string mode, std::string op, int Client_id){
    if (mode == "+o")
    {
        if (there_is_user(My_serv, op, Client_id)
            && already_on_channel(My_serv, channel, op, Client_id, 0))
        {
            std::vector<std::string> ops = channel.getOperators();
            if (std::find(ops.begin(), ops.end(), op) == ops.end())
                channel.addOperator(op);
            Add_Mode(channel,  mode, Get_client_nick(My_serv, channel, Client_id));
        }
    } else
    {
        std::vector<std::string> ops = channel.getOperators();
        channel.getOperators().erase(std::remove(ops.begin(), ops.end(), op), ops.end());
        Remove_Mode(channel, mode, Get_client_nick(My_serv, channel, Client_id));
    }
}


bool Operators::Check_identify(Server &My_serv, Channel &channel, std::string mode, std::string identify, int Client_id){
    if (My_serv.channels.empty())
        return (false);

    if (mode == "+o" || mode == "-o")
    {
        if (identify.empty())
        {
            std::string str = ERR_INVALIDMODEPARM(channel.getChannelName(), mode);
            send(My_serv.polls[Client_id].fd, str.c_str(), str.length(), 0);
        }
        else{
            std::string op = identify;
            Add_Remove_OP(My_serv, channel, mode, op, Client_id);
        }
    }
    else if (mode == "+i" || mode == "-i")
    {
        Add_Remove_INVITE(channel, mode, Get_client_nick(My_serv, channel, Client_id));
    }
    else if (mode == "+k" || mode == "-k")
    {
        std::string pass = identify;
        Add_Remove_PASS(My_serv, channel, mode, pass, Client_id);
    }
    else if (mode == "+t" || mode == "-t")
    {
        Add_Remove_TOPIC(channel, mode, Get_client_nick(My_serv, channel, Client_id));
    }
    else if (mode == "+l" || mode == "-l")
    {
        if (identify.empty())
        {
            std::string str = ERR_INVALIDMODEPARM(channel.getChannelName(), mode);
            send(My_serv.polls[Client_id].fd, str.c_str(), str.length(), 0);
        }
        else if (ft_isdigits(My_serv, channel, identify, Get_client_nick(My_serv, channel, Client_id), Client_id))
        {
            int user_limit = std::atoi(identify.c_str());
            Add_Remove_LIMIT(channel, mode, user_limit, Get_client_nick(My_serv, channel, Client_id));
        }
    }
    return (true);
}

void Operators::Set_mode(Server &My_serv, Channel &channel, std::string mode, std::string identify, int Client_id)
{
    if (Check_valid_Mode(My_serv, channel.getChannelName(), mode, Client_id)
        && Check_Channel_Op(My_serv, channel, Get_client_nick(My_serv, channel, Client_id), Client_id))
    {
        Check_identify(My_serv, channel, mode, identify, Client_id);
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
        std::string mode = getMood(moodPart, i);
        std::string identify = getMood(identify_Part, i);
        if (!channel.empty() && mode.empty())
        {
            int channel_pos = 0;
            if (op.there_is_channel(My_serv, channel, channel_pos, Client_id))
            {
                std::string str = ERR_INVALIDMODEPARM(My_serv.channels[channel_pos].getChannelName(), mode);
                send(My_serv.polls[Client_id].fd, str.c_str(), str.length(), 0);
            }
        }
        else
        {
            int channel_pos = -1;
            if (op.there_is_channel(My_serv, channel, channel_pos, Client_id)){
                if (channel_pos == -1)
                    return (1);
                op.Set_mode(My_serv, My_serv.channels[channel_pos], mode, identify, Client_id);
            }
        }
        i++;
    }
    return (0);
}
