/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezahiri <ezahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 09:10:33 by ezahiri           #+#    #+#             */
/*   Updated: 2025/03/13 01:34:55 by ezahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <unistd.h>
#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <sys/types.h>
#include <poll.h>
#include <sstream>
#include <netinet/in.h>
#include <signal.h>
#include <fcntl.h>
#include "Replies.hpp"
#include <string.h>
#include <arpa/inet.h>

#define MAX_CLIENT 128

class Client
{
    private :
        int Clientfd;
        std::string nickname;
        std::string username;
        std::string password;
        std::string ip;
    public :
        bool has_pass;
        bool has_nick;
        bool has_user;
        bool clientExist;
        std::string buffer;
        int getFd( void ) const;
        Client();
        const std::string &getNickname( void ) const;
        const std::string &getUsername ( void ) const;
        const std::string &getPassword ( void ) const;
        const std::string &getIp ( void ) const;
        void setFd (int fd);
        void setNickname (const std::string &nickname);
        void setUsername (const std::string &username);
        void setPassword (const std::string &password);
        void setIp (const std::string &ip);
        bool Authontacated()const;
};
#endif