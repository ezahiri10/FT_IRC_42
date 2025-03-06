/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakazdao <yakazdao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 06:46:07 by yakazdao          #+#    #+#             */
/*   Updated: 2025/03/06 14:12:43 by yakazdao         ###   ########.fr       */
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
        std::vector<std::string> splitByCRLF(const std::string& str);
        void Parse(std::string msg, int clientId);
        bool messageToBot(const std::string &msgpart, int clientId);


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
        void Authentication(const std::string &message, int clientId);
        bool checkNickAvailability(const std::string& nick);
        void pass(const std::string &arg, int clientId);
        void nick(const std::string &arg, int clientId);
        void user(const std::string &arg, int clientId);
        void getArgs(std::string message);
        std::vector<Client>::iterator getClient(int fd);
        void exec_cmds(const std::string &command, const std::string &arg, int i);
        void join(const std::string &arg, int clientId);
        bool checkChannelExist(const std::string &channelName);
        void createChannel(const std::string &Ch_name, int clientId);
        void addClientToChannel(const std::string &Ch_name, const std::string &Ch_pass, int clientId);
        std::vector<Channel>::iterator getChannelByName(const std::string &name);
        bool clientExistInChannel(const std::string &chName, int clientId);
        void privMsg(const std::string &arg, int clientId);
        void MsgToChannel(const std::string &channelName, const std::string &msg, int clientId);
        void MsgToClient(const std::string &clientName, const std::string &msg, int clientId);
        std::vector<Client>::iterator getClientByName(const std::string &name);
        void responseId(const std::string &str, int clientId);
        void responseFd(const std::string &str, int fd);
        std::string getAllUsers(const std::string &channel);
        void sendReponse(std::string reponse, int fdclient);
};


#endif