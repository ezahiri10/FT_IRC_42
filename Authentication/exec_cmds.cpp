/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmds.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezahiri <ezahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 23:22:13 by yakazdao          #+#    #+#             */
/*   Updated: 2025/03/01 16:41:56 by ezahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"
#include "../Client.hpp"
#include "../Channel.hpp"

void Server::createChannel(std::string Ch_name, std::string Ch_pass, int clientId){
    Channel newChannel;
    newChannel.setChannelName(Ch_name);
    if (!Ch_pass.empty()){
        newChannel.setIsprivate(true);
        newChannel.setPassword(Ch_pass);
    }
    std::vector<Client>::iterator iter;
    iter = getClient(this->polls[clientId].fd);
    iter->isOperator = true;
    newChannel.addOperator(iter->getNickname());
    newChannel.addClient(this->clients[clientId - 1]);
    this->channels.push_back(newChannel);
    std::cout << RPL_NAMREPLY(this->clients[clientId - 1].getNickname(), Ch_name, "@"+iter->getNickname());
    std::cout << RPL_ENDOFNAMES(this->clients[clientId - 1].getNickname(), Ch_name);
}

std::vector<Channel>::iterator Server::getChannelByName(std::string name){
    std::vector<Channel>::iterator iter;
    for(iter = this->channels.begin(); iter != this->channels.end(); iter++){
        if(name == iter->getChannelName())
            return (iter);
    }
    return iter;
}

void Server::addClientToChannel(std::string Ch_name, std::string Ch_pass, int clientId){
    std::vector<Channel>::iterator iter;
    iter = getChannelByName(Ch_name);
    if (checkIsClientExistInChannel(Ch_name, clientId))return;
    if (iter->Channelclients.size() + 1 > iter->getChannelLimit()){
        std::string err = ERR_CHANNELISFULL(this->clients[clientId - 1].getNickname(), Ch_name);
        send(this->polls[clientId].fd, err.c_str(), strlen(err.c_str()), 0);return;
    }
    if (iter->getIsprivate()){
        if(iter->getChannelPassword() == Ch_pass)
            iter->addClient(this->clients[clientId - 1]);
        else{
            std::string err = ERR_INVITEONLYCHAN(this->clients[clientId - 1].getNickname(), Ch_name);
            send(this->polls[clientId].fd, err.c_str(), strlen(err.c_str()), 0);return;
        }
    }else{
        iter->addClient(this->clients[clientId - 1]);
    }
    std::cout << RPL_JOIN(this->clients[clientId - 1].getNickname(), Ch_name);
}

std::string getParts(std::string str, char x){
    int pos = 0;
    std::string namePart;
    std::string passPart;
    pos = str.find(' ');
    if (pos == -1)
        passPart = "";
    else
        passPart = str.substr(pos+1);
    namePart = str.substr(0, pos);
    if(x == 'N')
        return (namePart);
    return (passPart);
}

std::string getPass(std::string pass, int i) {
    std::vector<std::string> passList;
    std::stringstream ss(pass);
    std::string token;
    while (getline(ss, token, ',')){
        passList.push_back(token);
    }
    if (i >= 0 && i < (int)passList.size()) {
        return passList[i];
    }
    return "";
}

void Server::join(std::string arg, int clientId) {
    std::string namePart = getParts(arg, 'N');
    std::string passPart = getParts(arg, 'O');
    std::stringstream ss(namePart);
    std::string channel;
    int i = 0;
    while (getline(ss, channel, ',')) {
        std::string pass = getPass(passPart, i);
        if (channel.empty() || channel[0] != '#'){
            std::string err = ERR_NOSUCHCHANNEL(channel);
            send(this->polls[clientId].fd, err.c_str(), err.length(), 0);
            continue;
        }
        if (!checkChannelExist(channel)) {
            createChannel(channel, pass, clientId);
        }
        else{
            addClientToChannel(channel, pass, clientId);
        }
        i++;
    }
}

void Server::exec_cmds(std::string command, std::string arg, int clientId){
    std::vector<Client>::iterator iter;
    iter = getClient(this->polls[clientId].fd);
    if (iter == this->clients.end() || !iter->Authontacated())
        send(this->polls[clientId].fd, ERR_NOTREGISTERED, strlen(ERR_NOTREGISTERED), 0);
    else if (command == "JOIN"){
        if (this->args.size() < 2){
            std::string err = ERR_NEEDMOREPARAMS(arg);
            send(this->polls[clientId].fd, err.c_str(), strlen(err.c_str()), 0);return;
        }
        join(arg, clientId);
    }
    else if (command == "PRIVMSG")
        privMsg(arg, clientId);
    else if (command == "KICK")
        Kick_func(arg, clientId);
    else if (command == "INVITE")
        Invite_func(arg, clientId);
    else if (command == "MODE")
        Mode_func(arg, clientId);
    else if (command == "TOPIC")
        Topic_func(arg, clientId);
}
