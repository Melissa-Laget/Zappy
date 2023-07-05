/*
** EPITECH PROJECT, 2023
** Zappy GUI
** File description:
** assets.cpp
*/

#include "assets.hpp"

Assets::Assets()
{
}

void Assets::init_assets(int height, int width)
{
    this->height = height;
    this->width = width;
    this->box_size = 1080 / height;
    this->width_checkerboard = box_size * width;
    this->rectangle_width = 1920 - width_checkerboard;

    if (width == 10)
        this->monster_scale = 0.5;
    else if (width == 15)
        this->monster_scale = 0.33;
    else if (width == 20)
        this->monster_scale = 0.25;
    this->font.loadFromFile("assets/Bronten.ttf");
    this->title_tna.setFont(this->font);
    this->title_tna.setString("Teams name: ");
    this->title_tna.setCharacterSize(40);
    this->title_tna.setFillColor(sf::Color::Black);
    this->title_tna.setPosition(310, 10);
    this->title_chat.setFont(this->font);
    this->title_chat.setString("Chat :");
    this->title_chat.setCharacterSize(40);
    this->title_chat.setFillColor(sf::Color::Black);
    this->title_chat.setPosition(350, 1080 / 3 + 10);
    this->title_info.setFont(this->font);
    this->title_info.setString("Info :");
    this->title_info.setCharacterSize(40);
    this->title_info.setFillColor(sf::Color::Black);
    this->title_info.setPosition(350, 2 * 1080 / 3);
    this->text_bct.setFont(this->font);
    this->text_bct.setString("Tile content :\nFood : 0\nLinemate : 0\nDeraumere : 0\nSibur : 0\nMendiane : 0\nPhiras : 0\nThystame : 0");
    this->text_bct.setCharacterSize(30);
    this->text_bct.setFillColor(sf::Color::Black);
    this->text_bct.setPosition(10, 2 * 1080 / 3 + 50);
    this->text_pin.setFont(this->font);
    this->text_pin.setString("Inventory :\nFood : 0\nLinemate : 0\nDeraumere : 0\nSibur : 0\nMendiane : 0\nPhiras : 0\nThystame : 0");
    this->text_pin.setCharacterSize(30);
    this->text_pin.setFillColor(sf::Color::Black);
    this->text_pin.setPosition(600, 2 * 1080 / 3 + 50);
    this->text_plv.setFont(this->font);
    this->text_plv.setString("Level : N");
    this->text_plv.setCharacterSize(30);
    this->text_plv.setFillColor(sf::Color::Black);
    this->text_plv.setPosition(350, 2 * 1080 / 3 + 90);
    this->text_pid.setFont(this->font);
    this->text_pid.setString("Player : N");
    this->text_pid.setCharacterSize(30);
    this->text_pid.setFillColor(sf::Color::Black);
    this->text_pid.setPosition(350, 2 * 1080 / 3 + 50);
    this->text_sgt.setFont(this->font);
    this->text_sgt.setString("");
    this->text_sgt.setCharacterSize(30);
    this->text_sgt.setFillColor(sf::Color::Black);
    this->text_sgt.setPosition(160, 60);
    this->tna_text.setFont(this->font);
    this->tna_text.setString("");
    this->tna_text.setCharacterSize(30);
    this->tna_text.setFillColor(sf::Color::Black);
    this->tna_text.setPosition(10, 100);
    new_text.setFont(this->font);
    new_text.setCharacterSize(30);
    new_text.setFillColor(sf::Color::Black);
    this->chat_messages_string = {};
    this->chat_texts = {};
    this->closeButtonTexture.loadFromFile("assets/quit-icon.png");
    this->closeButtonSprite.setTexture(this->closeButtonTexture);
    this->closeButtonSprite.setPosition(10, 10);
    this->optionsButtonTexture.loadFromFile("assets/hourglass.png");
    this->optionsButtonSprite.setTexture(this->optionsButtonTexture);
    this->optionsButtonSprite.setPosition(150, 10);
    this->optionsButtonSprite.setScale(sf::Vector2f(0.55, 0.55));
    this->optionsPlusButtonTexture.loadFromFile("assets/plus.png");
    this->optionsPlusButtonSprite.setTexture(this->optionsPlusButtonTexture);
    this->optionsPlusButtonSprite.setPosition(200, 10);
    this->optionsPlusButtonSprite.setScale(sf::Vector2f(0.5, 0.5));
    this->optionsMinusButtonTexture.loadFromFile("assets/minus.png");
    this->optionsMinusButtonSprite.setTexture(this->optionsMinusButtonTexture);
    this->optionsMinusButtonSprite.setPosition(90, 10);
    this->optionsMinusButtonSprite.setScale(sf::Vector2f(0.5, 0.5));
    this->leftrectangle1 = sf::RectangleShape(sf::Vector2f(this->rectangle_width, 1080 / 3));
    this->leftrectangle1.setPosition(0, 0);
    this->leftrectangle1.setFillColor(sf::Color::Blue);
    this->leftrectangle2 = sf::RectangleShape(sf::Vector2f(this->rectangle_width, 1080 / 3));
    this->leftrectangle2.setPosition(0, 1080 / 3);
    this->leftrectangle2.setFillColor(sf::Color::Red);
    this->leftrectangle3 = sf::RectangleShape(sf::Vector2f(this->rectangle_width, 1080 / 3));
    this->leftrectangle3.setPosition(0, 2 * 1080 / 3);
    this->leftrectangle3.setFillColor(sf::Color::Green);
    this->tileTexture.loadFromFile("assets/tile.png");
    for (int i = 0; i < width; i++) {
        std::vector<sf::Sprite> tmp;
        for(int j = 0; j < height; j++)
            tmp.push_back(sf::Sprite());
        this->tiles.push_back(tmp);
    }
    for (int i = 0; i < width; i++) {
        for(int j = 0; j < height; j++) {
            this->tiles[i][j].setTexture(this->tileTexture);
            float scalingFactor = std::min(static_cast<float>(this->box_size) / this->tileTexture.getSize().x,
                                           static_cast<float>(this->box_size) / this->tileTexture.getSize().y);
            this->tiles[i][j].setScale(sf::Vector2f(scalingFactor, scalingFactor));
            this->tiles[i][j].setPosition(i * this->box_size + this->rectangle_width, j * this->box_size);
        }
    }
    this->monster_red_texture.loadFromFile("assets/monster.png");
    this->monster_red_evolving_texture.loadFromFile("assets/monster_evolving.png");
    this->monster_blue_texture.loadFromFile("assets/monster_blue.png");
    this->monster_blue_evolving_texture.loadFromFile("assets/monster_blue_evolving.png");
    this->monster_green_texture.loadFromFile("assets/monster_green.png");
    this->monster_green_evolving_texture.loadFromFile("assets/monster_green_evolving.png");
    this->monster_orange_texture.loadFromFile("assets/monster_orange.png");
    this->monster_orange_evolving_texture.loadFromFile("assets/monster_orange_evolving.png");
    this->monster_pink_texture.loadFromFile("assets/monster_pink.png");
    this->monster_pink_evolving_texture.loadFromFile("assets/monster_pink_evolving.png");
    this->monster_sprite.setTexture(monster_red_texture);
    this->monster_sprite.setOrigin(sf::Vector2f(this->monster_red_texture.getSize().x / 2, this->monster_red_texture.getSize().y / 2));
    this->egg_texture.loadFromFile("assets/egg.png");
    this->egg_sprite.setTexture(egg_texture);
    this->egg_sprite.setOrigin(sf::Vector2f(this->egg_texture.getSize().x / 2, this->egg_texture.getSize().y / 2));
    this->clock = sf::Clock();
}

void Assets::create_player(int x, int y, int n, int o, std::string team, std::vector<std::string> teams)
{
    monster_sprite.setScale(sf::Vector2f(this->monster_scale, this->monster_scale));
    if (o == 1)
        monster_sprite.setRotation(0);
    else if (o == 2)
        monster_sprite.setRotation(90);
    else if (o == 3)
        monster_sprite.setRotation(180);
    else if (o == 4)
        monster_sprite.setRotation(-90);
    if (team == teams[0])
        monster_sprite.setTexture(monster_red_texture);
    else if (team == teams[1])
        monster_sprite.setTexture(monster_blue_texture);
    else if (team == teams[2])
        monster_sprite.setTexture(monster_green_texture);
    else if (team == teams[3])
        monster_sprite.setTexture(monster_orange_texture);
    else if (team == teams[4])
        monster_sprite.setTexture(monster_pink_texture);
    else
        monster_sprite.setTexture(monster_red_texture);
    monster_sprite.setPosition(x * this->box_size + this->rectangle_width + this->box_size / 2, y * this->box_size + this->box_size / 2);
    this->monster_sprites.insert(std::pair<int, std::pair<sf::Sprite, std::string>>(n, std::pair<sf::Sprite, std::string>(monster_sprite, team)));
}

void Assets::delete_player(int n)
{
    this->monster_sprites.erase(n);
}

void Assets::create_egg(int x, int y, int e)
{
    egg_sprite.setPosition(x * this->box_size + this->rectangle_width + (this->box_size / 2), y * this->box_size + (this->box_size / 2));
    egg_sprite.setScale(sf::Vector2f(0.1, 0.1));
    this->egg_sprites.insert(std::pair<int, sf::Sprite>(e, egg_sprite));
    this->egg_sprites[e].setScale(sf::Vector2f(0.1, 0.1));
}

void Assets::delete_egg(int e)
{
    this->egg_sprites.erase(e);
}

Assets::~Assets()
{
    this->monster_sprites.clear();
    this->egg_sprites.clear();
    for(auto &tile: this->tiles)
        tile.clear();
    this->tiles.clear();
    this->chat_messages_string.clear();
    this->chat_texts.clear();
}
