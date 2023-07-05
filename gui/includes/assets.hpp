/*
** EPITECH PROJECT, 2023
** Zappy GUI
** File description:
** GUI.hpp
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <map>

class Assets {
    public:
        Assets();
        ~Assets();
        void init_assets(int height, int width);
        int height;
        int width;
        int box_size;
        int width_checkerboard;
        int rectangle_width;
        std::map<int, std::pair<sf::Sprite, std::string>> monster_sprites;
        void create_player(int x, int y, int n, int o, std::string team, std::vector<std::string> teams);
        void delete_player(int n);
        void create_egg(int x, int y, int e);
        void delete_egg(int e);
        sf::Texture monster_red_texture;
        sf::Texture monster_blue_texture;
        sf::Texture monster_orange_texture;
        sf::Texture monster_pink_texture;
        sf::Texture monster_green_texture;
        sf::Sprite monster_sprite;
        float monster_scale;
        sf::Texture monster_red_evolving_texture;
        sf::Texture monster_blue_evolving_texture;
        sf::Texture monster_orange_evolving_texture;
        sf::Texture monster_pink_evolving_texture;
        sf::Texture monster_green_evolving_texture;
        sf::Texture egg_texture;
        sf::Sprite egg_sprite;
        std::map<int, sf::Sprite> egg_sprites;
        sf::Font font;
        sf::Text title_tna;
        sf::Text title_chat;
        sf::Text title_info;
        sf::Text text_bct;
        sf::Text text_pin;
        sf::Text text_plv;
        sf::Text text_pid;
        sf::Text tna_text;
        sf::Text text_sgt;
        std::vector<sf::Text> chat_texts;
        sf::Text new_text;
        std::vector<std::string> chat_messages_string;
        sf::Texture closeButtonTexture;
        sf::Sprite closeButtonSprite;
        sf::Texture optionsButtonTexture;
        sf::Sprite optionsButtonSprite;
        sf::Texture optionsPlusButtonTexture;
        sf::Sprite optionsPlusButtonSprite;
        sf::Texture optionsMinusButtonTexture;
        sf::Sprite optionsMinusButtonSprite;
        sf::RectangleShape leftrectangle1;
        sf::RectangleShape leftrectangle2;
        sf::RectangleShape leftrectangle3;
        sf::Texture tileTexture;
        std::vector<std::vector<sf::Sprite>> tiles;
        sf::Clock clock;
};
