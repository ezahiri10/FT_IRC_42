/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezahiri <ezahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 10:06:22 by ezahiri           #+#    #+#             */
/*   Updated: 2025/02/21 18:43:26 by ezahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"


int main (int ac , char **av)
{
    try
    {
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