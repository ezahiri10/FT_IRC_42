/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezahiri <ezahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 13:26:43 by ezahiri           #+#    #+#             */
/*   Updated: 2025/02/27 15:59:14 by ezahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
#define BOT_HPP

#include <arpa/inet.h>
#include "Server.hpp"
#include <array>
#include <time.h>

class Bot
{
    private :
        int botfd;
        int port;
        std::string serverpass;
        std::string nick;
        std::array<std::array<char, 3>, 3> s;
        void authentification();
        
    public :
        Bot(const std::string &port, const std::string &pass, const std::string &nick);
        bool setMove ();
        int checkDiagonal(char XO);
        int checkEdge(char XO, int n);
        void ticTacToe();
        void affichTable();
        bool isWin (char XO);
        void botMove();
        bool playerMove(bool &iseof);
        int bestMove(char XO);
        bool isFall();
};

#endif