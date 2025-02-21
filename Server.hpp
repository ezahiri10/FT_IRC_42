/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezahiri <ezahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 09:03:15 by ezahiri           #+#    #+#             */
/*   Updated: 2025/02/21 14:16:42 by ezahiri          ###   ########.fr       */
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

        void acceptConnection ();
        void recevMesseages(int i);
    public :
        Server(const std::string &port, const std::string &pass);
        void creatServer();
};


#endif