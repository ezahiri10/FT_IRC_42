/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezahiri <ezahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 13:33:21 by ezahiri           #+#    #+#             */
/*   Updated: 2025/02/27 16:10:33 by ezahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

void Bot::authentification()
{
    sockaddr_in add;

    add.sin_family = AF_INET;
    add.sin_port = htons(this->port);
    add.sin_addr.s_addr = INADDR_ANY;
    if (connect(this->botfd, (sockaddr *)&add, sizeof(add)) == -1)
    {
        perror("connect : ");
        throw std::runtime_error ("connect failed");
    }
    std::string msg = "PASS " + this->serverpass + "\n";
    if (send(this->botfd, msg.c_str(), msg.size(), 0) == -1)
        throw std::runtime_error ("send failed");
    msg = "NICK " + this->nick + "\n";
    if (send(this->botfd, msg.c_str(), msg.size(), 0) == -1)
        throw std::runtime_error ("send failed");
    msg = "U U U U\n";
    if (send(this->botfd, msg.c_str(), msg.size(), 0) == -1)
        throw std::runtime_error ("send failed");
}
Bot::Bot(const std::string &port, const std::string &pass, const std::string &nick)
{
    if (port.find_first_not_of("0123456789") != std::string::npos)
        throw std::invalid_argument("invalid port");
    std::stringstream ss(port);
    ss >> this->port;
    if (this->port <= 0 || this->port > 65535)
        throw std::invalid_argument("invalid port");
    this->botfd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->botfd == -1)
        throw std::runtime_error ("socket failed");
    this->serverpass = pass;
    this->nick = nick;
    char tmp[3][3] =  {{'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '9'}};

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            this->s[i][j] = tmp[i][j];
    authentification();
}

bool  Bot::setMove ()
{
    int n;

    n = bestMove('O');
    if (n < 0)
        n = bestMove('X');
    n -= 1;
    if (n > 0 && this->s[n / 3][n % 3] != 'O' && this->s[n / 3][n % 3] != 'X')
    {
        std::cout << "n / 3  " << n / 3  << "  n % 3 " << n % 3 << std::endl;
        this->s[n / 3][n % 3] = 'O';
        return (true);
    }
    return(false);
}

int Bot::checkDiagonal(char XO)
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

int Bot::checkEdge(char XO, int n)
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

int Bot::bestMove(char XO)
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

void Bot::botMove()
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

