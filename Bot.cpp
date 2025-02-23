/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakazdao <yakazdao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 13:33:21 by ezahiri           #+#    #+#             */
/*   Updated: 2025/02/23 21:59:31 by yakazdao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

Bot::Bot()
{
    char tmp[3][3] =  {{'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '9'}};

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            this->s[i][j] = tmp[i][j];
}

void Bot::botMove()
{
    int r = 0;
    while (true)
    {
        if (r == 8)
            r = 0;
        if (s[r / 3][r % 3] != 'X' && s[r / 3][r % 3] != 'O')
        {
            s[r / 3][r % 3] = 'O';
            return ;
        }
        r++;
    }
}

void Bot::affichTable()
{
    std::cout << "|----|----|----|" << std::endl;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            std::cout << "|  "  << s[i][j] << " " ;
        }
        std::cout << "|" << std::endl;
        std::cout << "|----|----|----| " << std::endl;
    }
}

bool Bot::isWin (char XO)
{
    int j = 0;
    std::string msg;
    if (XO == 'X')
        msg = "You Wiiiin";
    else if (XO == 'O')
        msg = "You  Lose" ;
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
        this->affichTable();
        std::cout << msg << std::endl;
        return true;
    }
    return (false);
}

bool Bot::playerMove(bool &iseof)
{
    std::string line;
    int r;

    std::cout << "select numbet 1 ~ 9 : ";
    getline(std::cin, line);
    if (std::cin.eof())
    {
        iseof = true;
        std::cout << "\nBye" << std::endl;
        return (false);
    }
    if (line.empty() || line.size() != 1 || !std::isdigit(line[0]))
    {
        std::cout << "invalid argement" << std::endl;
        return (false);
    }
    r = std::atoi(line.c_str()) - 1;
    if (r < 0 || s[r / 3][r % 3] == 'X' || s[r / 3][r % 3] == 'O')
    {
        std::cout << "invalid argement" << std::endl;
        return (false);
    }
    s[r / 3][r % 3] = 'X';
    return (true);
}


bool Bot::isFall()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
            if (s[i][j] >= '1' && s[i][j] <= '9')
                return (false);
    }
    this->affichTable();
    std::cout << "Draw" << std::endl;
    return (true);
}

 void Bot::ticTacToe()
 {
    bool iseof = false;
    std::cout << "select numbet 1 ~ 9 : " << std::endl;
    this->affichTable();
    while (iseof == false)
    {
        if (this->isFall())
            break ;
        if (!this->playerMove(iseof))
            continue;
        if (this->isWin('X') == true)
            break ;
        if (this->isFall())
            break ;
        this->botMove();
        if (this->isWin('O') == true)
            break ;
        this->affichTable();
    }
    
 }

