/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privMsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezahiri <ezahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 11:18:12 by yakazdao          #+#    #+#             */
/*   Updated: 2025/03/05 02:34:21 by ezahiri          ###   ########.fr       */
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
    return clients.end();
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
    cIter = getClient(this->polls[clientId].fd);
    if (!checkChannelExist(channelName)){
        std::string err = ERR_NOSUCHCHANNEL(channelName);
        send(this->polls[clientId].fd, err.c_str(), strlen(err.c_str()), 0);
        std::cout << ERR_NOSUCHCHANNEL(channelName);return;
    }
    if(!checkIsClientExistInChannel(channelName, clientId)){
        std::string err = ERR_NOTONCHANNEL(cIter->getNickname(), channelName);
        send(cIter->getFd(), err.c_str(), strlen(err.c_str()), 0);return;
    }
    iter = getChannelByName(channelName);
    for(cIter = iter->Channelclients.begin(); cIter != iter->Channelclients.end(); cIter++){
        int id = cIter->getFd();
        std::string str = RPL_PRIVMSG(this->clients[clientId - 1].getNickname(), cIter->getNickname(), msg);
        send(id, str.c_str(), strlen(str.c_str()), 0);
    }
}

void Server::MsgToClient(std::string clientName, std::string msg, int clientId){
    std::vector<Client>::iterator iter;
    if (!checkNickAvailability(clientName)){
        std::string err = ERR_NOSUCHNICK(clientName);
        send(this->polls[clientId].fd, err.c_str(), strlen(err.c_str()), 0);
        std::cout << ERR_NOSUCHNICK(clientName);return;
    }

    iter = getClientByName(clientName);
    int id = iter->getFd();
    std::string str = RPL_PRIVMSG(this->clients[clientId - 1].getNickname(), iter->getNickname(), msg);
    send(id, str.c_str(), strlen(str.c_str()), 0);
}

std::string trim(const std::string & source) {
    std::string s(source);
    s.erase(0,s.find_first_not_of(" \n\r\t"));
    s.erase(s.find_last_not_of(" \n\r\t")+1);
    return s;
}

bool Server::messageToBot(const std::string &msgpart, int clientId)
{
    std::vector<Client>::iterator iter;
    std::string nick;

    iter = getClientByName("BOT");
    if (msgpart == "GAME" && iter != this->clients.end())
    {
        nick =  "GAME " + this->clients[clientId - 1].getNickname();
        if (send (iter->getFd(), nick.c_str(), nick.size(), 0) < 0)
                throw std::runtime_error("send field");
        return (true);
    }
    else if (strncmp(msgpart.c_str(), "MOVE", 4) == 0 && iter != this->clients.end())
    {
        if (msgpart.at(4) != ' ')
            return (false);
        nick = "MOVE "  +  this->clients[clientId - 1].getNickname() + " " + msgpart.substr(4);
        if (send (iter->getFd(), nick.c_str(), nick.size(), 0) < 0)
                throw std::runtime_error("send field");
        return (true);
    }
    else if (msgpart == "QUIT" && iter != this->clients.end())
    {
        nick = "QUIT " + this->clients[clientId - 1].getNickname();
        if (send (iter->getFd(), nick.c_str(), nick.size(), 0) < 0)
                throw std::runtime_error("send field");
        return (true);
    }
    return (false);
}

void Server::privMsg(std::string arg, int clientId){
    if (this->args.size() < 3){
        std::string err = ERR_NEEDMOREPARAMS(arg);
        send(this->polls[clientId].fd, err.c_str(), strlen(err.c_str()), 0);return;
    }
    std::string namesPart = getPartss(arg, 'N');
    std::string msgPart = getPartss(arg, 'X');
    msgPart = trim(msgPart);
    if (msgPart[0] != ':')
        msgPart = this->args[2];
    else
        msgPart.erase(0,msgPart.find_first_not_of(":"));
    if (namesPart == "BOT" && messageToBot(msgPart, clientId))
        return ;
    std::cout << "msgPart :" << msgPart << std::endl;
    std::stringstream ss(namesPart);
    std::string name;
    while(getline(ss, name, ',')){
        // if (name[0] == '#')
        //     MsgToChannel(name, msgPart, clientId);
        // else
            MsgToClient(name, msgPart, clientId);
    }
}