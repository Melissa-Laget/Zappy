/*
** EPITECH PROJECT, 2023
** Zappy GUI
** File description:
** draw.cpp
*/

#include "gui.hpp"

int divide_only_once = 0;

void GUI::draw_bct(std::string cmd)
{
    std::string parsed_cmd = "Tile content : ";
    int x = 0, y = 0, food = 0, linemate = 0, deraumere = 0, sibur = 0, mendiane = 0, phiras = 0, thystame = 0;

    sscanf(cmd.c_str(), "bct %d %d %d %d %d %d %d %d %d\n",
        &x,
        &y,
        &food,
        &linemate,
        &deraumere,
        &sibur,
        &mendiane,
        &phiras,
        &thystame);
    parsed_cmd += std::to_string(x);
    parsed_cmd += " ";
    parsed_cmd += std::to_string(y);
    parsed_cmd += "\n";
    parsed_cmd += "Food : ";
    parsed_cmd += std::to_string(food);
    parsed_cmd += "\n";
    parsed_cmd += "Linemate : ";
    parsed_cmd += std::to_string(linemate);
    parsed_cmd += "\n";
    parsed_cmd += "Deraumere : ";
    parsed_cmd += std::to_string(deraumere);
    parsed_cmd += "\n";
    parsed_cmd += "Sibur : ";
    parsed_cmd += std::to_string(sibur);
    parsed_cmd += "\n";
    parsed_cmd += "Mendiane : ";
    parsed_cmd += std::to_string(mendiane);
    parsed_cmd += "\n";
    parsed_cmd += "Phiras : ";
    parsed_cmd += std::to_string(phiras);
    parsed_cmd += "\n";
    parsed_cmd += "Thystame : ";
    parsed_cmd += std::to_string(thystame);
    parsed_cmd += "\n";
    this->assets.text_bct.setString(parsed_cmd);
}

void GUI::draw_tna(std::string cmd)
{
    std::string parsed_cmd = "";
    std::vector<std::string> teamNames = {};
    std::string currentTeamName = "";

    for (std::string::size_type i = 4; i < cmd.size(); i++) {
        if (cmd[i] == '\n' || cmd[i] == '\0') {
            if (!currentTeamName.empty()) {
                teamNames.push_back(currentTeamName);
                currentTeamName.clear();
                i += 4;
            }
        } else
            currentTeamName += cmd[i];
    }
    for (std::vector<std::string>::size_type i = 0; i < teamNames.size(); i++) {
        parsed_cmd += teamNames[i];
        if (i < teamNames.size() - 1)
            parsed_cmd += "\n\n";
    }
    this->assets.tna_text.setString(parsed_cmd);
}

void GUI::draw_pnw(std::string cmd)
{
    int num_values_parsed = sscanf(cmd.c_str(), "pnw %d %d %d %d %d %s\n",
        &player_info.number,
        &player_info.x,
        &player_info.y,
        &player_info.orientation, 
        &player_info.level,
        player_info.team);
    if (num_values_parsed != 6)
        return;
    assets.create_player(player_info.x, player_info.y, player_info.number, player_info.orientation, player_info.team, this->tna);
}

void GUI::draw_pdi(std::string cmd)
{
    sscanf(cmd.c_str(), "pdi %d\n", &player_info.number);
    this->assets.delete_player(player_info.number);
}

void GUI::draw_enw(std::string cmd)
{
    int x = 0, y = 0, id = 0, tmp = 0;
    int num_values_parsed = sscanf(cmd.c_str(), "enw %d %d %d %d\n",
        &id,
        &tmp,
        &x,
        &y);
    if (num_values_parsed != 4)
        return;
    assets.create_egg(x, y, id);
}


void GUI::draw_edi(std::string cmd)
{
    int id = 0;
    int num_values_parsed = sscanf(cmd.c_str(), "edi %d\n", &id);
    if (num_values_parsed != 1)
        return;
    assets.delete_egg(id);
}

void GUI::draw_ppo(std::string cmd)
{
    int id = 0, x = 0, y = 0, o = 0;
    int num_values_parsed = sscanf(cmd.c_str(), "ppo %d %d %d %d\n",
        &id,
        &x,
        &y,
        &o);
    if (num_values_parsed != 4)
        return;

    auto newPosition = std::make_pair(x, y);
    for (auto& position : players_positions)
        position.second.erase(std::remove(position.second.begin(), position.second.end(), id), position.second.end());
    players_positions[newPosition].push_back(id);

    int player_count = players_positions[newPosition].size();
    if (player_count > 1 && divide_only_once == 0) {
        this->assets.monster_scale /= 2;
        divide_only_once = 1;
    }
    if (player_count == 1 && divide_only_once == 1) {
        this->assets.monster_scale *= 2;
        divide_only_once = 0;
    }
    int displacement = this->assets.box_size / 4;
    int index = 0;
    for (auto playerId : players_positions[newPosition]) {
        this->assets.monster_sprites[playerId].first.setScale(this->assets.monster_scale, this->assets.monster_scale);
        if (player_count > 1) {
            switch(index) {
                case 0:
                    this->assets.monster_sprites[playerId].first.setPosition((x * this->assets.box_size + this->assets.rectangle_width + this->assets.box_size / 2) - displacement, y * this->assets.box_size + this->assets.box_size / 2 - displacement);
                    break;
                case 1:
                    this->assets.monster_sprites[playerId].first.setPosition((x * this->assets.box_size + this->assets.rectangle_width + this->assets.box_size / 2) + displacement, y * this->assets.box_size + this->assets.box_size / 2 - displacement);
                    break;
                case 2:
                    this->assets.monster_sprites[playerId].first.setPosition((x * this->assets.box_size + this->assets.rectangle_width + this->assets.box_size / 2) - displacement, y * this->assets.box_size + this->assets.box_size / 2 + displacement);
                    break;
                case 3:
                    this->assets.monster_sprites[playerId].first.setPosition((x * this->assets.box_size + this->assets.rectangle_width + this->assets.box_size / 2) + displacement, y * this->assets.box_size + this->assets.box_size / 2 + displacement);
                    break;
                default:
                    break;
            }
            index++;
        } else {
            this->assets.monster_sprites[playerId].first.setPosition(x * this->assets.box_size + this->assets.rectangle_width + this->assets.box_size / 2, y * this->assets.box_size + this->assets.box_size / 2);
        }
    }
    if (o == 1)
        assets.monster_sprites[id].first.setRotation(0);
    else if (o == 2)
        assets.monster_sprites[id].first.setRotation(90);
    else if (o == 3)
        assets.monster_sprites[id].first.setRotation(180);
    else if (o == 4)
        assets.monster_sprites[id].first.setRotation(-90);
}

int GUI::count_players_on_tile(int x, int y)
{
    int count = 0;
    for (const auto& monster_sprite : assets.monster_sprites)
    {
        int monster_id = monster_sprite.first;
        int monster_x = 0, monster_y = 0, monster_o = 0;
        int num_values_parsed = sscanf(ask_server("ppo " + std::to_string(monster_id) + "\n").c_str(), "ppo %d %d %d %d\n",
            &monster_id,
            &monster_x,
            &monster_y,
            &monster_o);

        if (num_values_parsed == 4 && monster_x == x && monster_y == y)
            count++;
    }
    return count;
}


void GUI::draw_pin(std::string cmd)
{
    std::string parsed_cmd = "Inventory :\n";
    int n = 0, x = 0, y = 0, food = 0, linemate = 0, deraumere = 0, sibur = 0, mendiane = 0, phiras = 0, thystame = 0;

    sscanf(cmd.c_str(), "pin %d %d %d %d %d %d %d %d %d %d\n",
        &n,
        &x,
        &y,
        &food,
        &linemate,
        &deraumere,
        &sibur,
        &mendiane,
        &phiras,
        &thystame);
    parsed_cmd += "Food: ";
    parsed_cmd += std::to_string(food);
    parsed_cmd += "\n";
    parsed_cmd += "Linemate: ";
    parsed_cmd += std::to_string(linemate);
    parsed_cmd += "\n";
    parsed_cmd += "Deraumere: ";
    parsed_cmd += std::to_string(deraumere);
    parsed_cmd += "\n";
    parsed_cmd += "Sibur: ";
    parsed_cmd += std::to_string(sibur);
    parsed_cmd += "\n";
    parsed_cmd += "Mendiane: ";
    parsed_cmd += std::to_string(mendiane);
    parsed_cmd += "\n";
    parsed_cmd += "Phiras: ";
    parsed_cmd += std::to_string(phiras);
    parsed_cmd += "\n";
    parsed_cmd += "Thystame: ";
    parsed_cmd += std::to_string(thystame);
    parsed_cmd += "\n";
    this->assets.text_pin.setString(parsed_cmd);
}

void GUI::draw_pbc(std::string cmd)
{
    cmd.replace(0, 3, "Player");
    this->assets.chat_messages_string.push_back(cmd);
    this->assets.new_text.setString(cmd);
    this->assets.new_text.setPosition(10, 1080 / 3 + 60 + (this->assets.chat_messages_string.size() - 1) * 30);
    this->assets.chat_texts.push_back(this->assets.new_text);

    if (this->assets.chat_texts.size() > 5) {
        this->assets.chat_texts.erase(this->assets.chat_texts.begin());
        this->assets.chat_messages_string.erase(this->assets.chat_messages_string.begin());
        for (unsigned int i = 0; i < this->assets.chat_texts.size(); i++)
            this->assets.chat_texts[i].setPosition(10, 1080 / 3 + 60 + i * 30);
    }
}

void GUI::draw_pic(std::string cmd)
{
    std::istringstream iss(cmd);
    std::string prefix = "";
    int X = 0, Y = 0, L = 0, n = 0;
    std::vector<int> ns = {};

    if (iss >> prefix >> X >> Y >> L)
        while (iss >> n)
            ns.push_back(n);
    for (int n : ns) {
        if (this->assets.monster_sprites[n].second == this->tna[0])
            this->assets.monster_sprites[n].first.setTexture(this->assets.monster_red_evolving_texture);
        else if (this->assets.monster_sprites[n].second == this->tna[1])
            this->assets.monster_sprites[n].first.setTexture(this->assets.monster_blue_evolving_texture);
        else if (this->assets.monster_sprites[n].second == this->tna[2])
            this->assets.monster_sprites[n].first.setTexture(this->assets.monster_green_evolving_texture);
        else if (this->assets.monster_sprites[n].second == this->tna[3])
            this->assets.monster_sprites[n].first.setTexture(this->assets.monster_orange_evolving_texture);
        else if (this->assets.monster_sprites[n].second == this->tna[4])
            this->assets.monster_sprites[n].first.setTexture(this->assets.monster_pink_evolving_texture);
        else
            this->assets.monster_sprites[n].first.setTexture(this->assets.monster_red_evolving_texture);
        this->evolving.push_back(n);
    }
}

void GUI::draw_pie(std::string cmd)
{
    std::istringstream iss(cmd);
    std::string prefix = "";
    int X = 0, Y = 0, R = 0;

    if (iss >> prefix >> X >> Y >> R) {
        for (int n : this->evolving) {
            if (this->assets.monster_sprites[n].second == this->tna[0])
                this->assets.monster_sprites[n].first.setTexture(this->assets.monster_red_texture);
            else if (this->assets.monster_sprites[n].second == this->tna[1])
                this->assets.monster_sprites[n].first.setTexture(this->assets.monster_blue_texture);
            else if (this->assets.monster_sprites[n].second == this->tna[2])
                this->assets.monster_sprites[n].first.setTexture(this->assets.monster_green_texture);
            else if (this->assets.monster_sprites[n].second == this->tna[3])
                this->assets.monster_sprites[n].first.setTexture(this->assets.monster_orange_texture);
            else if (this->assets.monster_sprites[n].second == this->tna[4])
                this->assets.monster_sprites[n].first.setTexture(this->assets.monster_pink_texture);
            else
                this->assets.monster_sprites[n].first.setTexture(this->assets.monster_red_texture);
        }
        this->evolving.clear();
    }
}

void GUI::draw_game()
{
    window.draw(this->assets.leftrectangle1);
    window.draw(this->assets.leftrectangle2);
    window.draw(this->assets.leftrectangle3);
    window.draw(this->assets.closeButtonSprite);
    window.draw(this->assets.optionsButtonSprite);
    window.draw(this->assets.optionsPlusButtonSprite);
    window.draw(this->assets.optionsMinusButtonSprite);
    window.draw(this->assets.title_tna);
    window.draw(this->assets.title_chat);
    window.draw(this->assets.title_info);
    window.draw(this->assets.text_bct);
    window.draw(this->assets.text_pin);
    window.draw(this->assets.text_plv);
    window.draw(this->assets.text_pid);
    window.draw(this->assets.text_sgt);
    window.draw(this->assets.tna_text);
    for (auto& text : this->assets.chat_texts)
        window.draw(text);
    for (int i = 0; i < this->width; i++)
        for(int j = 0; j < this->height; j++)
            window.draw(this->assets.tiles[i][j]);
    for (auto &monster_sprite : assets.monster_sprites)
        window.draw(monster_sprite.second.first);
    for (auto &egg_sprite : assets.egg_sprites)
        window.draw(egg_sprite.second);
    draw_tna(this->tna_cmd);
}

void GUI::draw_cmd(std::string cmd)
{
    std::string cmd_tag = cmd.substr(0, 3);

    if (cmd_tag.compare("bct") == 0) {
        this->draw_bct(cmd);
        return;
    }
    if (cmd_tag.compare("tna") == 0) {
        this->draw_tna(cmd);
        return;
    }
    if (cmd_tag.compare("pnw") == 0) {
        this->draw_pnw(cmd);
        return;
    }
    if (cmd_tag.compare("ppo") == 0) {
        this->draw_ppo(cmd);
        return;
    }
    if (cmd_tag.compare("plv") == 0) {
        this->assets.text_plv.setString("Level : " + cmd.substr(6, 1));
        return;
    }
    if (cmd_tag.compare("pin") == 0) {
        this->draw_pin(cmd);
        return;
    }
    if (cmd_tag.compare("pex") == 0) {
        return;
    }
    if (cmd_tag.compare("pbc") == 0) {
        this->draw_pbc(cmd);
        return;
    }
    if (cmd_tag.compare("pic") == 0) {
        this->draw_pic(cmd);
        return;
    }
    if (cmd_tag.compare("pie") == 0) {
        this->draw_pie(cmd);
        return;
    }
    if (cmd_tag.compare("pfk") == 0) {
        return;
    }
    if (cmd_tag.compare("pdr") == 0) {
        return;
    }
    if (cmd_tag.compare("pgt") == 0) {
        return;
    }
    if (cmd_tag.compare("pdi") == 0) {
        this->draw_pdi(cmd);
        return;
    }
    if (cmd_tag.compare("enw") == 0) {
        this->draw_enw(cmd);
        return;
    }
    if (cmd_tag.compare("ebo") == 0) {
        return;
    }
    if (cmd_tag.compare("edi") == 0) {
        this->draw_edi(cmd);
        return;
    }
    if (cmd_tag.compare("sgt") == 0) {
        this->assets.text_sgt.setString(cmd.substr(4, cmd.size() - 4));
        return;
    }
    if (cmd_tag.compare("seg") == 0) {
        this->window.close();
        return;
    }
    if (cmd_tag.compare("smg") == 0) {
        return;
    }
    if (cmd_tag.compare("suc") == 0) {
        return;
    }
    if (cmd_tag.compare("sbp") == 0) {
        return;
    }
    return;
}
