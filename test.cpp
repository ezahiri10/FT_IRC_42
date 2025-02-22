void Server::Authentication(std::string message, int clientId) {
    std::string command, arg;

    // Check if client exists, else create it
    if (clients.find(clientId) == clients.end()) {
        clients[clientId] = Client();
        std::cout << "New client created\n";
    }

    // Find the correct pollfd
    int client_fd = -1;
    for (const auto& p : this->polls) {
        if (p.fd == clientId) {
            client_fd = p.fd;
            break;
        }
    }
    if (client_fd == -1) return;  // Client not found in poll list

    // Extract command
    int pos = message.find(' ');
    if (pos == std::string::npos) {
        std::string err = ERR_NEEDMOREPARAMS("UNKNOWN");
        send(client_fd, err.c_str(), err.length(), 0);
        return;
    }
    command = message.substr(0, pos);
    arg = message.substr(pos + 1);
    arg.erase(arg.find_last_not_of("\r\n") + 1); // Trim newlines

    // Handle unknown commands
    if (!checkCommand(command)) {
        std::string err = ERR_UNKNOWNCOMMAND(command);
        send(client_fd, err.c_str(), err.length(), 0);
        return;
    }

    // Get reference to the existing client
    Client& client = clients[clientId];

    // Handle commands
    if (command == "PASS") {
        if (checkPass(arg, this->serverpass, client_fd)) {
            client.setPassword(arg);
            client.has_pass = true;
        }
    } else if (command == "NICK") {
        if (checkNick(arg, client_fd)) {
            client.setNickname(arg);
            client.has_nick = true;
        }
    } else if (command == "USER") {
        if (checkUser(arg, client_fd)) {
            client.setUsername(arg);
            client.has_user = true;
        }
    }

    // Check if client is authenticated
    if (client.Authontacated()) {
        std::cout << "Client authenticated successfully!\n";
    }
}
