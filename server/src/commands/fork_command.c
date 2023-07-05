/*
** EPITECH PROJECT, 2023
** server
** File description:
** fork_command.c
*/

#include "server.h"

void update_client_per_team_after_fork(client_t *client, client_t *clients)
{
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].is_connected == 0 || clients[i].is_graphical == 1)
            continue;
        if (strcmp(client->team_name, clients[i].team_name) == 0) {
            client->team_max_clients++;
            return;
        }
    }
}

char *create_command_graphical(int egg_id, int client_id,
    int x_position, int y_position)
{
    char *output = malloc(sizeof(char) * 100);
    if (!output)
        exit(EXIT_FAILURE);
    msprintf(output, "enw %d %d %d %d\n", egg_id, client_id,
        x_position, y_position);
    return (output);
}

char *create_message_graphical(int egg_id, int client_id,
    int x_position, int y_position)
{
    char *output = NULL;

    msprintf("enw %d %d %d %d\n", egg_id, client_id,
        x_position, y_position);
    return (output);
}

void handle_fork_command(client_t *client,
    client_t *clients, server_params_t *server_params, char **args)
{
    tile_t *tile =
        &server_params->world[client->y_position][client->x_position];
    char *output_graphical = NULL;
    if (strcmp(args[0], "Fork") != 0)
        return;
    update_client_per_team_after_fork(client, clients);
    int length = 0;
    if (tile->eggs)
        for (; tile->eggs[length] != 0; ++length);
    tile->eggs = realloc(tile->eggs, (length + 2) * sizeof(int));
    if (!tile->eggs)
        exit(EXIT_FAILURE);
    int previous_value = (length > 0) ? tile->eggs[length - 1] : 0;
    tile->eggs[length] = previous_value + 1;
    tile->eggs[length + 1] = 0;
    output_graphical = create_message_graphical(tile->eggs[length],
            client->id, client->x_position, client->y_position);
    send_message_to_graphical(clients, output_graphical);
    free(output_graphical);
    send_response(client->socket, "ok\n");
}
