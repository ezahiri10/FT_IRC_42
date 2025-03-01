/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privMsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakazdao <yakazdao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 11:18:12 by yakazdao          #+#    #+#             */
/*   Updated: 2025/03/01 22:59:04 by yakazdao         ###   ########.fr       */
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
        send(id, msg.c_str(), strlen(msg.c_str()), 0);
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
    send(id, msg.c_str(), strlen(msg.c_str()), 0);
}

std::string trim(const std::string & source) {
    std::string s(source);
    s.erase(0,s.find_first_not_of(" \n\r\t"));
    s.erase(s.find_last_not_of(" \n\r\t")+1);
    return s;
}

void Server::privMsg(std::string arg, int clientId){
    if (this->args.size() < 3){
        std::string err = ERR_NEEDMOREPARAMS(arg);
        send(this->polls[clientId].fd, err.c_str(), strlen(err.c_str()), 0);return;
    }
    std::string namesPart = getPartss(arg, 'N');
    std::string msgPart = getPartss(arg, 'X');
    msgPart = trim(msgPart);
    std::stringstream ss(namesPart);
    std::string name;
    while(getline(ss, name, ',')){
        msgPart += "\r\n";
        if (name[0] == '#')
            MsgToChannel(name, msgPart, clientId);
        else
            MsgToClient(name, msgPart, clientId);
    }
}