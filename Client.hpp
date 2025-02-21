/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezahiri <ezahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 09:10:33 by ezahiri           #+#    #+#             */
/*   Updated: 2025/02/21 14:42:29 by ezahiri          ###   ########.fr       */
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
        int getFd( void );
        std::string getNickname( void );
        std::string getUsername ( void );
        std::string getPassword ( void );
        void setFd (int fd);
        void setNickname (const std::string &nickname);
        void setUsername (const std::string &username);
        void setPassword (const std::string &password);
        // ~Client();
};

#endif