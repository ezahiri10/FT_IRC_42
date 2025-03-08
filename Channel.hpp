/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fagr <ael-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 15:04:45 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/03/08 20:33:05 by ael-fagr         ###   ########.fr       */
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
        bool        haslimit;
        bool        istopic;
        bool        isPrivate;
        std::string name;                       // Channel name
        std::string topic;                      // Current topic of the channel
        std::string password;                   // Channel password (if needed)             
        std::vector<std::string> operators;     //:vector of operator nicknames
        std::vector<std::string> invitations;
        std::vector<std::string> modes;
    public:
        std::vector<Client> Channelclients;            //:vector of Clients nicknames
        Channel();
        ~Channel();

        std::string getChannelName() const;
        void setChannelName(const std::string &name);

        size_t getChannelLimit() const;
        void setChannelLimit(const int &limit);
        void incrChannelLimit();

        std::vector<std::string> getInvitations() const;
        void setInvitations(const std::vector<std::string> &client);
        void addInvitations(const std::string& client);

        bool getInvited() const;
        void setInvited(const bool &invt);

        bool getIstopic() const;
        void setIstopic(const bool &istopic);

        bool getHaslimit() const;
        void setHaslimit(const bool &haslimit);

        std::string getTopic() const;
        void setTopic(const std::string &topic);

        std::string getChannelPassword() const;
        void setPassword(const std::string &password);
        std::string getPassword() const;

        std::vector<std::string> getOperators() const;
        void setOperators(const std::vector<std::string> &operators);

        std::vector<std::string> getModes() const;
        void setModes(const std::vector<std::string> &modes);

        std::vector<Client>& getClients();
        void setClient(const std::vector<Client> &client);

        void addClient(const Client& client);
        void addOperator(const std::string &op);
        void addMode(const std::string &mode);
        void removeClient(int pos);
        void setIsprivate(const bool &isPrivate);
        bool getIsprivate() const;
};

#endif