/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fagr <ael-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 15:22:10 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/03/08 02:30:54 by ael-fagr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(){
    this->istopic = false;
    this->limit = 102;
    this->name = "";
    this->password = "";
    this->topic = "";
    this->isPrivate = false;
}
Channel::~Channel(){

}
std::string Channel::getChannelName() const{
    return (this->name);
}
void Channel::setChannelName(const std::string &name){
    this->name = name;
}

size_t Channel::getChannelLimit() const{
    return (this->limit);
}
void Channel::setChannelLimit(const int &limit){
    this->limit = limit;
}

void Channel::incrChannelLimit(){
    this->limit++;
}


bool Channel::getInvited() const{
    return (this->invited);
}
void Channel::setInvited(const bool &invt){
    this->invited = invt;
}
std::vector<std::string> Channel::getInvitations() const{
    return (this->invitations);
}
void Channel::setInvitations(const std::vector<std::string> &client){
    this->invitations = client;
}

std::string Channel::getTopic() const{
    return (this->topic);
}
void Channel::setTopic(const std::string &topic){
    this->topic = topic;
}
std::string Channel::getChannelPassword() const{
    return (this->password);
}
void Channel::setPassword(const std::string &password){
    this->password = password;
}

std::string Channel::getPassword() const{
    return (this->password);
}
std::vector<std::string> Channel::getOperators() const{
    return (this->operators);
}
void Channel::setOperators(const std::vector<std::string> &operators){
    this->operators = operators;
}
std::vector<std::string> Channel::getModes() const{
    return (this->modes);
}
void Channel::setModes(const std::vector<std::string> &modes){
    this->modes = modes;
}

std::vector<Client>& Channel::getClients(){
    return (this->Channelclients);
}
void Channel::setClient(const std::vector<Client> &client){
    this->Channelclients = client;
}

void Channel::addClient(const Client& client) {
    this->Channelclients.push_back(client);
}

void Channel::removeClient(int pos){
    this->Channelclients.erase(this->Channelclients.begin() + pos);
}
void Channel::addOperator(const std::string &op){
    this->operators.push_back(op);
}
void Channel::addMode(const std::string &mode){
    this->modes.push_back(mode);
}
void Channel::addInvitations(const std::string& client){
    this->invitations.push_back(client);
}
void Channel::setIsprivate(const bool &isPrivate){
    this->isPrivate = isPrivate;
}
bool Channel::getIsprivate() const{
    return this->isPrivate;
}
void Channel::setIstopic(const bool &istopic){
    this->istopic = istopic;
}
bool Channel::getIstopic() const{
    return this->istopic;
}

bool Channel::getHaslimit() const {
    return (this->haslimit);
}
void Channel::setHaslimit(const bool &haslimit){
    this->haslimit = haslimit;
}
// void Channel::clearChannel() {
//     this->clients.clear();
//     this->operators.clear(); 
//     this->topic.clear(); 
//     this->name.clear();   
// }
