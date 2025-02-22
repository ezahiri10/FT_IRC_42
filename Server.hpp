/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakazdao <yakazdao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 09:03:15 by ezahiri           #+#    #+#             */
/*   Updated: 2025/02/22 18:52:14 by yakazdao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP


#define BUFFER_SIZE 1024

#include "Client.hpp"

class Server
{
    private :
        int servfd;
        int port;
        std::string serverpass;
        std::vector<pollfd> polls;
        std::vector<Client> clients;
        Client newClient;
        void acceptConnection ();
        void recevMesseages(int i);
        void ifFailed(const std::string &e);
    public :
        Server(){}
        static bool isstop;
        static void handler(int sig);
        Server(const std::string &port, const std::string &pass);
        void creatServer();
        ~Server ();
        void Authentication(std::string message, int clientId);
        bool checkNickAvailability(const std::string& nick);
};


#endif