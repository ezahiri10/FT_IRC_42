/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privMsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakazdao <yakazdao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 03:16:23 by ezahiri           #+#    #+#             */
/*   Updated: 2025/03/06 14:10:45 by yakazdao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"
#include "../Channel.hpp"

std::string getPartss(std::string str, char x){
    int pos = 0;
    std::string namesPart;
    std::string msgPart;
    pos = str.find(' ');
    if (x == 'N')
        return (str.substr(0,pos));
    return (str.substr(pos));
}

void Server::MsgToChannel(const std::string &channelName, const std::string &msg, int clientId){
    std::vector<Channel>::iterator iter;
    std::vector<Client>::iterator cIter;
    cIter = getClient(this->polls[clientId].fd);
    if (!checkChannelExist(channelName)){
        responseFd(ERR_NOSUCHCHANNEL(channelName), this->polls[clientId].fd);
        std::cout << ERR_NOSUCHCHANNEL(channelName);return;
    }
    if(!clientExistInChannel(channelName, clientId)){
        responseFd(ERR_NOTONCHANNEL(cIter->getNickname(), channelName), cIter->getFd());
        return;
    }
    iter = getChannelByName(channelName);
    for(cIter = iter->Channelclients.begin(); cIter != iter->Channelclients.end(); cIter++){
        int fd = cIter->getFd();
        responseFd(RPL_PRIVMSG(this->clients[clientId - 1].getNickname(), cIter->getNickname(), msg), fd);
    }
}

void Server::MsgToClient(const std::string &clientName, const std::string &msg, int clientId){
    std::vector<Client>::iterator iter;
    if (!checkNickAvailability(clientName)){
        responseFd(ERR_NOSUCHNICK(clientName), this->polls[clientId].fd);
        std::cout << ERR_NOSUCHNICK(clientName);return;
    }

    iter = getClientByName(clientName);
    int fd = iter->getFd();
    responseFd(RPL_PRIVMSG(this->clients[clientId - 1].getNickname(), iter->getNickname(), msg), fd);
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

    nick.clear();
    iter = getClientByName("BOT");
    if (msgpart == "GAME" && iter != this->clients.end())
        nick =  "GAME " + this->clients[clientId - 1].getNickname();
    else if (strncmp(msgpart.c_str(), "MOVE", 4) == 0 && iter != this->clients.end())
    {
        if (msgpart.size() < 5)
            return (false);
        nick = "MOVE "  +  this->clients[clientId - 1].getNickname() + " " + msgpart.substr(4);
    }
    else if (msgpart == "QUIT" && iter != this->clients.end())
        nick = "QUIT " + this->clients[clientId - 1].getNickname();
    if (nick.empty() == false)
    {
        if (send (iter->getFd(), nick.c_str(), nick.size(), 0) < 0)
            throw std::runtime_error("send field");
        return (true);
    }
    return (false);
}

void Server::privMsg(const std::string &arg, int clientId){
    if (this->args.size() < 3){
        responseFd( ERR_NEEDMOREPARAMS(arg), this->polls[clientId].fd);
        return;
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
    std::stringstream ss(namesPart);
    std::string name;
    while(getline(ss, name, ',')){
        if (name[0] == '#')
            MsgToChannel(name, msgPart, clientId);
        else
            MsgToClient(name, msgPart, clientId);
    }
}