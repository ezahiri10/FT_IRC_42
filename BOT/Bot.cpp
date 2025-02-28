/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezahiri <ezahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 13:33:21 by ezahiri           #+#    #+#             */
/*   Updated: 2025/02/28 16:32:48 by ezahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
// request form : PLAY <NICK>
// form of the message : <NICK> MOVE <position>
// form of the message : <NICK> FINISH

void Bot::parseRequest(std::string msg)
{
    std::cout << "msg " << msg << std::endl;
    // std::string tmp;
    // std::stringstream ss(msg);
    // ss >> tmp;
    // if (tmp == "PLAY")
    // {
    //     ss >> tmp;
    //     this->players.push_back(tmp);
    // }
    // else if (tmp == "MOVE")
    // {
    //     ss >> tmp;
    //     std::cout << tmp << std::endl;
    // }
    // else if (tmp == "FINISH")
    // {
    //     ss >> tmp;
    //     std::cout << tmp << std::endl;
    // }
}

void Bot::recvMesseages(bool welcom = false)
{
    char buffer[BUFFER_SIZE];

    int numChar = recv(this->botfd, buffer, sizeof(buffer), 0);
    if (numChar == -1)
        throw std::runtime_error ("recv failed");
    if (numChar == BUFFER_SIZE)
        numChar = 1023;
    buffer[numChar] = '\0';
    if (welcom)
        std::cout << "buffer :" <<  buffer << std::endl;
    else
        parseRequest(buffer);
}

void Bot::authentification()
{
    sockaddr_in add;

    add.sin_family = AF_INET;
    add.sin_port = htons(this->port);
    add.sin_addr.s_addr = INADDR_ANY;
    if (connect(this->botfd, (sockaddr *)&add, sizeof(add)) == -1)
    {
        perror("connect : ");
        throw std::runtime_error ("connect failed");
    }
    std::string msg = "PASS " + this->serverpass + "\r\n";
    if (send(this->botfd, msg.c_str(), msg.size(), 0) == -1)
        throw std::runtime_error ("send failed");
    msg = "NICK " + this->nick + "\r\n";
    if (send(this->botfd, msg.c_str(), msg.size(), 0) == -1)
        throw std::runtime_error ("send failed");
    msg = "USER U U U U \r\n";
    if (send(this->botfd, msg.c_str(), msg.size(), 0) == -1)
        throw std::runtime_error ("send failed");
    recvMesseages(true);
}

Bot::Bot(const std::string &port, const std::string &pass, const std::string &nick)
{
    if (port.find_first_not_of("0123456789") != std::string::npos)
        throw std::invalid_argument("invalid port");
    std::stringstream ss(port);
    ss >> this->port;
    if (this->port <= 0 || this->port > 65535)
        throw std::invalid_argument("invalid port");
    this->botfd = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(this->botfd, O_NONBLOCK);
    if (this->botfd == -1)
        throw std::runtime_error ("socket failed");
    this->serverpass = pass;
    this->nick = nick;
    authentification();
}


