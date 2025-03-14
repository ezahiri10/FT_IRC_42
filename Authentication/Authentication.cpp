#include "../Client.hpp"
#include "../Server.hpp"

bool is_valid(std::string &arg){
    int pos = arg.find(' ');
    return (pos == -1 && !arg.empty()) ? true : false;
}

bool checkCommand(std::string &cmd)
{
    return (cmd == "PASS" || cmd == "NICK" || cmd == "USER"
        || cmd == "JOIN" || cmd == "KICK" || cmd == "INVITE"
        || cmd == "TOPIC" || cmd == "MODE" || cmd == "PRIVMSG") ? true: false;
}

bool checkPass(std::string clientPass, std::string serverPass, int clientId, bool XRP){
    if((XRP && ((clientPass != serverPass) || !is_valid(clientPass)))){
        std::string err = ERR_PASSWDMISMATCH(clientPass);
        send(clientId, err.c_str(), err.size(), 0);
        return (false);
    }
    return (true);
}

std::string getClientIP(int client_fd) {
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);

    if (getpeername(client_fd, (struct sockaddr*)&addr, &addr_len) == -1){
        throw std::runtime_error("Error: Could not retrieve client IP!");
    }
    char ipStr[INET_ADDRSTRLEN];
    if (!inet_ntop(AF_INET, &addr.sin_addr, ipStr, sizeof(ipStr))) {
        throw std::runtime_error("Error: inet_ntop failed!");
    }
    return std::string(ipStr);
}

std::string getArg(std::string str){
    int pos = 0;
    pos = str.find(' ');
    str = str.substr(pos + 1);
    str.erase(str.find_last_not_of("\r\n") + 1);
    return str;
}

void Server::pass(const std::string &arg, int clientId){
    std::vector<Client>::iterator iter;
    iter = getClient( this->polls[clientId].fd );
    bool XRP = true;
    if (iter != clients.end() && iter->clientExist){
        responseFd(ERR_ALREADYREGISTRED(iter->getNickname()), this->polls[clientId].fd);
        XRP = false;
    }
    if(checkPass(arg, this->serverpass, this->polls[clientId].fd, XRP)){
        this->clients[clientId - 1].setPassword(arg);
        this->clients[clientId - 1].has_pass = true;
        this->clients[clientId - 1].setFd( this->polls[clientId].fd );
        this->clients[clientId - 1].setIp(getClientIP(this->polls[clientId].fd));
    }
}

void Server::nick(const std::string &arg, int clientId){
    std::vector<Client>::iterator iter;
    iter = getClient( this->polls[clientId].fd );
    if(iter == clients.end() || !iter->has_pass){
        responseFd(ERR_NOTREGISTERED, this->polls[clientId].fd);
    }
    else if (checkNickAvailability(arg)){
        responseFd(ERR_NICKNAMEINUSE(arg), this->polls[clientId].fd);
    }
    else{
        this->clients[clientId - 1].setNickname(arg);
        this->clients[clientId - 1].has_nick = true;
    }
}

void Server::user(const std::string &arg, int clientId){
    std::vector<Client>::iterator iter;
    iter = getClient( this->polls[clientId].fd );
    if(iter == clients.end() || !iter->has_pass){
        responseFd(ERR_NOTREGISTERED, this->polls[clientId].fd);
    }
    else if (this->args.size() < 5)
        responseId(ERR_NEEDMOREPARAMS(arg), clientId);
    else{
        this->clients[clientId - 1].setUsername(arg);
        this->clients[clientId - 1].has_user = true;
    }
}


void Server::Authentication(const std::string &message, int clientId)
{
    std::string command, arg;

    if (message[0] == '\n' || message.empty())
        return;
    getArgs(message);
    arg = getArg(message);
    if (!this->args.empty())
    command = this->args[0];
    if(!checkCommand(command)){
        responseId(ERR_UNKNOWNCOMMAND(command), clientId);
        return ;
    }
    if (this->args.size() < 2){
        responseId(ERR_NEEDMOREPARAMS(command), clientId);
        return ;
    }
    else if (command == "PASS")
        pass(arg, clientId);
    else if(command == "NICK")
        nick(this->args[1], clientId);
    else if(command == "USER")
        user(this->args[1], clientId);
    else
        this->exec_cmds(command,arg, clientId);
    std::vector<Client>::iterator iter;
    iter = getClient( this->polls[clientId].fd );
    if (iter != clients.end() && iter->Authontacated() && !iter->clientExist){
        this->clients[clientId - 1].clientExist = true;
        std::cout << BLUE <<this->clients[clientId - 1].getNickname() << ": Connect To The Server" << RESET << std::endl;
        responseId(RPL_WELCOME(this->clients[clientId - 1].getNickname(), "Welcome To The Irc Server"), clientId);return;
    }
}
