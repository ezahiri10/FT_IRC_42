/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmds.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakazdao <yakazdao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 23:22:13 by yakazdao          #+#    #+#             */
/*   Updated: 2025/02/25 00:09:45 by yakazdao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"
#include "../Client.hpp"
#include "../Channel.hpp"


bool Server::checkChannelExist(std::string channelName){
    std::vector<Channel>::iterator iter;
    for(iter = channels.begin(); iter != channels.end(); iter++){
        if (channelName == iter->getName())
            return (false);
    }
    return (true);
}

void Server::createChannel(std::string Ch_name, std::string Ch_pass, bool isOperator, int clientId){
    Channel newChannel;
    newChannel.setName(Ch_name);
    newChannel.setPassword(Ch_pass);
    std::vector<Client>::iterator iter;
    iter = getClient(clientId);
    if (isOperator){
        iter->isOperator = true;
        newChannel.addOperator(iter->getNickname());
    }
    this->channels.push_back(newChannel);
}

void Server::addToChannel(std::string Ch_name, std::string Ch_pass, bool isOperator, int clientId){
    (void)Ch_name;
    (void)Ch_pass;
    (void)isOperator;
    (void)clientId;
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

std::string getPass(std::string pass, int i){
    char *cstr = strdup(pass.c_str());
    char* str;
    str = strtok(cstr, ",");
    int j = 0;
    while(str){
        if(i++ == j++)
            return (str);
        str = strtok(NULL, ",");
    }
    return "";
}

void Server::join(std::string arg, int clientId){
    std::string namePart = getParts(arg, 'N');
    std::string passPart = getParts(arg, 'O');
    std::string pass;
    char *cstr = strdup(passPart.c_str());
    char* str;
    str = strtok(cstr, ",");
    // int i = 0;
    while(str){
        // pass = getPass(passPart, i++);
        // std::cout << "p|"<<pass<<"|"<<std::endl;
        std::cout << "n|"<<str<<"|"<<std::endl;
        if(str[0] != '#'){
            std::string k = str;
            std::string err = ERR_NOSUCHCHANNEL(k);
            send(this->polls[clientId].fd, err.c_str(), strlen(err.c_str()), 0);return;
        }
        // else if (!checkChannelExist(str)){
        //     createChannel(str, pass, true, clientId);
        // }else{
        //     addToChannel(str, pass, false, clientId);
        // }
        str = strtok(NULL, ",");
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
        Kick_func(this, arg, this->polls[clientId].fd);
    else if (command == "INVITE")
        Invite_func(this, arg, this->polls[clientId].fd);
    else if (command == "MODE")
        Mode_func(this, arg, this->polls[clientId].fd);
    else if (command == "TOPIC")
        Topic_func(this, arg, this->polls[clientId].fd);
}
