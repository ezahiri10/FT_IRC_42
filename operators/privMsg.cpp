/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privMsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezahiri <ezahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 11:18:12 by yakazdao          #+#    #+#             */
/*   Updated: 2025/02/28 16:19:56 by ezahiri          ###   ########.fr       */
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
    return this->clients.end();
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
    msg += "\n";
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
        send(this->polls[id].fd, msg.c_str(), strlen(msg.c_str()), 0);
    }
}

void Server::MsgToClient(std::string clientName, std::string msg, int clientId)
{
    std::cout << "step 00" << std::endl;
    std::vector<Client>::iterator iter;
    if (!checkNickAvailability(clientName)){
        std::string err = ERR_NOSUCHNICK(clientName);
        send(this->polls[clientId].fd, err.c_str(), strlen(err.c_str()), 0);
        std::cout << ERR_NOSUCHNICK(clientName);return;
    }
    std::cout << "setp 11" << std::endl;
    msg+="\n";
    std::cout << "clientName : " << clientName << std::endl;
    iter = getClientByName(clientName); //! iter return end
    int id = iter->getFd();
    send(this->polls[id].fd, msg.c_str(), strlen(msg.c_str()), 0); //! heap-buffer-overflow  in int id = iter->getFd(); --> send
    std::cout << "setp 22" << std::endl;
}


void Server::privMsg(std::string arg, int clientId)
{
    std::vector<Client>::iterator iter;

    iter = getClientByName("BOT");
    this->printFd();
    if (this->args.size() < 3){
        std::string err = ERR_NEEDMOREPARAMS(arg);
        send(this->polls[clientId].fd, err.c_str(), strlen(err.c_str()), 0);return;
    }
    std::string namesPart = getPartss(arg, 'N');
    std::string msgPart = getPartss(arg, 'X');
    if (msgPart == " game" && iter != this->clients.end())
    {
        std::string nick = this->clients[clientId - 1].getNickname();
        std::cout << "iter->getFd() : " << iter->getFd() << std::endl; 
        if (send (iter->getFd(), nick.c_str(), nick.size(), 0) < 0)
        {
                perror("send : ");
                throw std::runtime_error("send field");
        }
    }
    std::cout << "step 1" << std::endl;
    std::stringstream ss(namesPart);
    std::string buffer;
    while(getline(ss, buffer, ',')){
        if (buffer[0] == '#')
        {
            std::cout << "step 3" << std::endl;
            MsgToChannel(buffer, msgPart, clientId);
            std::cout << "step 4" << std::endl;
        }
        else
        {
            std::cout << "step 5" << std::endl;
            MsgToClient(buffer, msgPart, clientId);
            std::cout << "step 6" << std::endl;
            
        }
    }
    std::cout << "step 2" << std::endl;
}