/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakazdao <yakazdao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 06:46:07 by yakazdao          #+#    #+#             */
/*   Updated: 2025/02/26 10:27:33 by yakazdao         ###   ########.fr       */
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
        bool checkIsClientExistInChannel(std::string chName, int clientId);
};
int Kick_func(Server *My_serv, std::string arg, int client_Fd);
int Topic_func(Server *My_serv, std::string arg, int client_Fd);
int Invite_func(Server *My_serv, std::string arg, int client_Fd);
int Mode_func(Server *My_serv, std::string arg, int client_Fd);


#endif