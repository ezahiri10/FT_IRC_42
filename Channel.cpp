/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fagr <ael-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 15:22:10 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/02/24 02:44:12 by ael-fagr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(){
    this->invited = false;
    this->limit = 10;
    this->name = "";
    this->password = "";
    this->topic = "";
}
Channel::~Channel(){

}
std::string Channel::getName(){
    return (this->name);
}
void Channel::setName(std::string name){
    this->name = name;
}

int Channel::geLimit(){
    return (this->limit);
}
void Channel::setLimit(int limit){
    this->limit = limit;
}

bool Channel::geInvited(){
    return (this->invited);
}
void Channel::setInvited(bool invt){
    this->invited = invt;
}

std::string Channel::getTopic(){
    return (this->topic);
}
void Channel::setTopic(std::string topic){
    this->topic = topic;
}
std::string Channel::getPassword(){
    return (this->password);
}
void Channel::setPassword(std::string password){
    this->password = password;
}
std::vector<std::string> Channel::getOperators(){
    return (this->operators);
}
void Channel::setOperators(std::vector<std::string> operators){
    this->operators = operators;
}
std::vector<std::string> Channel::getModes(){
    return (this->modes);
}
void Channel::setModes(std::vector<std::string> modes){
    this->modes = modes;
}

std::vector<Client>& Channel::getClients(){
    return (this->clients);
}
void Channel::setClient(std::vector<Client> client){
    this->clients = client;
}

void Channel::addClient(const Client& client) {
        this->clients.push_back(client);
}

void Channel::removeClient(int pos){
    this->clients.erase(this->clients.begin() + pos);
}

// void Channel::clearChannel() {
//     this->clients.clear();
//     this->operators.clear(); 
//     this->topic.clear(); 
//     this->name.clear();   
// }
