/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operators.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fagr <ael-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 20:58:52 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/02/24 04:58:48 by ael-fagr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPERATORS_HPP
#define OPERATORS_HPP

#include "../Channel.hpp"
#include "../Server.hpp"
#include "../Replies.hpp"
#include <set>
#include <iostream>

int split_3_arguments(std::string arg, std::string &p1, std::string &p2);
std::string Get_client_nick(Server *My_serv, int client_Fd);
bool there_is_Fd(Server *My_serv, int fd);
void send_message(Server *My_serv, std::string message, int clientFD);
bool there_is_user(Server *My_serv, std::string client, int client_Fd);
bool there_is_channel(Server *My_serv, std::string channel, int &index, int clientFD);
int split_4_arguments(std::string arg, std::string &channel, std::string &client, std::string &p3);
bool already_on_channel(Server *My_serv, std::string client, std::string channel, int client_Fd, int index, int check);
      

#endif
