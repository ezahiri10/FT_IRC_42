/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Operators.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fagr <ael-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 13:24:03 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/03/06 19:40:15 by ael-fagr         ###   ########.fr       */
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
        bool there_is_user(Server &My_serv, std::string client, int Client_id);
        int Get_Channel_client_pos(Channel &channel, const std::string& nickname);
        std::string Get_client_nick(Server &My_serv, Channel &channel, int clientId);
        void Set_New_topic(Server &My_serv, Channel  &channel, std::string topic, int Client_id);
        bool Check_Channel_Op(Server &My_serv, Channel &channel, std::string client_nick, int clientId);
        void Add_Remove_OP(Server &My_serv, Channel &channel, std::string mode, std::string op, int Client_id);
        bool already_on_channel(Server &My_serv, Channel &channel, std::string client, int clientId, int check);
        void Add_Remove_PASS(Server &My_serv, Channel &channel, std::string mode, std::string pass, int Client_id);
        bool Check_identify(Server &My_serv, Channel &channel, std::string mode, std::string identify, int Client_id);
    public:
        Operators();
        ~Operators();
        static int Mode_func(Server &My_serv, std::string arg, int Client_id);
        static int Invite_func(Server &My_serv, std::string arg, int Client_id);
        static int Kick_func(Server &My_serv, std::string arg, int Client_id);
        static int Topic_func(Server &My_serv, std::string arg, int Client_id);
        void Add_topic(Server &My_serv, Channel  &channel, std::string topic, int Client_id);
        bool there_is_channel(Server &My_serv, std::string channel, int &channel_pos, int clientFD);
        int Invite_client(Server &My_serv, Channel &channel, std::string invit_client, int Client_id);
        void Set_mode(Server &My_serv, Channel &channel, std::string mode, std::string identify, int Client_id);
        bool Check_kick(Server &My_serv, Channel &channel, std::string client, std::string reasen, int Client_id);
};
void send_message(Channel &channel, std::string str);

#endif