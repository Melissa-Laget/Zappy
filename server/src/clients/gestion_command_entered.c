/*
** EPITECH PROJECT, 2023
** gestion client command entered
** File description:
** gestion_command_entered
*/

#include "server.h"
#include <time.h>

command_info_t commands_list[] = {
    {"Broadcast", 7.0},
    {"Forward", 7.0},
    {"Right", 7.0},
    {"Left", 7.0},
    {"Look", 7.0},
    {"Inventory", 1.0},
    {"Connect_nbr", 0.0},
    {"Fork", 42.0},
    {"Eject", 7.0},
    {"Take", 7.0},
    {"Set", 7.0},
    {"Incantation", 300.0},
    {"msz", 0.0},
    {"bct", 0.0},
    {"mct", 0.0},
    {"tna", 0.0},
    {"ppo", 0.0},
    {"plv", 0.0},
    {"pin", 0.0},
    {"sgt", 0.0},
    {"sst", 0.0},
    {NULL, 0.0},
};

long long get_current_time_in_milliseconds(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000LL + tv.tv_usec / 1000LL;
}

command_t *create_new_command(char **args, server_params_t *server_params)
{
    command_t *new_command = calloc(1, sizeof(command_t));
    if (!new_command || !args || !server_params)
        return NULL;
    new_command->name = strdup(args[0]);
    new_command->args = duplicate_args(args);
    long long ms = get_current_time_in_milliseconds();
    for (command_info_t *command = commands_list; command->name; ++command) {
        if (strcmp(new_command->name, command->name) == 0) {
            new_command->executed = 0;
            new_command->execution_time = ms + command->execution_time_factor
                * 1000LL / server_params->frequency;
            new_command->next = NULL;
            return new_command;
        }
    }
    free_command_args(new_command->args);
    if (new_command->name && strlen(new_command->name) > 0)
        free(new_command->name);
    return NULL;
}

void handle_incantation_command_gestion(client_t *clients, client_t *client,
    server_params_t *server_params, char **args)
{
    char *output_incantation = NULL;
    if (strcmp(args[0], "Incantation") == 0) {
        if (can_do_incantation(clients, client, server_params, args) == 0) {
            output_incantation = msprintf("ko\n");
            send_response(client->socket, output_incantation);
            free(output_incantation);
            free_array(args);
            return;
        }
        send_message_to_graphical_start_incantation(clients,
            client, server_params, args);
    }
}

void add_new_command(client_t *client, command_t *new_command, char **args)
{
    if (client->commands == NULL) {
        client->commands = new_command;
        if (client->commands == NULL) {
            free_array(args);
            return;
        }
    } else {
        command_t *tmp = client->commands;
        while (tmp->next != NULL)
            tmp = tmp->next;
        tmp->next = new_command;
        if (tmp->next == NULL) {
            free_array(args);
            return;
        }
    }
}

void handle_client_request(client_t *clients, char *buffer, client_t *client,
    server_params_t *server_params)
{
    if (!buffer || strlen(buffer) == 0)
        return;

    char **args = get_args_from_buffer(buffer);
    if (args == NULL)
        return;
    if (args[0] == NULL) {
        free_array(args);
        return;
    }
    handle_incantation_command_gestion(clients, client, server_params, args);
    command_t *new_command = create_new_command(args, server_params);
    add_new_command(client, new_command, args);
}
