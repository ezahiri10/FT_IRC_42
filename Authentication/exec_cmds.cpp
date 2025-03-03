/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmds.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakazdao <yakazdao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 23:22:13 by yakazdao          #+#    #+#             */
/*   Updated: 2025/03/03 16:40:16 by yakazdao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"
#include "../Client.hpp"
#include "../Channel.hpp"

void Server::responseId(const std::string &str, int clientId){
    send(this->polls[clientId].fd, str.c_str(), strlen(str.c_str()), 0);
}

void Server::responseFd(const std::string &str, int fd){
    send(fd, str.c_str(), strlen(str.c_str()), 0);
}

std::vector<std::string> Server::getAllUsers(std::string channel){
    std::vector<std::string> allUsers;
    std::vector<Client>::iterator iter;
    std::vector<Channel>::iterator chIter = getChannelByName(channel);
    std::string name;
    for(iter = chIter->Channelclients.begin(); iter != chIter->Channelclients.end(); iter++){
        allUsers.push_back(iter->getNickname());
    }
    return allUsers;
}

void Server::createChannel(std::string Ch_name, std::string Ch_pass, int clientId){
    Channel newChannel;
    newChannel.setChannelName(Ch_name);
    if (!Ch_pass.empty()){
        newChannel.setIsprivate(true);
        newChannel.setPassword(Ch_pass);
    }
    std::vector<Client>::iterator iter;
    iter = getClient(this->polls[clientId].fd);
    newChannel.addOperator(iter->getNickname());
    newChannel.addClient(this->clients[clientId - 1]);
    this->channels.push_back(newChannel);
    responseId(RPL_NAMREPLY(this->clients[clientId - 1].getNickname(), Ch_name, "@"+iter->getNickname()), clientId);
    responseId(RPL_ENDOFNAMES(this->clients[clientId - 1].getNickname(), Ch_name),clientId );
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
    std::vector<Client>::iterator it;
    iter = getChannelByName(Ch_name);
    if (iter->geInvited()){
        responseId(ERR_INVITEONLYCHAN(this->clients[clientId - 1].getNickname(), Ch_name), clientId);return;
    }
    if (checkIsClientExistInChannel(Ch_name, clientId))return;
    if (iter->Channelclients.size() + 1 > iter->getChannelLimit()){
        responseId(ERR_CHANNELISFULL(this->clients[clientId - 1].getNickname(), Ch_name), clientId);return;
    }
    if (iter->getIsprivate()){
        if(iter->getChannelPassword() == Ch_pass)
            iter->addClient(this->clients[clientId - 1]);
        else{
            responseId(ERR_INVITEONLYCHAN(this->clients[clientId - 1].getNickname(), Ch_name), clientId);return;
        }
    }else{
        iter->addClient(this->clients[clientId - 1]);
    }
    for(it = iter->Channelclients.begin(); it != iter->Channelclients.end(); it++){
        responseFd(RPL_JOIN(it->getNickname(), Ch_name), it->getFd());
    }

    std::vector<std::string> users = getAllUsers(Ch_name);
    std::string usersList = "";

    for (size_t i = 0; i < users.size(); ++i) {
        usersList += users[i];
        if (i < users.size() - 1) usersList += " "; 
    }
    responseId(RPL_NAMREPLY(this->clients[clientId - 1].getNickname(), Ch_name, usersList), clientId);
    // responseId(RPL_ENDOFNAMES(this->clients[clientId - 1].getNickname(), Ch_name),clientId );
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
        if (!checkChannelExist(channel))
            createChannel(channel, pass, clientId);
        else
            addClientToChannel(channel, pass, clientId);
        i++;
    }
}

void Server::exec_cmds(std::string command, std::string arg, int clientId){
    Operators op;
    std::vector<Client>::iterator iter;
    iter = getClient( this->polls[clientId].fd);
    if (iter == this->clients.end() || !iter->Authontacated())
        responseId(ERR_NOTREGISTERED, clientId);
    else if (command == "JOIN"){
        if (this->args.size() < 2){
            responseId(ERR_NEEDMOREPARAMS(arg), clientId);return;
        }
        join(arg, clientId);
    }
    else if (command == "PRIVMSG")
        privMsg(arg, clientId);
    else if (command == "KICK")
        op.Kick_func(*this, arg, clientId);
    else if (command == "INVITE")
        op.Invite_func(*this, arg, clientId);
    else if (command == "MODE")
        op.Mode_func(*this, arg, clientId);
    else if (command == "TOPIC")
        op.Topic_func(*this, arg, clientId);
}
