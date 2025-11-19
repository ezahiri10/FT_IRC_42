/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakazdao <yakazdao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 10:06:22 by ezahiri           #+#    #+#             */
/*   Updated: 2025/03/13 14:20:02 by yakazdao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void printWelcomeBanner(){
    std::cout << BLUE <<
       "IIIIIIIIIIRRRRRRRRRRRRRRRRR           CCCCCCCCCCCCC\n"
        "I::::::::IR::::::::::::::::R       CCC::::::::::::C\n"
        "I::::::::IR::::::RRRRRR:::::R    CC:::::::::::::::C\n"
        "II::::::IIRR:::::R     R:::::R  C:::::CCCCCCCC::::C\n"
        "  I::::I    R::::R     R:::::R C:::::C       CCCCCC\n"
        "  I::::I    R::::R     R:::::RC:::::C              \n"
        "  I::::I    R::::RRRRRR:::::R C:::::C              \n"
        "  I::::I    R:::::::::::::RR  C:::::C              \n"
        "  I::::I    R::::RRRRRR:::::R C:::::C              \n"
        "  I::::I    R::::R     R:::::RC:::::C              \n"
        "  I::::I    R::::R     R:::::RC:::::C              \n"
        "  I::::I    R::::R     R:::::R C:::::C       CCCCCC\n"
        "II::::::IIRR:::::R     R:::::R  C:::::CCCCCCCC::::C\n"
        "I::::::::IR::::::R     R:::::R   CC:::::::::::::::C\n"
        "I::::::::IR::::::R     R:::::R     CCC::::::::::::C\n"
        "IIIIIIIIIIRRRRRRRR     RRRRRRR        CCCCCCCCCCCCC\n";
        std::cout << YELLOW << "Welcome To The IRC Server :"<<RESET << std::endl<<std::endl;
}

int main (int ac , char **av)
{
    try
    {
        signal(SIGPIPE, SIG_IGN);
        signal(SIGQUIT, Server::handler);
        signal(SIGINT, Server::handler);
        if (ac != 3)
            throw std::invalid_argument ("Not enough of Arguments");
        printWelcomeBanner();
        Server serv (av[1], av[2]);
        serv.creatServer();
        
    }
    catch(std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return (1);
    }
}