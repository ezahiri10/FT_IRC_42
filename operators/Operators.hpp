/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Operators.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fagr <ael-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 13:24:03 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/03/08 22:44:06 by ael-fagr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPERATORS_HPP
#define OPERATORS_HPP

#include "../Server.hpp"
#include "../Replies.hpp"
#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <set>
class Server;

class Operators 
{
    private:
        Server *Myserv;
        bool ThereIsUser(std::string client_nick, int Client_id);
        std::string GetClientNick(Channel &channel, int clientId);
        void DesplayTopic(Channel  &channel, int Client_id);
        void SetNewTopic(Channel  &channel, std::string topic, int Client_id);
        bool CheckChannelOp(Channel &channel, std::string client_nick, int clientId);
        void AddRemoveOP(Channel &channel, std::string mode, std::string op, int Client_id);
        bool AlreadyOnChannel(Channel &channel, std::string client, int clientId, int check);
        bool CheckValidMode(std::string client, std::string mode, int Client_id);
        void AddRemovePass(Channel &channel, std::string mode, std::string pass, int Client_id);
        bool CheckIdentify(Channel &channel, std::string mode, std::string identify, int Client_id);
        void AddRemoveLimit(Channel &channel, std::string mode, std::string identify, int  Client_id);
        int GetClientFd(std::string nick_name);
    public:
        Operators();
        ~Operators();
        Server *getMyserv() const;
        static bool HasInvited(Channel &channel, std::string client_nick);
        void setMyserv(Server &server);
        static int GetChannelClientPos(Channel &channel, const std::string& nickname);
        static void SendMessage(Channel &channel, std::string str);
        static int ModeFunc(Server &My_serv, std::string arg, int Client_id);
        static int InviteFunc(Server &My_serv, std::string arg, int Client_id);
        static int KickFunc(Server &My_serv, std::string arg, int Client_id);
        static int TopicFunc(Server &My_serv, std::string arg, int Client_id);
        void AddTopic(Channel  &channel, std::string topic, int Client_id);
        bool ThereIsChannel(std::string channel, int &channel_pos, int clientFD);
        int InviteClient(Channel &channel, std::string invit_client, int Client_id);
        int AddClient(Channel &channel, std::string nick_name, std::string invit_client);
        void SetMode(Channel &channel, std::string mode, std::string identify, int Client_id);
        bool Check_kick(Channel &channel, std::string client, std::string reasen, int Client_id);
};

#endif