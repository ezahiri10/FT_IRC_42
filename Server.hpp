/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakazdao <yakazdao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 06:46:07 by yakazdao          #+#    #+#             */
/*   Updated: 2025/03/02 15:29:15 by yakazdao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SERVER_HPP
#define SERVER_HPP


#define BUFFER_SIZE 1024

#include "Client.hpp"
#include "Channel.hpp"
#include "operators/Operators.hpp"
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
        void privMsg(std::string arg, int clientId);
        void MsgToChannel(std::string channelName, std::string msg, int clientId);
        void MsgToClient(std::string clientName, std::string msg, int clientId);
        std::vector<Client>::iterator getClientByName(std::string name);
        void response(const std::string &str, int clientId);

};


#endif