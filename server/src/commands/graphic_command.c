/*
** EPITECH PROJECT, 2023
** handle_command
** File description:
** graphic_command
*/

#include "server.h"
#include <stdio.h>

char *get_tna(server_params_t *server_params, client_t *client)
{
    char *output = NULL;

    for (int i = 0; server_params->team_names[i]; i++) {
        char *temp = msprintf("tna %s\n", server_params->team_names[i]);
        output = concat_strings(output, temp);
        free(temp);
    }
    return output;
}

char *get_bct(server_params_t *server_params, client_t *client)
{
    char *output = NULL;

    for (int i = 0; i < server_params->height; i++)
        for (int j = 0; j < server_params->width; j++) {
            char *temp = msprintf("bct %d %d %d %d %d %d %d %d %d\n", j, i,
                    server_params->world[i][j].food,
                    server_params->world[i][j].linemate,
                    server_params->world[i][j].deraumere,
                    server_params->world[i][j].sibur,
                    server_params->world[i][j].mendiane,
                    server_params->world[i][j].phiras,
                    server_params->world[i][j].thystame);
            output = concat_strings(output, temp);
            free(temp);
        }
    return output;
}

void handle_graphic_command(client_t *client, server_params_t *server_params)
{
    char *output = NULL;
    char buffer_map_size[50];
    char buffer_sgt[50];
    sprintf(buffer_map_size, "msz %d %d\n", server_params->width,
        server_params->height);
    sprintf(buffer_sgt, "sgt %d\n", server_params->frequency);
    output = msprintf("%s%s%s%s", buffer_map_size, buffer_sgt,
        get_bct(server_params, client), get_tna(server_params, client));
    send_response(client->socket, output);
    free(output);
    return;
}
