/*
** EPITECH PROJECT, 2023
** lib_two_functions
** File description:
** lib_two
*/

#include "server.h"

void free_command(command_t *command)
{
    if (command) {
        if (command->name) {
            free(command->name);
            command->name = NULL;
        }
        if (command->args) {
            free_command_args(command->args);
            command->args = NULL;
        }
    }
    if (command->next) {
        free_command(command->next);
        command->next = NULL;
    } else
        free(command);
}

void free_command_executed(command_t *command)
{
    if (command) {
        if (command->name) {
            free(command->name);
            command->name = NULL;
        }
        if (command->args) {
            free_command_args(command->args);
            command->args = NULL;
        }
    }
}

int find_empty_slot(client_t *clients)
{
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].socket == 0)
            return i;
    }
    return -1;
}

int build_message_incantation(client_t *clients,
    client_t *client, char *message)
{
    int nb_player[7] = { 1, 2, 2, 4, 4, 6, 6 };
    sprintf(message, "pic %d %d %d", client->x_position,
        client->y_position, client->level);
    int nb_player_on_tile = 0;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (is_valid_user(&clients[i]) == 0)
            continue;
        if (clients[i].x_position == client->x_position
            && clients[i].y_position == client->y_position
            && clients[i].level == client->level) {
            char player_num[10];
            sprintf(player_num, " %d", clients[i].id);
            strcat(message, player_num);
            nb_player_on_tile++;
        }
    }
    strcat(message, "\n");
}

void setup_readfds(int server_socket, fd_set *readfds)
{
    FD_ZERO(readfds);
    FD_SET(server_socket, readfds);
}
