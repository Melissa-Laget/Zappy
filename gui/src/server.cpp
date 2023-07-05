/*
** EPITECH PROJECT, 2023
** Zappy
** File description:
** server
*/

#include "gui.hpp"

void GUI::setup_fd_set(fd_set *read_fds, int sockfd)
{
    FD_ZERO(read_fds);
    FD_SET(sockfd, read_fds);
    FD_SET(STDIN_FILENO, read_fds);
}

void GUI::handle_read_server()
{
    char buf[BUFFER_SIZE] = {0};
    int num_bytes = 0;

    if ((num_bytes = recv(this->clientSocket, buf, BUFFER_SIZE - 1, 0)) <= 0)
        exit(84);
    buf[num_bytes] = '\0';
    char* line = strtok(buf, "\n");
    while (line != nullptr) {
        line[strlen(line)] = '\n';
        draw_cmd(line);
        line = strtok(nullptr, "\n");
    }
}

std::string GUI::ask_server(std::string cmd)
{
    std::string response = "";
    char buffer[BUFFER_SIZE] = {0};
    memset(buffer, 0, BUFFER_SIZE);
    if (send(this->clientSocket, cmd.c_str(), cmd.length(), 0) == -1) {
        std::cerr << "Failed to send command to the server." << std::endl;
        close(this->clientSocket);
        return "error";
    }
    int bytesRead = recv(this->clientSocket, buffer, BUFFER_SIZE - 1, 0);
    if (bytesRead == -1) {
        std::cerr << "Failed to receive response from the server." << std::endl;
        close(this->clientSocket);
        return "error";
    }
    buffer[bytesRead] = '\0';
    response = buffer;
    return response;
}

int GUI::connectToServer()
{
    this->clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->clientSocket == -1) {
        std::cerr << "Failed to create socket." << std::endl;
        exit(84);
    }

    std::string serverIP = this->machine;
    if (this->machine == "localhost")
        serverIP = "127.0.0.1";
    int serverPort = this->port;

    struct sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(serverIP.c_str());
    serverAddress.sin_port = htons(serverPort);

    if (connect(this->clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Failed to connect to the server." << std::endl;
        close(this->clientSocket);
        exit(84);
    }

    const char *command = "GRAPHIC\n";
    if (send(this->clientSocket, command, strlen(command), 0) == -1) {
        std::cerr << "Failed to send command to the server." << std::endl;
        close(this->clientSocket);
        exit(84);
    }

    char buffer[BUFFER_SIZE] = {0};
    memset(buffer, 0, BUFFER_SIZE);
    if (recv(this->clientSocket, buffer, BUFFER_SIZE, 0) == -1) {
        std::cerr << "Failed to receive data from the server." << std::endl;
        close(this->clientSocket);
        exit(84);
    }
    char buffer1[BUFFER_SIZE] = {0};
    memset(buffer1, 0, BUFFER_SIZE);
    if (recv(this->clientSocket, buffer1, BUFFER_SIZE, 0) == -1) {
        std::cerr << "Failed to receive data from the server." << std::endl;
        close(this->clientSocket);
        exit(84);
    }

    int x = 0, y = 0, sgt = 0;
    char *line = strtok(buffer, "\n");
    line = strtok(buffer1, "\n");
    while (line != nullptr) {
        if (sscanf(line, "msz %d %d", &x, &y) == 2) {
        }
        if (sscanf(line, "sgt %d", &sgt) == 1) {
        }
        if (strncmp(line, "tna", 3) == 0) {
            std::string teamName = line + 4;
            this->tna.push_back(teamName);
        }
        line = strtok(nullptr, "\n");
    }
    this->width = x;
    this->height = y;
    this->sgt = sgt;
    return 0;
}
