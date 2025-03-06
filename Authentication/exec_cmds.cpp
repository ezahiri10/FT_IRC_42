/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmds.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakazdao <yakazdao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 23:22:13 by yakazdao          #+#    #+#             */
/*   Updated: 2025/03/06 14:12:10 by yakazdao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"
#include "../Client.hpp"
#include "../Channel.hpp"


std::string Server::getAllUsers(const std::string &channel){
    std::vector<std::string> allUsers;
    std::vector<std::string> operators;
    std::vector<Client>::iterator iter;
    std::vector<Channel>::iterator chIter = getChannelByName(channel);
    std::string name;
    operators = chIter->getOperators();
    for(iter = chIter->Channelclients.begin(); iter != chIter->Channelclients.end(); iter++){
        for(size_t i = 0; i < operators.size(); i++){
            if (iter->getNickname() == operators[i])
                name = "@"+iter->getNickname();
            else
                name = iter->getNickname();
        }
        allUsers.push_back(name);
    }
    std::string usersList = "";
    for (size_t i = 0; i < allUsers.size(); ++i) {
        usersList += allUsers[i];
        if (i < allUsers.size() - 1) usersList += " ";
    }
    return usersList;
}

void Server::createChannel(const std::string &Ch_name, int clientId){
    Channel newChannel;
    std::vector<Client>::iterator iter;
    std::vector<Channel>::iterator chIter;
    std::vector<Client>::iterator it;

    newChannel.setChannelName(Ch_name);
    iter = getClient(this->polls[clientId].fd);
    newChannel.addOperator(iter->getNickname());
    newChannel.addClient(this->clients[clientId - 1]);
    this->channels.push_back(newChannel);
    std::string users = getAllUsers(Ch_name);
    chIter = getChannelByName(Ch_name);
    for(it = chIter->Channelclients.begin(); it != chIter->Channelclients.end(); it++){
        std::string str;
        str = it->getNickname()+ "!" + it->getUsername();
        responseFd(RPL_JOINMSG(str, this->clients[clientId - 1].getIp(), Ch_name), it->getFd());
        responseFd(RPL_NAMREPLY(this->clients[clientId - 1].getNickname(), Ch_name, users), it->getFd());
    }
    std::cout << RPL_ENDOFNAMES(this->clients[clientId - 1].getNickname(), Ch_name);
}

std::vector<Channel>::iterator Server::getChannelByName(const std::string &name){
    std::vector<Channel>::iterator iter;
    for(iter = this->channels.begin(); iter != this->channels.end(); iter++){
        if(name == iter->getChannelName())
            return (iter);
    }
    return iter;
}

void Server::addClientToChannel(const std::string &Ch_name, const std::string &Ch_pass, int clientId){
    std::vector<Channel>::iterator iter;
    std::vector<Client>::iterator it;
    iter = getChannelByName(Ch_name);
    if (iter->geInvited()){
        responseId(ERR_INVITEONLYCHAN(this->clients[clientId - 1].getNickname(), Ch_name), clientId);return;
    }
    if (clientExistInChannel(Ch_name, clientId))return;
    if (iter->Channelclients.size() + 1 > iter->getChannelLimit()){
        responseId(ERR_CHANNELISFULL(this->clients[clientId - 1].getNickname(), Ch_name), clientId);return;
    }
    if (iter->getIsprivate()){
        if(iter->getChannelPassword() == Ch_pass)
            iter->addClient(this->clients[clientId - 1]);
        else{
            responseId(ERR_INVITEONLYCHAN(this->clients[clientId - 1].getNickname(), Ch_name), clientId);return;
        }
    }else
        iter->addClient(this->clients[clientId - 1]);
    std::string users = getAllUsers(Ch_name);
    for(it = iter->Channelclients.begin(); it != iter->Channelclients.end(); it++){
        std::string str = this->clients[clientId - 1].getNickname()+ "!" + it->getUsername();
        responseFd(RPL_JOINMSG(str, this->clients[clientId - 1].getIp(), Ch_name), it->getFd());
        responseFd(RPL_NAMREPLY(it->getNickname(), Ch_name, users), it->getFd());
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

void Server::join(const std::string &arg, int clientId) {
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
            createChannel(channel, clientId);
        else
            addClientToChannel(channel, pass, clientId);
        i++;
    }
}

void Server::exec_cmds(const std::string &command, const std::string &arg, int clientId){
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
