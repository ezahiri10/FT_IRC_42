/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezahiri <ezahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 09:10:33 by ezahiri           #+#    #+#             */
/*   Updated: 2025/02/21 16:32:06 by ezahiri          ###   ########.fr       */
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
 
#define MAX_CLIENT 128

class Client
{
    private :
        int Clientfd;
        std::string nickname;
        std::string username;
        std::string password;
        bool arr[3];
    public :
        Client();
        int getFd( void ) const;
        std::string getNickname( void ) const;
        std::string getUsername ( void ) const;
        std::string getPassword ( void ) const;
        void setFd (int fd);
        void setNickname (const std::string &nickname);
        void setUsername (const std::string &username);
        void setPassword (const std::string &password);
};

#endif