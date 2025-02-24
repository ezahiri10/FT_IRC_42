/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fagr <ael-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 15:04:45 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/02/24 02:52:11 by ael-fagr         ###   ########.fr       */
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
        std::string name;                       // Channel name
        std::string topic;                      // Current topic of the channel
        std::string password;                   // Channel password (if needed)             
        std::vector<std::string> operators;     //:vector of operator nicknames
        std::vector<std::string> modes;         // Channel mode flags (e.g., +i, +m, +t, etc.)
        std::vector<Client> clients;            //:vector of Clients nicknames
    public:
        Channel();
        ~Channel();

        std::string getName();
        void setName(std::string name);

        int geLimit();
        void setLimit(int name);

        bool geInvited();
        void setInvited(bool invt);

        std::string getTopic();
        void setTopic(std::string topic);

        std::string getPassword();
        void setPassword(std::string password);

        std::vector<std::string> getOperators();
        void setOperators(std::vector<std::string> operators);

        std::vector<std::string> getModes();
        void setModes(std::vector<std::string> modes);

        std::vector<Client>& getClients();
        void setClient(std::vector<Client> client);

        void addClient(const Client& client);
        void removeClient(int pos);
};

#endif