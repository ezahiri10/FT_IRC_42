/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezahiri <ezahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 11:22:18 by ezahiri           #+#    #+#             */
/*   Updated: 2025/02/28 11:27:09 by ezahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "../Server.hpp"
#include <array>

class Player
{
    private :
        std::string nickname;
        std::array<std::array<char, 3>, 3> s;
    public :
        Player(const std::string &nick);
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