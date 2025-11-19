/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezahiri <ezahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 12:24:17 by ezahiri           #+#    #+#             */
/*   Updated: 2025/03/13 01:31:05 by ezahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
{
    this->Clientfd = -1;
    this->password = "";
    this->nickname = "";
    this->username = "";
    this->buffer = "";
    this->ip = "";
    this->has_pass = false;
    this->has_nick = false;
    this->has_user = false;
    this->clientExist = false;
}

int Client::getFd( void ) const 
{
    return (this->Clientfd);
}

const std::string & Client::getNickname( void ) const 
{
    return (this->nickname);
}

const std::string & Client::getUsername( void ) const 
{
    return (this->username);
}

const std::string & Client::getPassword( void ) const
{
    return (this->password);
}

const std::string & Client::getIp ( void ) const{
    return this->ip;
}

void Client::setIp (const std::string &ip){
    this->ip = ip;
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


