/*
** EPITECH PROJECT, 2023
** projets
** File description:
** info_command
*/

#include "server.h"

void print_mct(server_params_t *server_params, client_t *client)
{
    char buffer_bct[200];

    for (int i = 0; i < server_params->height; i++) {
        for (int j = 0; j < server_params->width; j++) {
            sprintf(buffer_bct, "bct %d %d %d %d %d %d %d %d %d\n", j, i,
                    server_params->world[i][j].food,
                    server_params->world[i][j].linemate,
                    server_params->world[i][j].deraumere,
                    server_params->world[i][j].sibur,
                    server_params->world[i][j].mendiane,
                    server_params->world[i][j].phiras,
                    server_params->world[i][j].thystame);
            send_response(client->socket, buffer_bct);
        }
    }
}

void print_msz(server_params_t *server_params, client_t *client)
{
    char buffer_msz[50];

    sprintf(buffer_msz, "msz %d %d\n", server_params->width,
        server_params->height);
    send_response(client->socket, buffer_msz);
}

void print_tna(server_params_t *server_params, client_t *client)
{
    if (!server_params->team_names)
        return;
    dprintf(client->socket, get_tna(server_params, client));
}

void print_sgt(server_params_t *server_params, client_t *client)
{
    char buffer_sgt[50];

    sprintf(buffer_sgt, "sgt %d\n", server_params->frequency);
    send_response(client->socket, buffer_sgt);
}
