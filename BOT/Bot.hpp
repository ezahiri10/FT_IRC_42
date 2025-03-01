/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezahiri <ezahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 13:26:43 by ezahiri           #+#    #+#             */
/*   Updated: 2025/03/01 16:36:43 by ezahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
#define BOT_HPP

#include <arpa/inet.h>
#include "../Server.hpp"
#include <array>
#include <time.h>
#include "Player.hpp"

class Bot
{
    private :
        int botfd;
        int port;
        std::vector<Player> players;
        std::string serverpass;
        std::string nick;
        void authentification();
        void sendRequest(std::string msg);
        int getPlayerByName(const std::string &name);
        
    public :
        void parseRequest(std::string msg);
        Bot(const std::string &port, const std::string &pass, const std::string &nick);
        void recvMesseages(bool welcom );
};

#endif