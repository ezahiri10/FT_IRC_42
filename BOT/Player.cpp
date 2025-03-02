/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezahiri <ezahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 11:20:31 by ezahiri           #+#    #+#             */
/*   Updated: 2025/03/02 17:44:11 by ezahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Player.hpp"

void Player::sendRequest(std::string msg, int botfd)
{
    msg += "\r\n";
    if (send(botfd, msg.c_str(), msg.size(), 0) == -1)
        throw std::runtime_error ("send failed");
}

std::string Player::getNickname()
{
    return (this->nickname);
}

Player::Player(const std::string &nick)
{
    this->nickname = nick;
    char tmp[3][3] =  {{'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '9'}};

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            this->s[i][j] = tmp[i][j];
}

void  Player::getBoard(int botfd)
{
    std::string board = "|----|----|----| \n";
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            board += "|  ";
            board += s[i][j];
            board += " ";
        }
        board += "|";
        board += "\n|----|----|----| \n";
    }
    std::string buffer;
    std::stringstream boardStream(board);
    while (getline(boardStream, buffer, '\n'))
    {
        sendRequest("PRIVMSG " + this->nickname + " " + buffer, botfd);
    }
}

bool  Player::setMove ()
{
    int n;

    n = bestMove('O');
    if (n < 0)
        n = bestMove('X');
    n -= 1;
    if (n > 0 && this->s[n / 3][n % 3] != 'O' && this->s[n / 3][n % 3] != 'X')
    {
        this->s[n / 3][n % 3] = 'O';
        return (true);
    }
    return(false);
}

int Player::checkDiagonal(char XO)
{
    char _bot = (XO == 'O') ? 'X' : 'O';

    for (int i = 0; i < 2 ; i++)
    {
        if (this->s[i][i] == XO  && this->s[i + 1][i + 1] == XO && this->s[2 * (i == 0)][2 * (i == 0)] != _bot)
            return (1 + 8 * (i == 0));
    }
     if (this->s[0][0] == XO && this->s[2][2] == XO && this->s[1][1] != _bot)
        return (5);
    return (-1);
}

int Player::checkEdge(char XO, int n)
{
    char _bot = (XO == 'O') ? 'X' : 'O';

    for (int i = 0; i < 2; i++)
    {
        if (this->s[n][i] == XO  && this->s[n][i + 1] == XO && this->s[n][2 * (i == 0)] != _bot)
            return (1 + 2 * (i == 0)); 
    }
    if (this->s[n][0] == XO && this->s[n][2] == XO && this->s[n][1] != _bot)
        return (2);
    return (-1);
}

int Player::bestMove(char XO)
{
    int r = -1;
    int i = 0;

    for ( ;i < 3; i++)
    {
        r = checkEdge (XO, i);
        if (r > 0)
            break ;
    }
    if (r > 0)
        return (r + 3 * (i == 1) + 6 * (i == 2));
    return (checkDiagonal (XO));
}

void Player::botMove()
{
    while (setMove() == false)
    {
        time_t r = time(NULL) % 8;

        if (s[r / 3][r % 3] != 'X' && s[r / 3][r % 3] != 'O')
        {
            s[r / 3][r % 3] = 'O';
            return ;
        }
    }
}


bool Player::isWin (char XO, int botfd)
{
    int j = 0;
    std::string msg;
    if (XO == 'X')
        msg = "You Win";
    else if (XO == 'O')
        msg = "You Lose";
    for (int i =0; i < 3 && j == 0; i++)
    {
        j = 0;
        if (s[i][j] == XO && s[i][j + 1] == XO && s[i][j+ 2] == XO)
            j = 1;
        if (s[j][i] == XO && s[j + 1][i] == XO && s[j+ 2][i] == XO)
            j = 1;
    }
    if (s[0][0] == XO && s[1][1] == XO && s[2][2] == XO)
            j = 1;
    if (s[0][2] == XO && s[1][1] == XO && s[2][0] == XO)
            j = 1;
    if (j == 1)
    {
        getBoard(botfd);
        sendRequest("PRIVMSG " + this->nickname + " " + msg, botfd);
        return true;
    }
    return (false);
}

bool Player::playerMove(const std::string &move, int botfd)
{
    int r;

    if (move.empty() || move.size() != 1 || !std::isdigit(move[0]))
    {
        sendRequest(("PRIVMSG " + this->nickname + " invalid move").c_str(), botfd);
        return (false);
    }
    r = std::atoi(move.c_str()) - 1;
    if (r < 0 || s[r / 3][r % 3] == 'X' || s[r / 3][r % 3] == 'O')
    {
        sendRequest(("PRIVMSG " + this->nickname + " invalid move").c_str(), botfd);
        return (false);
    }
    s[r / 3][r % 3] = 'X';
    return (true);
}


bool Player::isFall(int botfd)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
            if (s[i][j] >= '1' && s[i][j] <= '9')
                return (false);
    }
    sendRequest("PRIVMSG " + this->nickname + " Game Over", botfd);
    return (true);
}


bool Player::ticTacToe(const std::string &move, int botfd)
{
    if (this->isFall(botfd) == true)
        return (true);
    if (this->playerMove(move, botfd) == false)
        return (false);
    if (this->isWin('X', botfd) == true)
        return  (true);
    if (this->isFall(botfd) == true)
        return (true);
    this->botMove();
    if (this->isWin('O', botfd) == true)
        return (true);
    getBoard(botfd);
    return (false);
}