/*
** EPITECH PROJECT, 2023
** server
** File description:
** client_command.c
*/

#include "server.h"

int get_connect_nbr(client_t *clients, client_t *client)
{
    int count = 0;

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].is_connected == 0 || clients[i].is_graphical == 1)
            continue;
        if (clients[i].id == client->id)
            continue;
        if (client->team_name == NULL || clients[i].team_name == NULL)
            continue;
        if (strcmp(client->team_name, clients[i].team_name) == 0)
            count++;
    }
    return client->team_max_clients - count;
}

static void print_connect_nbr(server_params_t *server_params,
    client_t *client, client_t *clients)
{
    int count = 0;
    char *output = NULL;

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].is_connected == 0 || clients[i].is_graphical == 1)
            continue;
        if (strcmp(client->team_name, clients[i].team_name) == 0)
            count++;
    }
    output = msprintf("%d\n", client->team_max_clients - count);
    send_response(client->socket, output);
    free(output);
}

void handle_connect_nbr_command(client_t *clients,
    client_t *client, server_params_t *server_params, char **args)
{
    if (strcmp(args[0], "Connect_nbr") == 0)
        print_connect_nbr(server_params, client, clients);
}
