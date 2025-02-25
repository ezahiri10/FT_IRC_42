/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakazdao <yakazdao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 16:44:48 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/02/25 11:46:14 by yakazdao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "operators.hpp"

/*

The TOPIC command is used to change or view the topic of the given channel. If <topic> is not given, either RPL_TOPIC or RPL_NOTOPIC is returned specifying the current channel topic or lack of one. If <topic> is an empty string, the topic for the channel will be cleared.

If the client sending this command is not joined to the given channel, and tries to view its’ topic, the server MAY return the ERR_NOTONCHANNEL (442) numeric and have the command fail.

If RPL_TOPIC is returned to the client sending this command, RPL_TOPICWHOTIME SHOULD also be sent to that client.

If the protected topic mode is set on a channel, then clients MUST have appropriate channel permissions to modify the topic of that channel. If a client does not have appropriate channel permissions and tries to change the topic, the ERR_CHANOPRIVSNEEDED (482) numeric is returned and the command will fail.

If the topic of a channel is changed or cleared, every client in that channel (including the author of the topic change) will receive a TOPIC command with the new topic as argument (or an empty argument if the topic was cleared) alerting them to how the topic has changed. If the <topic> param is provided but the same as the previous topic (ie. it is unchanged), servers MAY notify the author and/or other users anyway.

Clients joining the channel in the future will receive a RPL_TOPIC numeric (or lack thereof) accordingly.
*/

//Test Write TOPIC command in irc server

void Set_New_topic(Server *My_serv, std::string channel, std::string topic, int index, int client_Fd)
{
    std::string str = RPL_TOPIC(My_serv->clients[client_Fd].getNickname(), channel, topic);
    send_message(My_serv, str, client_Fd);
    // RPL_TOPIC(My_serv->clients[client_Fd].getNickname(), channel, topic);
    My_serv->channels[index].setTopic(topic);
}
void Check_topic(Server *My_serv, std::string channel, std::string topic, int index, int client_Fd){
    if (topic.empty())
    {
        if (My_serv->channels[index].getTopic().empty()){
            std::string str = RPL_NOTOPIC(My_serv->clients[client_Fd].getNickname(), channel);
            send_message(My_serv, str, client_Fd);
        }
        else{
            std::cout << "HELLLLLLLLL " << My_serv->channels[index].getTopic() << std::endl;
            std::string str =  My_serv->clients[client_Fd].getNickname() + " " + channel + " " + My_serv->channels[index].getTopic() + '\n';
            send_message(My_serv, str, client_Fd);
        }
    }
    else {
        // RPL_TOPIC();
        std::cout << "Was Here \n";
        Set_New_topic(My_serv, channel, topic, index, client_Fd);
    }

}

void Add_topic(Server *My_serv, std::string channel, std::string topic, int client_Fd)
{
    int index = 0;
    if (there_is_Fd(My_serv, client_Fd)
        && there_is_channel(My_serv, channel, index, client_Fd))
    {
        Check_topic(My_serv, channel, topic, index, client_Fd);
    }
}

int Topic_func(Server *My_serv, std::string arg, int client_Fd)
{
    Channel My_channel;
    Client My_client;
    std::string channel;
    std::string topic;

    My_channel.setChannelName("#MY_CHANNEL");
    My_channel.setTopic("HELLLLLLDLDLDLDLDLLDDL");
    My_serv->channels.push_back(My_channel);

    My_client.setFd(client_Fd);
    My_client.setNickname("CHIDORI");
    My_serv->clients.push_back(My_client);

    My_channel.addClient(My_client);
    if (arg.empty())
        return (false);

    split_3_arguments(arg, channel, topic);
    if (channel.empty())
    {
        std::string str = ERR_NEEDMOREPARAMS(arg);
        send_message(My_serv, str, client_Fd);
    }
    else
        Add_topic(My_serv, channel, topic, client_Fd);
    return (true);
}