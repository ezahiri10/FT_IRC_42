/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privMsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezahiri <ezahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 11:18:12 by yakazdao          #+#    #+#             */
/*   Updated: 2025/03/02 15:28:05 by ezahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"
#include "../Channel.hpp"

std::vector<Client>::iterator Server::getClientByName(std::string name){
    std::vector<Client>::iterator iter;
    for(iter = clients.begin(); iter != clients.end(); iter++){
        if (iter->getNickname() == name)
            return (iter);
    }
    return this->clients.end();
}

std::string getPartss(std::string str, char x){
    int pos = 0;
    std::string namesPart;
    std::string msgPart;
    pos = str.find(' ');
    if (x == 'N')
        return (str.substr(0,pos));
    return (str.substr(pos));
}

void Server::MsgToChannel(std::string channelName, std::string msg, int clientId){
    std::vector<Channel>::iterator iter;
    std::vector<Client>::iterator cIter;
    msg += "\n";
    if (!checkChannelExist(channelName)){
        std::string err = ERR_NOSUCHCHANNEL(channelName);
        send(this->polls[clientId].fd, err.c_str(), strlen(err.c_str()), 0);
        std::cout << ERR_NOSUCHCHANNEL(channelName);return;
    }
    if(!checkIsClientExistInChannel(channelName, clientId)){
        std::string err = ERR_NOTONCHANNEL(clients[clientId].getNickname(), channelName);
        send(this->polls[clientId].fd, err.c_str(), strlen(err.c_str()), 0);return;
    }
    iter = getChannelByName(channelName);
    for(cIter = iter->Channelclients.begin(); cIter != iter->Channelclients.end(); cIter++){
        int id = cIter->getFd();
        send(this->polls[id].fd, msg.c_str(), strlen(msg.c_str()), 0);
    }
}

// l = "PRIVMSG LAME :Hello! Want to play Tic-Tac-Toe?\r\n";
void Server::MsgToClient(std::string clientName, std::string msg, int clientId)
{
    std::vector<Client>::iterator iter;
    if (!checkNickAvailability(clientName)){
        std::string err = ERR_NOSUCHNICK(clientName);
        send(this->polls[clientId].fd, err.c_str(), strlen(err.c_str()), 0);
        std::cout << ERR_NOSUCHNICK(clientName);return;
    }
    if (msg.substr(msg.size() - 2) != "\r\n")
        msg += "\r\n";
    size_t fin = msg.find(" ");
    if (fin != std::string::npos)
        msg = msg.substr(fin + 1);
    std::string forsend = "PRIVMSG " + clientName + " :" + msg;
    for (size_t i = 0; i < forsend.size(); i++)
    {
        if (forsend[i] == '\r')
            std::cout << "\\r";
        else if (forsend[i] == '\n')
            std::cout << "\\n"; 
        else  
            std::cout << forsend[i];
    }
    std::cout << std::endl;
    iter = getClientByName(clientName); //! iter return end
    if (iter != this->clients.end())
        std::cout << "ClientName: " << clientName << std::endl;
    send(iter->getFd(), forsend.c_str(), strlen(forsend.c_str()), 0); //! heap-buffer-overflow  in int id = iter->getFd(); --> send
}
 
bool Server::messageToBot(const std::string &msgpart, int clientId) //! add functiom to check if the message is for the bot
{
    std::vector<Client>::iterator iter;
    std::string nick;

    iter = getClientByName("BOT");
    if (msgpart == " GAME" && iter != this->clients.end())
    {
        nick =  "GAME " + this->clients[clientId - 1].getNickname();
        if (send (iter->getFd(), nick.c_str(), nick.size(), 0) < 0)
                throw std::runtime_error("send field");
        return (true);
    }
    else if (strncmp(msgpart.c_str() + 1, "MOVE", 4) == 0 && iter != this->clients.end())
    {
        nick = "MOVE "  +  this->clients[clientId - 1].getNickname() + " " + msgpart.substr(5);
        if (send (iter->getFd(), nick.c_str(), nick.size(), 0) < 0)
                throw std::runtime_error("send field");
        return (true);
    }
    else if (msgpart == " QUIT" && iter != this->clients.end())
    {
        nick = "QUIT " + this->clients[clientId - 1].getNickname();
        if (send (iter->getFd(), nick.c_str(), nick.size(), 0) < 0)
                throw std::runtime_error("send field");
        return (true);
    }
    return (false);
}

void Server::privMsg(std::string arg, int clientId)
{
    if (this->args.size() < 3){
        std::string err = ERR_NEEDMOREPARAMS(arg);
        send(this->polls[clientId].fd, err.c_str(), strlen(err.c_str()), 0);return;
    }
    std::string namesPart = getPartss(arg, 'N');
    std::string msgPart = getPartss(arg, 'X');
    if (namesPart == "BOT" && messageToBot(msgPart, clientId))
        return ;
    std::stringstream ss(namesPart);
    std::string buffer;
    while(getline(ss, buffer, ',')){
        if (buffer[0] == '#')
            MsgToChannel(buffer, msgPart, clientId);
        else
            MsgToClient(buffer, msgPart, clientId);
    }
}