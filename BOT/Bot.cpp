/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezahiri <ezahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 13:33:21 by ezahiri           #+#    #+#             */
/*   Updated: 2025/03/01 22:26:37 by ezahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

bool Bot::isRunning = true;

int Bot::getPlayerByName(const std::string &name)
{
    for (size_t i = 0; i < this->players.size(); i++)
    {
        if (this->players[i].getNickname() == name)
            return (i);
    }
    return (-1);
}

void Bot::parseRequest(std::string msg)
{
    int player;
    std::string tmp;
    std::stringstream ss(msg);

    ss >> tmp;
    if (tmp == "GAME")
    {
        ss >> tmp;
        int player = getPlayerByName(tmp);
        if (player > 0)
            return ;
        Player p(tmp);
        this->players.push_back(p);
        Player::sendRequest("PRIVMSG " + tmp + " for palaying with me\nsend : MOVE <position> to play", this->botfd);
        Player::sendRequest("PRIVMSG " + tmp + " Select number 1 ~ 9 :\n\n\n\n", this->botfd);
        Player::sendRequest(("PRIVMSG " + p.getNickname() + " "+ p.getBoard()).c_str(), botfd);
    }
    else if (tmp == "MOVE")
    {
        ss >> tmp;
        player = getPlayerByName(tmp);
        if (player == -1)
            return ;
        ss >> tmp;
        if (this->players[player].ticTacToe(tmp, this->botfd) == true)
            this->players.erase(this->players.begin() + player);
    }
    else if (tmp == "QUIT")
    {
        ss >> tmp;
        player = getPlayerByName(tmp);
        if (player == -1)
            return ;
        this->players.erase(this->players.begin() + player);
        Player::sendRequest("PRIVMSG " + tmp + " has left the game", this->botfd);
    }
}

void Bot::recvMesseages(bool welcom )
{
    char buffer[BUFFER_SIZE];

    int numChar = recv(this->botfd, buffer, sizeof(buffer), 0);
    if (numChar == -1)
        throw std::runtime_error ("recv failed");
    if (numChar == 0)
        throw std::runtime_error ("connection closed");
    if (numChar == BUFFER_SIZE)
        numChar = 1023;
    buffer[numChar] = '\0';
    if (welcom)
        std::cout <<  buffer << std::endl;
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
        throw std::runtime_error ("connect failed");
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

void Bot::handler(int signum)
{
    (void)signum;
    std::cout << "Exiting..." << std::endl;
    exit(0);
}

Bot::~Bot()
{
    close(this->botfd);
}
