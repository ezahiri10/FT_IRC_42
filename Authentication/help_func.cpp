/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help_func.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakazdao <yakazdao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 20:05:17 by yakazdao          #+#    #+#             */
/*   Updated: 2025/02/26 20:08:28 by yakazdao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Client.hpp"
#include "../Server.hpp"


void Server::getArgs(std::string message) {
    this->args.clear();
    size_t endPos = message.find_last_not_of("\r\n");
    if (endPos != std::string::npos) {
        message.erase(endPos + 1);
    }
    std::stringstream ss(message);
    std::string str;
    while (ss >> str)
        this->args.push_back(str);
}

bool Server::checkNickAvailability(const std::string& nick){
    std::vector<Client>::iterator iter;
    for (iter = this->clients.begin(); iter != this->clients.end(); iter++){
        if (iter->getNickname() == nick){
            return true;
        }
    }
    return false;
}
std::vector<Client>::iterator Server::getClient(int fd){
    std::vector<Client>::iterator iter;
    for(iter = clients.begin(); iter != clients.end(); iter++){
        if (iter->getFd() == fd)
            return (iter);
    }
    return clients.end();
}

bool Server::checkChannelExist(std::string channelName){
    std::vector<Channel>::iterator iter;
    for(iter = channels.begin(); iter != channels.end(); iter++){
        if (channelName == iter->getChannelName())
            return (true);
    }
    return (false);
}

bool Server::checkIsClientExistInChannel(std::string chName, int clientId){
    std::vector<Channel>::iterator chIter;
    std::vector<Client>::iterator clIter;
    std::vector<Client>::iterator iter;
    chIter = getChannelByName(chName);
    clIter = getClient(clientId);
    for(iter = chIter->Channelclients.begin(); iter != chIter->Channelclients.end(); iter++){
        if (iter->getNickname() == clIter->getNickname())
            return (true);
    }
    return (false);
}