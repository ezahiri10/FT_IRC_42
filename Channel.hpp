/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fagr <ael-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 15:04:45 by ael-fagr          #+#    #+#             */
/*   Updated: 2025/02/21 15:08:33 by ael-fagr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <vector>
class Channel {
    private:
        std::string name;                       // Channel name
        std::string topic;                      // Current topic of the channel
        std::string password;                   // Channel password (if needed)             
        std::vector<std::string> operators;     //:vector of operator nicknames
        std::vector<std::string> modes;         // Channel mode flags (e.g., +i, +m, +t, etc.)
    public:
        Channel();
        ~Channel();
};

#endif