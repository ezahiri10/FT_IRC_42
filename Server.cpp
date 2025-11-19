/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezahiri <ezahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 10:21:35 by ezahiri           #+#    #+#             */
/*   Updated: 2025/03/14 17:59:02 by ezahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Server.hpp"

bool Server::isstop = false;

void Server::ifFailed(const std::string &e)
{
    close (this->servfd);
    for (size_t i = 1; i < this->polls.size(); i++)
    {
        close (this->polls[i].fd);
    }
    throw std::runtime_error((e + ": " + strerror(errno)).c_str());
}

Server::Server(const std::string &port, const std::string &pass)
{
    if (pass.find_first_of(" \t") != std::string::npos || pass.empty())
        throw std::invalid_argument("invalid password");
    if (port.find_first_not_of("0123456789") != std::string::npos)
        throw std::invalid_argument("invalid port");
    std::stringstream ss(port);
    ss >> this->port;
    if (this->port <= 1023 || this->port > 65535)
        throw std::invalid_argument("invalid port");
    this->servfd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->servfd == -1)
        throw std::runtime_error ((std::string("socket failed: ") + strerror(errno)).c_str());
    this->serverpass = pass;
}

void Server::acceptConnection()
{
    if (isstop == true)
        return ;
    int clienfd = accept(this->servfd, NULL, NULL);
    if (clienfd < 0)
        throw std::runtime_error ((std::string("accept failed: ") + strerror(errno)).c_str());
    pollfd p;
    p.fd = clienfd;
    p.events= POLLIN;
    this->polls.push_back(p);
    Client newClient;
    this->clients.push_back(newClient);
    std::cout << "Client " << clienfd <<  " is connected" << std::endl;
}

void Server::removeUserFromChienl(const std::string &name)
{
    std::string msg;
    int pos;

    for (size_t i = 0; i < this->channels.size(); i++)
    {
        pos = Operators::getChannelClientPos(this->channels[i], name);
        if (pos != -1)
        {
            msg = RPL_PRIVMSG(name, this->channels[i].getChannelName(), "QUIT");
            responseFd(msg, this->channels[i].Channelclients[pos].getFd());
            this->channels[i].removeClient(pos); 
            if (this->channels[i].getClients().empty())
                this->channels.erase(this->channels.begin() + i);
        }
    }
}

void Server::recevMesseages(int i)
{
    char buffer[BUFFER_SIZE];

    if (isstop == true)
        return ;
    int numChar = recv(this->polls[i].fd, buffer, sizeof(buffer), 0);
    if (numChar <= 0)
    {
        std::cout << RED << "Client " << this->polls[i].fd << " is disconnected" << RESET << std::endl;
        if (this->clients[i - 1].getNickname() != "BOT")
        {
            this->messageToBot("QUIT", i);
        }
        removeUserFromChienl (this->clients[i - 1].getNickname());
        this->clients.erase(this->clients.begin() + i - 1);
        close(this->polls[i].fd);
        this->polls.erase(this->polls.begin() + i);
        return ;
    }
    if (numChar == 1024)
        numChar +=-1;
    buffer[numChar] = '\0';
    Parse(buffer, i);
}



void Server::handler(int sig)
{
    (void)sig;
    Server::isstop = true;
}

void Server::creatServer()
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
    while (!isstop)
    {
        int tocheck = poll (this->polls.data(), this->polls.size(), -1);
        if (tocheck < 0 && !isstop)
            throw std::runtime_error ("poll failed");
        if (this->polls[0].revents & POLLIN){
            acceptConnection();
        }
        for (size_t i = 1; i < this->polls.size(); i++)
        {
            if (this->polls[i].revents & POLLIN)
                recevMesseages(i);
        }
    }
}

std::vector<std::string> Server::splitByCRLF(const std::string& str) 
{
    std::vector<std::string> result;
    size_t start = 0, end;

    while ((end = str.find("\r\n", start)) != std::string::npos) {
        result.push_back(str.substr(start, end - start));
        start = end + 2;
    }
    if (start < str.size()) {
        result.push_back(str.substr(start));
    }
    return result;
}

void Server::changeNewLineToCRLF(std::string &msg)
{
    size_t pos = 0;
    while ((pos = msg.find("\n", pos)) != std::string::npos)
    {
        if (msg[pos - 1] != '\r')
        {
            msg.replace(pos, 1, "\r\n");
            pos += 2;
        }
        else
            pos++;
    }
}

void printNewlinCr(const std::string &msg)
{
    for (size_t i = 0; i < msg.size(); i++)
    {
        if (msg[i] == '\n')
            std::cout << "\\n";
        else if (msg[i] == '\r')
            std::cout << "\\r";
        else
            std::cout << msg[i];
    }
    std::cout << std::endl;
}

void Server::Parse(std::string msg, int clientId)
{
    if (msg.find ("\n") == std::string::npos)
    {
        this->clients[clientId - 1].buffer += msg;
        return ;
    }
    this->clients[clientId - 1].buffer += msg;
    changeNewLineToCRLF(this->clients[clientId - 1].buffer);
    if (this->clients[clientId - 1].buffer.substr(this->clients[clientId - 1].buffer.size() - 1) != "\n")
        return ;
    std::vector<std::string> tokns = splitByCRLF(this->clients[clientId - 1].buffer);
    for (size_t i = 0; i < tokns.size(); i++)
    {
        Authentication(tokns[i], clientId);
    }
    this->clients[clientId - 1].buffer.clear();
}

Server::~Server()
{
    for (size_t i = 0;i < this->polls.size(); i++)
    {
        close (this->polls[i].fd);
    }
}