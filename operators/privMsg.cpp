/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privMsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakazdao <yakazdao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 11:18:12 by yakazdao          #+#    #+#             */
/*   Updated: 2025/02/26 14:59:35 by yakazdao         ###   ########.fr       */
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

// void recieveMsg(int fd){
//     char buffer[1024];
//     recv(fd, buffer, strlen(buffer), 0);
//     std::cout << "::::" <<buffer;
// }

void Server::MsgToChannel(std::string channelName, std::string msg, int clientId){
    std::vector<Channel>::iterator iter;
    std::vector<Client>::iterator cIter;
    if (!checkChannelExist(channelName)){
        std::string err = ERR_NOSUCHCHANNEL(channelName);
        send(this->polls[clientId].fd, err.c_str(), strlen(err.c_str()), 0);
        std::cout << ERR_NOSUCHCHANNEL(channelName);return;
    }
    iter = getChannelByName(channelName);
    for(cIter = iter->Channelclients.begin(); cIter != iter->Channelclients.end(); cIter++){
        int id = cIter->getFd();
        send(this->polls[id].fd, msg.c_str(), strlen(msg.c_str()), 0);
        // std::cout << "NAME:: "<< cIter->getNickname() << std::endl;
        // std::cout << "ID:: "<< cIter->getFd() << std::endl;
    }
}

void Server::MsgToClient(std::string clientName, std::string msg){
    (void)clientName;
    (void)msg;
}

void Server::privMsg(std::string arg, int clientId){
    if (this->args.size() < 3){
        std::string err = ERR_NEEDMOREPARAMS(arg);
        send(this->polls[clientId].fd, err.c_str(), strlen(err.c_str()), 0);return;
    }
    std::string namesPart = getPartss(arg, 'N');
    std::string msgPart = getPartss(arg, 'X');
    std::stringstream ss(namesPart);
    std::string buffer;
    while(getline(ss, buffer, ',')){
        if (buffer[0] == '#')
            MsgToChannel(buffer, msgPart, clientId);
        else
            MsgToClient(buffer, msgPart);
    }
}