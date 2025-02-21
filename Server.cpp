/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezahiri <ezahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 10:21:35 by ezahiri           #+#    #+#             */
/*   Updated: 2025/02/21 15:44:59 by ezahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::ifFailed(const std::string &e)
{
    close(this->servfd);
    throw std::runtime_error(e.c_str());
}

Server::Server(const std::string &port, const std::string &pass)
{
    if (pass.find_first_of(" \t") != std::string::npos || pass.empty())
        throw std::invalid_argument("invalid password");
    if (port.find_first_not_of("0123456789") != std::string::npos)
        throw std::invalid_argument("invalid port");
    std::stringstream ss(port);
    ss >> this->port;
    if (this->port <= 0 || this->port > 65535)
        throw std::invalid_argument("invalid port");
    this->servfd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->servfd == -1)
        throw std::runtime_error ("socket failed");
    this->serverpass = pass;
}

void Server::acceptConnection()
{

    int clienfd = accept(this->servfd, NULL, NULL);
    if (clienfd < 0)
        throw std::runtime_error ("accept failed");
    pollfd p;
    p.fd = clienfd;
    p.events= POLLIN;
    this->polls.push_back(p);
    std::cout << "Client " << clienfd <<  " is connected" << std::endl;
}

void Server::recevMesseages(int i)
{
    char s[BUFFER_SIZE];

    int numChar = recv(this->polls[i].fd, s, sizeof(s), 0);
    if (numChar < 0)
        throw std::runtime_error ("recv failed");
    if (numChar == 0)
    {
        std::cout << "Client " << this->polls[i].fd <<  " is disconnected" << std::endl;
        close(this->polls[i].fd);
        this->polls.erase(this->polls.begin() + i);
        return ;
    }
    s[numChar] = '\0';
    std::cout << "s :" << s << std::endl;
}

void Server::creatServer ()
{
    sockaddr_in add;
    pollfd p;

    add.sin_family = AF_INET;
    add.sin_port = htons(this->port);
    add.sin_addr.s_addr = INADDR_ANY;
    int en = 1;
    if(setsockopt(this->servfd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1)
        ifFailed ("setsockopt failed");
    if (bind (this->servfd, (sockaddr *)&add, sizeof(add)) == -1)
        ifFailed ("bind failed");
    if (listen(this->servfd, MAX_CLIENT) == -1)
        ifFailed ("listen failed");
    p.fd = this->servfd;
    p.events = POLLIN;
    this->polls.push_back(p);
    while (true)
    {
        int tocheck = poll (this->polls.data(), this->polls.size(), -1);
        if (tocheck < 0)
            throw std::runtime_error ("poll failed");
        if (this->polls[0].revents & POLLIN)
            acceptConnection();
        for (size_t i = 1; i < this->polls.size(); i++)
        {
            if (this->polls[i].revents & POLLIN)
                recevMesseages(i);
        }
    }
    
}
    