/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Operators.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fagr <ael-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 13:24:03 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/03/08 20:41:33 by ael-fagr         ###   ########.fr       */
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
        bool there_is_user(std::string client_nick, int Client_id);
        std::string Get_client_nick(Channel &channel, int clientId);
        void Desplay_topic(Channel  &channel, int Client_id);
        void Set_New_topic(Channel  &channel, std::string topic, int Client_id);
        bool Check_Channel_Op(Channel &channel, std::string client_nick, int clientId);
        void Add_Remove_OP(Channel &channel, std::string mode, std::string op, int Client_id);
        bool already_on_channel(Channel &channel, std::string client, int clientId, int check);
        bool Check_valid_Mode(std::string client, std::string mode, int Client_id);
        void Add_Remove_PASS(Channel &channel, std::string mode, std::string pass, int Client_id);
        bool Check_identify(Channel &channel, std::string mode, std::string identify, int Client_id);
        void Add_Remove_LIMIT(Channel &channel, std::string mode, std::string identify, int  Client_id);
        bool has_invited(Channel &channel, std::string client_nick);
    public:
        Operators();
        ~Operators();
        Server *getMyserv() const;
        void setMyserv(Server &server);
        static int Get_Channel_client_pos(Channel &channel, const std::string& nickname);
        static void send_message(Channel &channel, std::string str);
        static int Mode_func(Server &My_serv, std::string arg, int Client_id);
        static int Invite_func(Server &My_serv, std::string arg, int Client_id);
        static int Kick_func(Server &My_serv, std::string arg, int Client_id);
        static int Topic_func(Server &My_serv, std::string arg, int Client_id);
        void Add_topic(Channel  &channel, std::string topic, int Client_id);
        bool there_is_channel(std::string channel, int &channel_pos, int clientFD);
        int Invite_client(Channel &channel, std::string invit_client, int Client_id);
        int ADD_client(Channel &channel, std::string nick_name, std::string invit_client, int Client_id);
        void Set_mode(Channel &channel, std::string mode, std::string identify, int Client_id);
        bool Check_kick(Channel &channel, std::string client, std::string reasen, int Client_id);
};

#endif