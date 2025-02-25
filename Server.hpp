/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fagr <ael-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 06:46:07 by yakazdao          #+#    #+#             */
/*   Updated: 2025/02/25 19:35:23 by ael-fagr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SERVER_HPP
#define SERVER_HPP


#define BUFFER_SIZE 1024

#include "Client.hpp"
#include "Channel.hpp"

class Server
{
    private :
        int servfd;
        int port;
        std::string serverpass;
        void acceptConnection ();
        void recevMesseages(int i);
        void ifFailed(const std::string &e);
    public :
        Server();
        std::vector<pollfd> polls;
        std::vector<Client> clients;
        std::vector<Channel> channels;
        static bool isstop;
        std::vector<std::string>args;
        static void handler(int sig);
        Server(const std::string &port, const std::string &pass);
        void creatServer();
        ~Server ();
        void Authentication(std::string message, int clientId);
        bool checkNickAvailability(const std::string& nick);
        void pass(std::string arg, int clientId);
        void nick(std::string arg, int clientId);
        void user(std::string arg, int clientId);
        void getArgs(std::string message);
        std::vector<Client>::iterator getClient(int fd);
        void exec_cmds(std::string command, std::string arg, int i);
        void join(std::string arg, int clientId);
        bool checkChannelExist(std::string channelName);
        void createChannel(std::string Ch_name, std::string Ch_pass, int clientId);
        void addClientToChannel(std::string Ch_name, std::string Ch_pass, int clientId);
        std::vector<Channel>::iterator getChannelByName(std::string name);

        bool there_is_Fd(int fd);
        std::string Get_client_nick(int client_Fd);
        int Mode_func(std::string arg, int client_Fd);
        int Kick_func(std::string arg, int client_Fd);
        int Topic_func(std::string arg, int client_Fd);
        int Invite_func(std::string arg, int client_Fd);
        bool there_is_user(std::string client, int client_Fd);
        int Get_client_pos(const std::string& nickname, int index);
        void Add_Remove_OP(std::string mode, std::string op, int index);
        bool there_is_channel(std::string channel, int &index, int clientFD);
        void Add_topic(std::string channel, std::string topic, int client_Fd);
        bool Check_identify(std::string mode, std::string identify, int index);
        void Set_mode(std::string channel, std::string mode, std::string identify, int client_Fd);
        bool Check_kick(std::string channel, std::string client, std::string reasen, int client_Fd);
        int Invite_client(std::string arg, std::string invit_client, std::string channel, int client_Fd);
        bool already_on_channel(std::string client, std::string channel, int client_Fd, int index, int check);

};


#endif