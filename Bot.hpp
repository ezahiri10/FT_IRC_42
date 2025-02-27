/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezahiri <ezahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 13:26:43 by ezahiri           #+#    #+#             */
/*   Updated: 2025/02/27 12:41:35 by ezahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
#define BOT_HPP

#include "Server.hpp"
#include <array>
#include <time.h>

class Bot
{
    private :
        std::array<std::array<char, 3>, 3> s;
    public :
        Bot();
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