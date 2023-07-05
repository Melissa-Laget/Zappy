/*
** EPITECH PROJECT, 2023
** B-YEP-400-BDX-4-1-zappy-max.peixoto
** File description:
** check_win_event.c
*/

#include "server.h"

void count_nb_player(client_t *client, server_params_t *server_params,
    int team_count, int *team)
{
    for (int j = 0; j < team_count; j++)
        if (strcmp(client->team_name, server_params->team_names[j]) == 0)
            team[j] += 1;
}

void check_win_event(client_t client, client_t *clients,
    server_params_t *server_params)
{
    int team_count = 0;
    int *team = NULL;
    char *output_to_graphical = NULL;
    for (; server_params->team_names[team_count] != NULL; team_count++);
    team = calloc(team_count, sizeof(int));
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].is_graphical == 1 || clients[i].is_connected == 0
            || clients[i].is_dead == 1)
            continue;
        if (clients[i].level == 8)
            count_nb_player(&clients[i], server_params, team_count, team);
    }
    for (int i = 0; i < team_count; i++)
        if (team[i] >= 6) {
            output_to_graphical = msprintf("seg %s\n",
                server_params->team_names[i]);
            send_message_to_graphical(clients, output_to_graphical);
            return;
        }
}
