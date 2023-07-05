/*
** EPITECH PROJECT, 2023
** server
** File description:
** incantation_command.c
*/

#include "server.h"

void send_elevation_underway(client_t *clients, client_t *client)
{
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (is_valid_user(&clients[i]) == 0)
            continue;
        if (clients[i].x_position == client->x_position
            && clients[i].y_position == client->y_position
            && clients[i].level == client->level)
            dprintf(clients[i].socket, "Elevation underway\n");
            clients[i].is_elevating = 1;
    }
}

int can_do_incantation(client_t *clients, client_t *client,
    server_params_t *server_params, char **args)
{
    int nb_player[7] = { 1, 2, 2, 4, 4, 6, 6 };
    int nb_player_on_tile = 0;

    nb_player_on_tile = get_nbr_on_tile(clients, client);

    if (client->level < 8) {
        if (nb_player_on_tile >= nb_player[client->level - 1]
            && check_stone(
                &server_params->world[client->y_position][client->x_position],
                client->level) == true) {
            send_elevation_underway(clients, client);
            return 1;
        }
    }
    return 0;
}

void send_message_to_graphical_start_incantation(client_t *cls,
    client_t *cl, server_params_t *sp, char **args)
{
    int nb_player[7] = { 1, 2, 2, 4, 4, 6, 6 };
    char msg[1024] = { 0 };
    sprintf(msg, "pic %d %d %d", cl->x_position, cl->y_position, cl->level);
    int nb_player_on_tile = 0;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (is_valid_user(&cls[i]) == 0)
            continue;
        if (compare_positions_and_level(&cls[i], cl) == 1) {
            char player_num[10];
            sprintf(player_num, " %d", cls[i].id);
            strcat(msg, player_num);
            nb_player_on_tile++;
        }
    }
    strcat(msg, "\n");
    if (cl->level < 8) {
        if (nb_player_on_tile >= nb_player[cl->level - 1] && check_stone(&sp
                ->world[cl->y_position][cl->x_position], cl->level) == true)
            send_message_to_graphical(cls, msg);
    }
}

void end_incantation(client_t *clients, client_t *client,
    int level_client_before, tile_t *tile)
{
    char *output_graphical = NULL;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (is_valid_user(&clients[i]) == 0)
            continue;
        if (clients[i].x_position == client->x_position
            && clients[i].y_position == client->y_position
            && clients[i].level == level_client_before) {
            clients[i].level++;
            dprintf(clients[i].socket,
                "Current level: %d\n", clients[i].level);
            output_graphical = msprintf("pie %d %d %d\n",
                clients[i].x_position,
                clients[i].y_position, clients[i].level);
            send_message_to_graphical(clients, output_graphical);
            clients[i].is_elevating = 0;
            free(output_graphical);
        }
    }
    remove_stones(tile, client->level);
}

void handle_incantation_command(client_t *clients,
    client_t *client, server_params_t *server_params, char **args)
{
    tile_t *tile =
        &server_params->world[client->y_position][client->x_position];
    int nb_player_on_tile = 0;
    int nb_player[7] = { 1, 2, 2, 4, 4, 6, 6 };
    char *output_graphical = NULL;
    if (strcmp(args[0], "Incantation") != 0)
        return;
    printf("handle_incantation_command\n");
    nb_player_on_tile = get_nbr_on_tile(clients, client);
    int level_client_before = client->level;
    if (level_client_before > 8 || nb_player_on_tile <
        nb_player[level_client_before - 1] ||
        check_stone(tile, level_client_before) == false)
        return;
    end_incantation(clients, client, level_client_before, tile);
}
