/*
** EPITECH PROJECT, 2023
** bct_command
** File description:
** bct_command
*/

#include "server.h"

bool check_null_args(char **args, int socket)
{
    if (args[1] == NULL || args[2] == NULL) {
        send_response(socket, "sbp\n");
        return true;
    }
    return false;
}

long convert_strtol(char *arg, int socket)
{
    char *end;
    long val = strtol(arg, &end, 10);
    if (end == arg || *end != '\0') {
        send_response(socket, "sbp\n");
        return -1;
    }
    return val;
}

bool check_bounds(long x, long y, server_params_t *server_params, int socket)
{
    if (x < 0 || x >= server_params->width
        || y < 0 || y >= server_params->height) {
        send_response(socket, "sbp\n");
        return true;
    }
    return false;
}

char *build_message_bct(server_params_t *server_params, long x, long y)
{
    return msprintf(
        "bct %ld %ld %d %d %d %d %d %d %d\n",
        x, y,
        server_params->world[y][x].food,
        server_params->world[y][x].linemate,
        server_params->world[y][x].deraumere,
        server_params->world[y][x].sibur,
        server_params->world[y][x].mendiane,
        server_params->world[y][x].phiras,
        server_params->world[y][x].thystame
    );
}

void print_bct(server_params_t *server_params, client_t *client, char **args)
{
    char *buffer_bct = NULL;

    if (check_null_args(args, client->socket))
        return;
    long x = convert_strtol(args[1], client->socket);
    if (x == -1)
        return;
    long y = convert_strtol(args[2], client->socket);
    if (y == -1)
        return;
    if (check_bounds(x, y, server_params, client->socket))
        return;
    buffer_bct = build_message_bct(server_params, x, y);
    if (buffer_bct) {
        send_response(client->socket, buffer_bct);
        free(buffer_bct);
    }
}
