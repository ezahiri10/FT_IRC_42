/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fagr <ael-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 22:53:47 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/02/24 05:01:43 by ael-fagr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "operators.hpp"
/*The MODE command is used to set or remove options (or modes) from a given target.

User mode
If <target> is a nickname that does not exist on the network, the ERR_NOSUCHNICK (401) numeric is returned. 
If <target> is a different nick than the user who sent the command, the ERR_USERSDONTMATCH (502) numeric is returned.

If <modestring> is not given, the RPL_UMODEIS (221) numeric is sent back containing the current modes of the target user.

If <modestring> is given, the supplied modes will be applied, and a MODE message will be sent to the user containing the changed modes. 
If one or more modes sent are not implemented on the server,
 the server MUST apply the modes that are implemented, and then send the ERR_UMODEUNKNOWNFLAG (501) in reply along with the MODE message.

Channel mode
If <target> is a channel that does not exist on the network, the ERR_NOSUCHCHANNEL (403) numeric is returned.

If <modestring> is not given, the RPL_CHANNELMODEIS (324) numeric is returned. 
Servers MAY choose to hide sensitive information such as channel keys when sending the current modes.
Servers MAY also return the RPL_CREATIONTIME (329) numeric following RPL_CHANNELMODEIS.

If <modestring> is given, the user sending the command MUST have appropriate channel privileges on the target channel to change the modes given. 
If a user does not have appropriate privileges to change modes on the target channel, 
the server MUST NOT process the message, and ERR_CHANOPRIVSNEEDED (482) numeric is returned. 
If the user has permission to change modes on the target, the supplied modes will be applied based on the type of the mode (see below). 
For type A, B, and C modes, arguments will be sequentially obtained from <mode arguments>. 
If a type B or C mode does not have a parameter when being set, the server MUST ignore that mode. 
If a type A mode has been sent without an argument, the contents of the list MUST be sent to the user, 
unless it contains sensitive information the user is not allowed to access. When the server is done processing the modes, 
a MODE command is sent to all members of the channel containing the mode changes. 
Servers MAY choose to hide sensitive information when sending the mode changes.
*/

// int Check_Sign(std::string mode)
// {
//     if (mode[0] != '-' || mode[0] != '+')
//         return (false);
//     return (true);
// }

void Add_Remove_PASS(Server *My_serv, std::string mode, std::string pass, int index){//k
    if (mode == "+k")
        My_serv->channels[index].setPassword(pass);
    else
        My_serv->channels[index].setPassword("");
}

void Add_Remove_OP(Server *My_serv, std::string mode, std::string op, int index){//o
    if (mode == "+o") {
        if (there_is_user(My_serv, op, index))
        {
            std::vector<std::string> ops = My_serv->channels[index].getOperators();
            if (std::find(ops.begin(), ops.end(), op) == ops.end())
                ops.push_back(op);
        }
    } else {
        std::vector<std::string> ops = My_serv->channels[index].getOperators();
        ops.erase(std::remove(ops.begin(), ops.end(), op), ops.end());
    }
}

void Add_Remove_LIMIT(Server *My_serv, std::string mode, int user_limit, int index){//l
    if (mode == "+l")
        My_serv->channels[index].setLimit(user_limit);
    else
        My_serv->channels[index].setLimit(10);
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

bool Check_valid_Mode(Server *My_serv, std::string client, std::string mode, int client_Fd){
    const std::string modes[] = { 
        "+i", "+t", "+k", "+o", "+l", 
        "-i", "-t", "-k", "-o", "-l"
    };
    std::set<std::string> valid_modes(modes, modes + 10);

    if (valid_modes.find(mode) != valid_modes.end())
        return true;
    std::string str = ERR_UMODEUNKNOWNFLAG(client);
    send_message(My_serv, str, client_Fd);
    return (false);
}

bool Check_Client_Nick(Server *My_serv, std::string nick, int client_Fd)
{
    std::string Client_nick = Get_client_nick(My_serv, client_Fd);
    if (nick == Client_nick)
        return (true);
    std::string str = ERR_USERSDONTMATCH(nick);
    send_message(My_serv, str, client_Fd);
    return (false);
}

bool Check_identify(Server *My_serv, std::string mode, std::string identify, int index){
    if (mode == "+o" || mode == "-o")
    {
        std::string op = identify;
        Add_Remove_OP(My_serv, mode, op, index);
    }
    else if (mode == "+i" || mode == "-i")
    {
        std::string invisit = identify;
        Add_Remove_INVITE(My_serv, mode, invisit, index);
    }
    else if (mode == "+k" || mode == "-k")
    {
        std::string pass = identify;
        Add_Remove_PASS(My_serv, mode, pass, index);
    }
    else if (mode == "+t" || mode == "-t")
    {
        std::string topic = identify;
        Add_Remove_TOPIC(My_serv, mode, topic, index);
    }
    else if (mode == "+l" || mode == "-l")
    {
        if (ft_isdigits(identify))
        {
            int user_limit = std::atoi(identify.c_str());
            Add_Remove_LIMIT(My_serv, mode, user_limit, index);
        }
    }
    return (true);
}

void Set_mode(Server *My_serv, std::string channel, std::string mode, std::string identify, int client_Fd)
{
    (void)identify;
    int index = 0;
    if (there_is_Fd(My_serv, client_Fd)
        && there_is_channel(My_serv, channel, index, client_Fd)
        && Check_valid_Mode(My_serv, channel, mode, client_Fd))
    {
        Check_identify(My_serv, mode, identify, index);
    }
}

int  Mode_func(Server *My_serv, std::string arg, int client_Fd)
{
    Channel My_channel;
    Client My_client;
    std::string channel;
    std::string mode;
    std::string identify;

    My_channel.setName("#MY_CHANNEL");
    My_serv->channels.push_back(My_channel);

    My_client.setFd(client_Fd);
    My_client.setNickname("CHIDORI");
    My_serv->clients.push_back(My_client);

    My_channel.addClient(My_client);
    if (arg.empty())
        return (false);
    split_4_arguments(arg, channel, mode, identify);
    if (mode.empty() || channel.empty())
    {
        std::string str = ERR_NEEDMOREPARAMS(arg);
        send_message(My_serv, str, client_Fd);
    }
    else
        Set_mode(My_serv, channel, mode, identify, client_Fd);
    return (0);
}