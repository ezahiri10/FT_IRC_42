/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fagr <ael-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 10:06:22 by ezahiri           #+#    #+#             */
/*   Updated: 2025/03/06 21:49:02 by ael-fagr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

int main (int ac, char **av)
{
    try {
        signal(SIGQUIT, Bot::handler);
        signal(SIGINT, Bot::handler);
        if (ac != 3)
            throw std::invalid_argument("Usage: ./bot <port> <pass>");
        Bot b(av[1], av[2]);
        while (true)
        {
            b.recvMesseages(false);
        }
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return (1);
    }
}

