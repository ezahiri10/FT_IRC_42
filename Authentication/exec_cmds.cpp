/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmds.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fagr <ael-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 23:22:13 by yakazdao          #+#    #+#             */
/*   Updated: 2025/02/25 19:36:29 by ael-fagr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"
#include "../Client.hpp"
#include "../Channel.hpp"


bool Server::checkChannelExist(std::string channelName){
    std::vector<Channel>::iterator iter;
    for(iter = channels.begin(); iter != channels.end(); iter++){
        if (channelName == iter->getChannelName())
            return (true);
    }
    return (false);
}

void Server::createChannel(std::string Ch_name, std::string Ch_pass, int clientId){
    Channel newChannel;
    newChannel.incrChannelLimit();
    newChannel.setChannelName(Ch_name);
    if (!Ch_pass.empty()){
        newChannel.setIsprivate(true);
        newChannel.setPassword(Ch_pass);
    }
    std::vector<Client>::iterator iter;
    iter = getClient(clientId);
    iter->isOperator = true;
    newChannel.addOperator(iter->getNickname());
    newChannel.addClient(this->clients[clientId - 1]);
    this->channels.push_back(newChannel);
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
    if (iter->getChannelLimit() > 10){
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
    iter->incrChannelLimit();
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
    //!!!!!    TEST CHANNELS HERE : FOR ABDELLATIF OTATA ===============
    // COMPLATE YOUR WORK BASED ON THIS CHANNEL INFO
    std::cout << "=============size: "<<this->channels.size()<<std::endl;
    std::vector<Channel>::iterator iter;
    std::vector<Client>::iterator iterc;
    for(iter=channels.begin(); iter!=channels.end();iter++){
        std::cout << "=======CHNNEL========"<<std::endl;
        std::cout << "channel name  : "<<iter->getChannelName() <<std::endl;
        std::cout << "channel pass  : "<<iter->getChannelPassword() <<std::endl;
        std::cout << "channel isprv : "<<iter->getIsprivate() <<std::endl;
        std::cout << "   client exist in the channel are:"<<std::endl;
        for (iterc=iter->Channelclients.begin(); iterc!=iter->Channelclients.end();iterc++){
            std::cout << "      ::::clientname : "<<iterc->getNickname() <<std::endl;
            std::cout << "      ::::clientOper : "<<iterc->isOperator <<std::endl;
        }
    }
}

void Server::exec_cmds(std::string command, std::string arg, int clientId){
    std::vector<Client>::iterator iter;
    iter = getClient(clientId);
    if (iter == this->clients.end() || !iter->Authontacated()){
        send(this->polls[clientId].fd, ERR_NOTREGISTERED, strlen(ERR_NOTREGISTERED), 0);
    }
    else if (command == "JOIN"){
        if (this->args.size() < 2){
            std::string err = ERR_NEEDMOREPARAMS(arg);
            send(this->polls[clientId].fd, err.c_str(), strlen(err.c_str()), 0);return;
        }
        join(arg, clientId);
    }
    else if (command == "KICK")
    {
        std::cout << "KICK\n";
        Kick_func(arg, this->polls[clientId].fd);
    }
    else if (command == "INVITE")
        Invite_func(arg, this->polls[clientId].fd);
    else if (command == "MODE")
        Mode_func(arg, this->polls[clientId].fd);
    else if (command == "TOPIC")
        Topic_func(arg, this->polls[clientId].fd);
}
