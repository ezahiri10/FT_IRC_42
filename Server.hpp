/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fagr <ael-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 09:03:15 by ezahiri           #+#    #+#             */
/*   Updated: 2025/02/24 00:22:35 by ael-fagr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#define BUFFER_SIZE 1024

#include "Client.hpp"
#include "Channel.hpp"
// #include "Replies.hpp"

class Server
{
    private :
        int servfd;
        int port;
        std::string serverpass;

        void acceptConnection ();
        void recevMesseages(Server *serv, int i);
    public :
        std::vector<pollfd> polls;
        std::vector<Client> clients;
        std::vector<Channel> channels;
        Server(const std::string &port, const std::string &pass);
        void creatServer(Server *serv);
};

int Kick_func(Server *My_serv, std::string arg, int client_Fd);
int Topic_func(Server *My_serv, std::string arg, int client_Fd);
int Invite_func(Server *My_serv, std::string arg, int client_Fd);
int Mode_func(Server *My_serv, std::string arg, int client_Fd);


#endif