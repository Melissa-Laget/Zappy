/*
** EPITECH PROJECT, 2023
** handle_command
** File description:
** handle_command
*/

#include "server.h"

void handle_command_five(client_t *client,
    server_params_t *server_params, char **args)
{
    if (strcmp(args[0], "Take") == 0) {
        take_command(server_params, client, args);
        return;
    }
    if (strcmp(args[0], "Set") == 0) {
        set_command(server_params, client, args);
        return;
    }
}

void handle_command_four(client_t *client,
    server_params_t *server_params, char **args)
{
    if (strcmp(args[0], "bct") == 0) {
        print_bct(server_params, client, args);
        return;
    }
    if (strcmp(args[0], "Forward") == 0) {
        handle_forward_command(server_params, client, args);
        return;
    }
    if (strcmp(args[0], "Right") == 0) {
        handle_right_command(client, args);
    }
    if (strcmp(args[0], "Left") == 0) {
        handle_left_command(client, args);
        return;
    }
    handle_command_five(client, server_params, args);
}

void handle_command_three(client_t *client,
    server_params_t *server_params, char **args)
{
    if (strcmp(args[0], "sgt") == 0) {
        print_sgt(server_params, client);
        return;
    }
    if (strcmp(args[0], "mct") == 0) {
        print_mct(server_params, client);
        return;
    }
    if (strcmp(args[0], "msz") == 0) {
        print_msz(server_params, client);
        return;
    }
    if (strcmp(args[0], "tna") == 0) {
        print_tna(server_params, client);
        return;
    }
    handle_command_four(client, server_params, args);
}

void handle_command_two(client_t *client,
    server_params_t *server_params, char **args)
{
    if (strcmp(args[0], "Inventory") == 0) {
        print_inventory(server_params, client);
        return;
    }
    if (strcmp(args[0], "GRAPHIC") == 0) {
        handle_graphic_command(client, server_params);
        return;
    }
    handle_command_three(client, server_params, args);
}

void handle_command(client_t *client,
    server_params_t *server_params, char **args)
{
    int client_socket = client->socket;

    if (!args || !args[0]) {
        send_response(client_socket, "ko\n");
        return;
    }
    handle_command_two(client, server_params, args);
}
