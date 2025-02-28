/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakazdao <yakazdao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 15:04:45 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/02/28 10:38:41 by yakazdao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <vector>
#include "Client.hpp"

class Channel {
    private:
        int         limit;
        bool        invited;
        bool        istopic;
        bool        isPrivate;
        std::string name;                       // Channel name
        std::string topic;                      // Current topic of the channel
        std::string password;                   // Channel password (if needed)             
        std::vector<std::string> operators;     //:vector of operator nicknames
        std::vector<std::string> modes;
    public:
        std::vector<Client> Channelclients;            //:vector of Clients nicknames
        Channel();
        ~Channel();

        std::string getChannelName();
        void setChannelName(std::string name);

        size_t getChannelLimit();
        void setChannelLimit(int limit);
        void incrChannelLimit();

        bool geInvited();
        void setInvited(bool invt);

        bool getistopic();
        void setIstopic(bool istopic);

        std::string getTopic();
        void setTopic(std::string topic);

        std::string getChannelPassword();
        void setPassword(std::string password);

        std::vector<std::string> getOperators();
        void setOperators(std::vector<std::string> operators);

        std::vector<std::string> getModes();
        void setModes(std::vector<std::string> modes);

        std::vector<Client>& getClients();
        void setClient(std::vector<Client> client);

        void addClient(const Client& client);
        void addOperator(const std::string op);
        void addMode(const std::string mode);
        void removeClient(int pos);
        void setIsprivate(bool isPrivate);
        bool getIsprivate();
};

#endif