/*
** EPITECH PROJECT, 2023
** server
** File description:
** incantation_command.c
*/

#include "server.h"

bool check_stone(tile_t *tile, int level)
{
    int linemate[7] = { 1, 1, 2, 1, 1, 1, 1 };
    int deraumere[7] = { 0, 1, 0, 1, 2, 2, 2 };
    int sibur[7] = { 0, 1, 1, 2, 1, 3, 2 };
    int mendiane[7] = { 0, 0, 0, 0, 3, 0, 2 };
    int phiras[7] = { 0, 0, 2, 1, 0, 1, 2 };
    int thystame[7] = { 0, 0, 0, 0, 0, 0, 1 };

    if (level > 0 && level < 8 && tile->linemate < linemate[level - 1]
        || tile->deraumere < deraumere[level - 1]
        || tile->sibur < sibur[level - 1] || tile->phiras < phiras[level - 1]
        || tile->mendiane < mendiane[level - 1]
        || tile->thystame < thystame[level - 1])
        return false;
    return true;
}

void remove_stones(tile_t *tile, int level)
{
    int linemate[7] = { 1, 1, 2, 1, 1, 1, 1 };
    int deraumere[7] = { 0, 1, 0, 1, 2, 2, 2 };
    int sibur[7] = { 0, 1, 1, 2, 1, 3, 2 };
    int mendiane[7] = { 0, 0, 0, 0, 3, 0, 2 };
    int phiras[7] = { 0, 0, 2, 1, 0, 1, 2 };
    int thystame[7] = { 0, 0, 0, 0, 0, 0, 1 };

    tile->linemate -= linemate[level - 2];
    tile->deraumere -= deraumere[level - 2];
    tile->sibur -= sibur[level - 2];
    tile->mendiane -= mendiane[level - 2];
    tile->phiras -= phiras[level - 2];
    tile->thystame -= thystame[level - 2];
}

int is_valid_user(client_t *client)
{
    if ((client->is_connected == 1 && client->is_graphical == 0
        && client->is_dead == 0))
        return 1;
    return 0;
}

int get_nbr_on_tile(client_t *clients, client_t *client)
{
    int nb_player_on_tile = 0;

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (is_valid_user(&clients[i]) == 0)
            continue;
        if (clients[i].x_position == client->x_position
            && clients[i].y_position == client->y_position
            && clients[i].level == client->level)
            nb_player_on_tile += 1;
    }
    return nb_player_on_tile;
}
