/*
** EPITECH PROJECT, 2023
** init_client
** File description:
** init
*/

#include "server.h"
#include <stdlib.h>

#include <stdlib.h>
#include <time.h>

int generate_random_orienation(void)
{
    int num = (rand() % 4) + 1;
    return num;
}

void init_cliens_list_two(client_t *clients, int i,
    server_params_t *server_params)
{
    clients[i].deraumere = 0;
    clients[i].sibur = 0;
    clients[i].mendiane = 0;
    clients[i].phiras = 0;
    clients[i].thystame = 0;
    clients[i].is_connected = 0;
    clients[i].is_dead = 0;
    clients[i].team_max_clients = server_params->clients_per_team;
    clients[i].food_losing_timer = 0;
    clients[i].is_elevating = 0;
    clients[i].commands = NULL;
}

void init_clients_list(client_t *clients, server_params_t *server_params)
{
    for (int i = 0; i < MAX_CLIENTS; i++) {
        clients[i].socket = 0;
        clients[i].id = i;
        clients[i].start_loggin = 0;
        clients[i].is_graphical = 0;
        clients[i].team_name = NULL;
        clients[i].level = 1;
        clients[i].x_position =
            generate_rand_position(server_params->width - 2);
        clients[i].y_position =
            generate_rand_position(server_params->height - 2);
        if (clients[i].x_position == 0)
            clients[i].x_position = 1;
        if (clients[i].y_position == 0)
            clients[i].y_position = 1;
        clients[i].orientation = generate_random_orienation();
        clients[i].food = 9;
        clients[i].linemate = 0;
        init_cliens_list_two(clients, i, server_params);
    }
}

void handle_disconnect_two(client_t *client, server_params_t *server_params)
{
    client->x_position = generate_rand_position(server_params->width - 1);
    client->y_position = generate_rand_position(server_params->height - 1);
    if (client->x_position == 0)
        client->x_position = 1;
    if (client->y_position == 0)
        client->y_position = 1;
    client->sibur = 0;
    client->mendiane = 0;
    client->phiras = 0;
    client->thystame = 0;
    client->is_connected = 0;
    client->is_dead = 0;
    client->team_max_clients = server_params->clients_per_team;
    client->food_losing_timer = 0;
    client->is_elevating = 0;
    if (client->commands) {
        free_command(client->commands);
        client->commands = NULL;
    }
}

void handle_disconnect(client_t *client, client_t *clients,
    server_params_t *server_params)
{
    char *output_to_graphical = msprintf("pdi %d\n", client->id);
    send_message_to_graphical(clients, output_to_graphical);
    close(client->socket);
    client->socket = 0;
    client->start_loggin = 0;
    if (client->team_name)
        free(client->team_name);
    client->team_name = NULL;
    client->is_graphical = 0;
    client->level = 1;
    client->x_position = 0;
    client->y_position = 0;
    client->orientation = generate_random_orienation();
    client->food = 9;
    client->linemate = 0;
    client->deraumere = 0;
    handle_disconnect_two(client, server_params);
}
