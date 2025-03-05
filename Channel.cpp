/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezahiri <ezahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 15:22:10 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/03/05 02:32:28 by ezahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(){
    this->invited = false;
    this->istopic = false;
    this->limit = 12;
    this->name = "";
    this->password = "";
    this->topic = "";
    this->isPrivate = false;
}
Channel::~Channel(){

}
std::string Channel::getChannelName(){
    return (this->name);
}
void Channel::setChannelName(std::string name){
    this->name = name;
}

size_t Channel::getChannelLimit(){
    return (this->limit);
}
void Channel::setChannelLimit(int limit){
    this->limit = limit;
}

void Channel::incrChannelLimit(){
    this->limit++;
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
std::string Channel::getChannelPassword(){
    return (this->password);
}
void Channel::setPassword(std::string password){
    this->password = password;
}

std::string Channel::getPassword(){
    return (this->password);
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
    return (this->Channelclients);
}
void Channel::setClient(std::vector<Client> client){
    this->Channelclients = client;
}

void Channel::addClient(const Client& client) {
    this->Channelclients.push_back(client);
}

void Channel::removeClient(int pos){
    this->Channelclients.erase(this->Channelclients.begin() + pos);
}
void Channel::addOperator(const std::string op){
    this->operators.push_back(op);
}
void Channel::addMode(const std::string mode){
    this->modes.push_back(mode);
}
void Channel::setIsprivate(bool isPrivate){
    this->isPrivate = isPrivate;
}
bool Channel::getIsprivate(){
    return this->isPrivate;
}
void Channel::setIstopic(bool istopic){
    this->istopic = istopic;
}
bool Channel::getistopic(){
    return this->istopic;
}
// void Channel::clearChannel() {
//     this->clients.clear();
//     this->operators.clear(); 
//     this->topic.clear(); 
//     this->name.clear();   
// }
