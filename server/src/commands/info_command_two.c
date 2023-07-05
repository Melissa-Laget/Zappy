/*
** EPITECH PROJECT, 2023
** projets
** File description:
** info_command
*/

#include "server.h"

void print_ppo(client_t *client, client_t *player)
{
    char *output = NULL;
    if (player == NULL) {
        send_response(client->socket, "sbp\n");
        return;
    }
    output = msprintf("ppo %d %d %d %d\n", player->id,
        player->x_position, player->y_position, player->orientation);
    if (!output) {
        printf("Error: Could not allocate memory for output string\n");
        return;
    }
    send_response(client->socket, output);
    if (output)
        free(output);
}

void print_plv(client_t *client, client_t *player)
{
    char *output = NULL;
    if (player == NULL) {
        send_response(client->socket, "ko\n");
        return;
    }
    output = msprintf("plv %d %d\n", player->id, player->level);
    if (!output) {
        printf("Error: Could not allocate memory for output string\n");
        return;
    }
    send_response(client->socket, output);
    if (output)
        free(output);
}

void print_pin(client_t *client, client_t *player)
{
    char *output = NULL;
    if (player == NULL) {
        send_response(client->socket, "ko\n");
        return;
    }
    output = msprintf("pin %d %d %d %d %d %d %d %d %d %d\n", player->id,
        player->x_position, player->y_position, player->food,
        player->linemate, player->deraumere, player->sibur,
        player->mendiane, player->phiras, player->thystame);
    if (!output) {
        printf("Error: Could not allocate memory for output string\n");
        return;
    }
    send_response(client->socket, output);
    if (output)
        free(output);
}

void handle_command_with_player_nbr(client_t *clients, client_t *client,
    server_params_t *server_params, char **args)
{
    if (!args[0] || !args[1])
        return;
    int player_id = atoi(args[1]);
    client_t *player = NULL;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].is_connected == 0 || clients[i].is_graphical == 1)
            continue;
        if (clients[i].id == player_id) {
            player = &clients[i];
            break;
        }
    }
    if (strcmp(args[0], "ppo") == 0)
        print_ppo(client, player);
    if (strcmp(args[0], "plv") == 0)
        print_plv(client, player);
    if (strcmp(args[0], "pin") == 0)
        print_pin(client, player);
}
