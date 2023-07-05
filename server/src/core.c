/*
** EPITECH PROJECT, 2023
** core_server
** File description:
** core
*/

#include "server.h"

void handle_sst(server_params_t *server_params, char **args, client_t *client)
{
    int new_freq = 0;
    if (!server_params || !args)
        return;
    if (!args[0])
        return;
    if (strcmp(args[0], "sst") == 0) {
        if (args[1] && atoi(args[1]) > 0) {
            new_freq = atoi(args[1]);
            server_params->frequency = new_freq >= 2 && new_freq <= 1000
                ? new_freq : server_params->frequency;
            print_sgt(server_params, client);
        }
    }
}

void handle_commands(client_t *clients, client_t *client,
    server_params_t *server_params, command_t cmd)
{
    if (!clients || !client || !server_params || !cmd.args)
        return;
    if (!cmd.args[0])
        return;
    handle_incantation_command(clients, client, server_params, cmd.args);
    if (client->is_elevating == 1)
        return;
    handle_command(client, server_params, cmd.args);
    handle_connect_nbr_command(clients, client, server_params, cmd.args);
    handle_command_with_player_nbr(clients, client, server_params, cmd.args);
    handle_broadcast_command(clients, client, cmd.args, server_params);
    handle_eject_command(clients, client, server_params, cmd.args);
    handle_fork_command(client, clients, server_params, cmd.args);
    handle_look_command(clients, client, server_params, cmd.args);
    handle_sst(server_params, cmd.args, client);
}

void execute_commands_if_ready(client_t *clients, client_t *client,
    server_params_t *server_params)
{
    if (!clients || !client || !server_params)
        return;

    struct timeval tv;
    gettimeofday(&tv, NULL);
    long long current_time_in_milliseconds = tv.tv_sec
        * 1000LL + tv.tv_usec / 1000LL;

    command_t *tmp = client->commands;
    if (tmp == NULL)
        return;
    while (tmp != NULL) {
        if (current_time_in_milliseconds >= tmp->execution_time
            && tmp->executed == 0) {
            handle_commands(clients, client, server_params, *tmp);
            tmp->executed = 1;
        }
        tmp = tmp->next;
    }
}

void check_lose_food(client_t *client, server_params_t *server_params)
{
    if (!client || !server_params)
        return;
    if (client->is_graphical == 1 || client->is_connected == 0
        || client->is_dead == 1)
        return;

    struct timeval tv;
    gettimeofday(&tv, NULL);
    long long current_time_in_milliseconds =
        tv.tv_sec * 1000LL + tv.tv_usec / 1000LL;

    if (current_time_in_milliseconds >= client->food_losing_timer) {
        client->food -= 1;
        client->food_losing_timer = current_time_in_milliseconds
            + 126000LL / server_params->frequency;
        printf("Client %d lost 1 food\n", client->id);
    }
}

void check_client_activity(client_t *clients,
    int server_socket, fd_set *readfds, server_params_t *server_params)
{
    int valread = 0;
    char buffer[BUFFER_SIZE] = {0};
    for (int i = 0; i < MAX_CLIENTS; i++) {
        check_events_server(&clients[i], clients, server_params, i);
        execute_commands_if_ready(clients, &clients[i], server_params);
        if (!FD_ISSET(clients[i].socket, readfds))
            continue;
        valread = read(clients[i].socket, buffer, BUFFER_SIZE - 1);
        if (valread <= 0) {
            handle_disconnect(&clients[i], clients, server_params);
            continue;
        }
        buffer[valread] = '\0';
        if (strcmp(buffer, "\n") == 0) {
            send_response(clients[i].socket, "ko\n");
            continue;
        }
        handle_client_request(clients, buffer, &clients[i], server_params);
    }
}
