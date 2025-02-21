#include "Server.hpp"

//KICK #&food nickname reason
//Test Write kick command in irc server

void send_message(Server *My_serv, std::string message)
{
    std::vector<Client>::iterator it;
    int sock_fd;
    for (it = My_serv->clients.begin(); it < My_serv->clients.end(); it++)
    {
        sock_fd = (*it).fd;
        send(sock_fd, message.c_str(), message.length(), 0);
    }
}

bool there_is_user(Server *My_serv,std::string channel, std::string user, std::string reasen)
{
    std::vector<Client>::iterator it;
    for (it = My_serv->clients.begin(); it < My_serv->clients.end(); it++){
        if (!strcmp(user.c_str(), (*it).nick_name.c_str()))
        {
            std::cout << user << " Kick The Channell" << channel;
            if (!reasen.empty())
                std::cout << " cause " << reasen << std::endl;
            else
                std::cout << std::endl;
            std::string msg = user + " Kick The Channell " + channel;
            send_message(My_serv, msg);
            My_serv->clients.erase(it);
        }
    }
}

int split_argument(std::string arg, std::string &channel, std::string &user, std::string &reason)
{
    std::istringstream stream(arg);
    std::string temp;

    if (std::getline(stream, temp, ' '))
        channel = temp;
    else
        return (0);

    if (std::getline(stream, temp, ' '))
        user = temp;
    else
        return (0);

    std::getline(stream, reason);
    return 1;
}
bool Check_send(Server *My_serv, std::string arg)
{
    if (arg.empty())
        return (false);
    std::string user;
    std::string channel;
    std::string reasen;
    int bol;
    bol = split_argument(arg, channel, user, reasen);
    std::cout << "Channel: " << channel << std::endl;
    std::cout << "User: " << user << std::endl;
    std::cout << "Reason: " << reasen << std::endl;
    if (!bol)
    {
        std::cerr << "Invalid NUmber of argiments" << std::endl;
        return (false);
    }
    
    if (channel[0] == '#' || channel[0] == '&')
    {
        std::vector<Channels>::iterator it;
        for (it = My_serv->Channels.begin(); it < My_serv->Channels.end(); it++){
            if (!strcmp(channel.c_str(), (*it).nick_name.c_str()))
            {
                there_is_user(My_serv, channel, user, reasen);
            }
        }
        return (true);
    }
    std::cerr << "No such channel" << std::endl;
    return (false);
}

int main_func(void)
{
    Server My_serv;
    std::string arg = "KICK #CHANNEL NICK REASON";
    Check_send(&My_serv, arg);
}