/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Authentication.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezahiri <ezahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 14:47:41 by ezahiri           #+#    #+#             */
/*   Updated: 2025/02/28 16:15:47 by ezahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Client.hpp"
#include "../Server.hpp"

bool is_valid(std::string arg){
    int pos = arg.find(' ');
    return (pos == -1 && !arg.empty()) ? true : false;
}

bool checkCommand(std::string cmd){
    return (cmd == "PASS" || cmd == "NICK" || cmd == "USER"
        || cmd == "JOIN" || cmd == "KICK" || cmd == "INVITE"
        || cmd == "TOPIC" || cmd == "MODE" || cmd == "PRIVMSG") ? true: false;
}

bool checkPass(std::string clientPass, std::string serverPass, int clientId, bool XRP){
    if((XRP && ((clientPass != serverPass) || !is_valid(clientPass)))){
        std::string err = ERR_PASSWDMISMATCH(clientPass);
        send(clientId, err.c_str(), strlen(err.c_str()), 0);
        return (false);
    }
    std::cout << "Password is correct" << std::endl;
    return (true);
}

std::string getArg(std::string str){
    int pos = 0;
    pos = str.find(' ');
    str = str.substr(pos + 1);
    str.erase(str.find_last_not_of("\r\n") + 1);
    return str;
}

void Server::pass(std::string arg, int clientId){
    std::vector<Client>::iterator iter;
    iter = getClient(this->polls[clientId].fd);
    bool XRP = true;
    if (iter != clients.end() && iter->clientExist){
        std::string err = ERR_ALREADYREGISTRED(iter->getNickname());
        send(this->polls[clientId].fd, err.c_str(), strlen(err.c_str()), 0);
        XRP = false;
    }
    if(checkPass(arg, this->serverpass, this->polls[clientId].fd, XRP)){
        Client newClient;
        newClient.setFd(this->polls[clientId].fd); //! ERRset the fd of the client not the index
        newClient.setPassword(arg);
        newClient.has_pass = true;
        this->clients.push_back(newClient);
    }
}

void Server::nick(std::string arg, int clientId){
    std::vector<Client>::iterator iter;
    iter = getClient(this->polls[clientId].fd);
    if(iter == clients.end() || !iter->has_pass){
        send(this->polls[clientId].fd, ERR_NOTREGISTERED, strlen(ERR_NOTREGISTERED), 0);
        return;
    }
    else if (checkNickAvailability(arg)){
        std::string err = ERR_NICKNAMEINUSE(arg);
        send(this->polls[clientId].fd, err.c_str(), strlen(err.c_str()), 0);
    }
    else{
        this->clients[clientId - 1].setNickname(arg);
        this->clients[clientId - 1].has_nick = true;
    }
}

void Server::user(std::string arg, int clientId){
    std::vector<Client>::iterator iter;
    iter = getClient(this->polls[clientId].fd);
    if(iter == clients.end() || !iter->has_pass){
        send(this->polls[clientId].fd, ERR_NOTREGISTERED, strlen(ERR_NOTREGISTERED), 0);
        return;
    }
    else if (this->args.size() < 5){
        std::string err = ERR_NEEDMOREPARAMS(arg);
        send(this->polls[clientId].fd, err.c_str(), strlen(err.c_str()), 0);return;
    }
    else{
        this->clients[clientId - 1].setUsername(arg);
        this->clients[clientId - 1].has_user = true;
    }
}

void Server::printFd() // for debugging
{
    std::cout << " ----------------- Fd ----------------- " << std::endl;
    for (int i = 0; i < (int)this->polls.size(); i++)
    {
        std::cout << " Fd : " << this->polls[i].fd << std::endl;
    }
    std::cout << " ----------------- Fd Clients ----------------- " << std::endl;
    for (int i = 0; i < (int)this->clients.size(); i++)
    {
        std::cout << "Nick : " << this->clients[i].getNickname() << " fd : " << this->clients[i].getFd() << std::endl;
    }
    std::cout << "----------------------------------------" << std::endl;
}

void Server::Authentication(std::string message, int clientId){
    if (message[0] == '\n')return;
    std::string command, arg;
    getArgs(message);
    arg = getArg(message);
    if (!this->args.empty())
    command = this->args[0];
    if (this->args.size() < 2){
        std::string err = ERR_NEEDMOREPARAMS(command);
        send(this->polls[clientId].fd, err.c_str(), strlen(err.c_str()), 0);return;
    }
    if(!checkCommand(command)){
        std::string err = ERR_UNKNOWNCOMMAND(command);
        send(this->polls[clientId].fd, err.c_str(), strlen(err.c_str()), 0);return;
    }
    else if (command == "PASS")
        pass(arg, clientId);
    else if(command == "NICK")
        nick(this->args[1], clientId);
    else if(command == "USER")
        user(this->args[1], clientId);
    else
        this->exec_cmds(command,arg, clientId);
    std::vector<Client>::iterator iter;
    iter = getClient(this->polls[clientId].fd);
    if (iter != clients.end() && iter->Authontacated() && !iter->clientExist){
        this->clients[clientId - 1].clientExist = true;
        std::string welc = RPL_WELCOME(this->clients[clientId - 1].getNickname(), "Welcome To The Irc Server");
        send(this->polls[clientId].fd, welc.c_str(), strlen(welc.c_str()), 0);return;
    }
}