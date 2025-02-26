/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakazdao <yakazdao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 12:24:17 by ezahiri           #+#    #+#             */
/*   Updated: 2025/02/26 09:57:30 by yakazdao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
{
    this->Clientfd = -1;
    this->password = "";
    this->nickname = "";
    this->username = "";
    this->has_pass = false;
    this->has_nick = false;
    this->has_user = false;
    this->clientExist = false;
    this->isOperator = false;
}

int Client::getFd( void ) const 
{
    return (this->Clientfd);
}

std::string Client::getNickname( void ) const 
{
    return (this->nickname);
}

std::string Client::getUsername( void ) const 
{
    return (this->username);
}

std::string Client::getPassword( void ) const
{
    return (this->password);
}

void Client::setFd (int fd)
{
    this->Clientfd = fd;
}

void Client::setNickname (const std::string &nickname)
{
    this->nickname = nickname;
}

void Client::setUsername (const std::string &username)
{
    this->username = username;
}

void Client::setPassword (const std::string &password)
{
    this->password = password;
}

bool Client::Authontacated() const
{
    return (this->has_pass && this->has_nick && this->has_user);
}


