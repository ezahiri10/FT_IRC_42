/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezahiri <ezahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 13:28:33 by ezahiri           #+#    #+#             */
/*   Updated: 2025/02/20 17:15:33 by ezahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#define MAX_LOG 128

#define PORT 8080
int main ()
{
    sockaddr_in add;

    add.sin_family = AF_INET;
    add.sin_port = htons(PORT);
    add.sin_addr.s_addr = INADDR_ANY;

    int sockfd = socket (AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("socket :");
        return(1);
    }
    if (bind( sockfd, (const sockaddr *)&add, sizeof(add)) == -1)
    {
        perror("bind :");
        return(1);
    }
    if (listen(sockfd, MAX_LOG) == -1)
    {
        perror("listen :");
        return (1);
    }
    int client_fd = accept(sockfd, nullptr, nullptr);
    if (client_fd == -1)
    {
        perror("accept :");
        return (1);
    }
    struct pollfd pol;
    
    pol.events = POLLIN;
    pol.fd = sockfd;
    // std::cout << "client_fd : " << client_fd << std::endl;
    char s[1000];
    if (poll(&pol, 1, -1) < -1)
        exit (1);
    // std::cout << "salam" <<std::endl;
    // int rv = recv(client_fd, s, 1000, 0);
    // if (rv == 0)
    //     std::cout << "connction is cut" << std::endl;
    // std::cout << rv << std::endl;
    // // send(client_fd, "salam kalam 3aalam", 19, 0);
}