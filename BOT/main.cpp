/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezahiri <ezahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 10:06:22 by ezahiri           #+#    #+#             */
/*   Updated: 2025/02/28 12:16:48 by ezahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

int main (int ac, char **av)
{
    try {
        if (ac != 4)
            throw std::invalid_argument("Usage: ./bot <port> <pass> <nick>");
        Bot b(av[1], av[2], av[3]);
        while (1)
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

