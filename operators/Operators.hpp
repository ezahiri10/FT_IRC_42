/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Operators.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fagr <ael-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 13:24:03 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/03/01 16:03:50 by ael-fagr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPERATORS_HPP
#define OPERATORS_HPP

#include "../Server.hpp"
#include "../Replies.hpp"
#include <iostream>
#include <iterator>
#include <vector>
#include <set>
class Server;
class Operators {
    private:
        void Desplay_topic(Server &My_serv, std::string channel, int channel_pos, int Client_id);
        void Set_New_topic(Server &My_serv, std::string channel, std::string topic, int channel_pos, int Client_id);
        void Add_Mode(Server &My_serv, std::string mode, int channel_pos);
        void send_message(Server &My_serv, std::string str, int channel_pos);
        void Remove_Mode(Server &My_serv, std::string mode, int channel_pos);
        void Add_Remove_PASS(Server &My_serv, std::string mode, std::string pass, int channel_pos);
        void Add_Remove_LIMIT(Server &My_serv, std::string mode, int user_limit, int channel_pos);
        int ADD_client(Server &My_serv, std::string nick_name, std::string invit_client, std::string channel, int channel_pos, int Client_id);
        void Add_Remove_TOPIC(Server &My_serv, std::string mode, int channel_pos);
        void Add_Remove_INVITE(Server &My_serv, std::string mode, int channel_pos);
        bool Check_valid_Mode(Server &My_serv, std::string client, std::string mode, int Client_id);
        bool there_is_Fd(Server &My_serv, int fd);
        bool ft_isdigits(Server &My_serv, std::string identify, std::string client, std::string channel, int Client_id);
        bool there_is_user(Server &My_serv, std::string client, int Client_id);
        std::string Get_client_nick(Server &My_serv, int Client_id, int channel_pos);
        void Add_Remove_OP(Server &My_serv, std::string mode, std::string op, int channel_pos);
        int Get_Channel_client_pos(Server &My_serv, const std::string& nickname, int channel_pos);
        bool Check_identify(Server &My_serv, std::string mode, std::string identify, std::string channel_name, int channel_pos, int Client_id);
        bool already_on_channel(Server &My_serv, std::string client, std::string channel, int Client_id, int channel_pos, int check);
        bool Check_Channel_Op(Server &My_serv, std::string client_nick, std::string channel, int channel_pos, int Client_id);
    public:
        Operators();
        ~Operators();
        static int Mode_func(Server &My_serv, std::string arg, int Client_id);
        static int Invite_func(Server &My_serv, std::string arg, int Client_id);
        static int Kick_func(Server &My_serv, std::string arg, int Client_id);
        static int Topic_func(Server &My_serv, std::string arg, int Client_id);
        void Add_topic(Server &My_serv, std::string channel, std::string topic, int Client_id);
        bool there_is_channel(Server &My_serv, std::string channel, int &channel_pos, int clientFD);;
        void Print_Channel_Modes(Server &My_serv, std::string channel, int channel_pos, int Client_id);;
        void Set_mode(Server &My_serv, std::string channel, std::string mode, std::string identify, int Client_id);;
        bool Check_kick(Server &My_serv, std::string channel, std::string client, std::string reasen, int Client_id);
        int Invite_client(Server &My_serv, std::string arg, std::string invit_client, std::string channel, int Client_id);
};

#endif