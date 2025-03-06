/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fagr <ael-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 13:26:43 by ezahiri           #+#    #+#             */
/*   Updated: 2025/03/06 21:44:22 by ael-fagr         ###   ########.fr       */
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
        void parseRequest(const std::string &msg);

    public :
        static int forclose;
        Bot(const std::string &port, const std::string &pass);
        void recvMesseages(bool welcom );
        static void handler(int signum);
        void ifFailed(const std::string &e);
        ~Bot();
};

#endif