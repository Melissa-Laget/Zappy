/*
** EPITECH PROJECT, 2023
** Zappy GUI
** File description:
** gui.cpp
*/

#include "gui.hpp"

GUI::GUI(int port, std::string machine)
{
    this->port = port;
    this->machine = machine;
}

GUI::~GUI()
{
}

void GUI::init_game()
{
    this->window.create(sf::VideoMode(1920, 1080), "Trantor", sf::Style::Fullscreen);
    this->window.setFramerateLimit(60);
    this->assets = Assets();
    this->assets.init_assets(this->height, this->width);
    this->assets.text_sgt.setString(std::to_string(this->sgt));

    this->tna_cmd = ask_server("tna\n");
}

void GUI::check_event()
{
    std::string cmd = "";
    std::string server_response = "";
    std::string player_level = "";
    std::string player_inventory = "";

    if (this->event.type == sf::Event::Closed)
        window.close();
    if (this->event.type == sf::Event::MouseButtonReleased) {
        if (this->event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            if (this->assets.closeButtonSprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                window.close();
            }
            if (this->assets.optionsPlusButtonSprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                if (this->sgt == 2)
                    this->sgt = 10;
                else
                    this->sgt += 10;
                server_response = ask_server("sst " + std::to_string(this->sgt) + "\n");
                draw_cmd(server_response);
            }
            if (this->assets.optionsMinusButtonSprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                this->sgt -= 10;
                if (this->sgt <= 1)
                    this->sgt = 2;
                server_response = ask_server("sst " + std::to_string(this->sgt) + "\n");
                draw_cmd(server_response);
            }
            for (auto &monster_sprite : assets.monster_sprites) {
                if (monster_sprite.second.first.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    this->assets.text_pid.setString("Player : " + std::to_string(monster_sprite.first));
                    player_level = ask_server("plv " + std::to_string(monster_sprite.first) + "\n");
                    draw_cmd(player_level);
                    player_inventory = ask_server("pin " + std::to_string(monster_sprite.first) + "\n");
                    draw_cmd(player_inventory);
                }
            }
            for (int i = 0; i < this->width; i++) {
                for(int j = 0; j < this->height; j++) {
                    if (this->assets.tiles[i][j].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        cmd = "bct " + std::to_string(i) + " " + std::to_string(j) + "\n";
                        server_response = ask_server(cmd);
                        draw_cmd(server_response);
                    }
                }
            }
        }
    }
}

void GUI::game_loop()
{
    fd_set read_fds;
    struct timeval tv = {
        .tv_sec = 0,
        .tv_usec = 100000
    };
    std::string player_position = "";

    while (window.isOpen()) {
        if (this->assets.clock.getElapsedTime().asMilliseconds() > 100) {
            this->assets.clock.restart();
            while (window.pollEvent(this->event)) {
                check_event();
            }

            for (auto &monster_sprite : assets.monster_sprites) {
                player_position = ask_server("ppo " + std::to_string(monster_sprite.first) + "\n");
                draw_cmd(player_position);
            }
            window.clear();
            this->setup_fd_set(&read_fds, this->clientSocket);
            if (select(this->clientSocket + 1, &read_fds, NULL, NULL, &tv) == -1) {
                std::cout << "Error: select" << std::endl;
                exit(84);
            }
            if (FD_ISSET(this->clientSocket, &read_fds))
                handle_read_server();
            draw_game();
            window.display();
        }
    }
}
