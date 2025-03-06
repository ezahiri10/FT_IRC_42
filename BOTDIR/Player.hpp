/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezahiri <ezahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 11:22:18 by ezahiri           #+#    #+#             */
/*   Updated: 2025/03/06 23:27:49 by ezahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <arpa/inet.h>
#include <array>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024

class Player
{
    private :
        std::array<std::array<char, 3>, 3> s;
        std::string nickname;
    
        bool playerMove(const std::string &move, int botfd);
        bool isWin (char XO, int botfd);
        int checkEdge(char XO, int n);
        int checkDiagonal(char XO);
        bool isFall(int botfd);
        int bestMove(char XO);
        bool setMove ();
        void botMove();
    public :
        std::string getNickname();
        Player(const std::string &nick);
        bool ticTacToe(const std::string &move, int botfd);
        void getBoard(int botfd);
        static void sendRequest(std::string msg, int botfd);
};

#endif