/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezahiri <ezahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 10:06:22 by ezahiri           #+#    #+#             */
/*   Updated: 2025/02/21 19:04:43 by ezahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"


int main (int ac , char **av)
{
    try
    {
        signal(SIGQUIT, Server::handler);
        signal(SIGINT, Server::handler);
        if (ac != 3)
            throw std::invalid_argument ("Not enough of Arguments");
        Server serv (av[1], av[2]);
        serv.creatServer();
    }
    catch(std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return (1);
    }
}