/*
** EPITECH PROJECT, 2023
** lib_three
** File description:
** lib_four
*/

#include "server.h"

long long init_food_losing_timer(server_params_t *server_params)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long long current_time_in_milliseconds =
        tv.tv_sec * 1000LL + tv.tv_usec / 1000LL;

    return current_time_in_milliseconds +
        126000LL / server_params->frequency;
}

int calc_distance(int x1, int y1, int x2, int y2)
{
    int dist = abs(x2 - x1) + abs(y2 - y1);
    return dist;
}

void spawn_food(server_params_t *server_params)
{
    if (!server_params)
        return;

    struct timeval tv;
    gettimeofday(&tv, NULL);
    long long current_time_in_milliseconds =
        tv.tv_sec * 1000LL + tv.tv_usec / 1000LL;
    if (current_time_in_milliseconds
        >= server_params->food_spawning_timer) {
        srand(current_time_in_milliseconds);
        distribute_food(server_params);
        distribute_linemate(server_params);
        distribute_deraumere(server_params);
        distribute_sibur(server_params);
        distribute_mendiane(server_params);
        distribute_phiras(server_params);
        distribute_thystame(server_params);
        server_params->food_spawning_timer = current_time_in_milliseconds
            + 20000LL / server_params->frequency;
    }
}

void check_events_server(client_t *client, client_t *clients,
    server_params_t *server_params, int i)
{
    check_lose_food(&clients[i], server_params);
    spawn_food(server_params);
    check_death_player(clients, &clients[i], server_params);
    check_win_event(clients[i], clients, server_params);
}

int compare_positions_and_level(client_t *cl1, client_t *cl2)
{
    if (cl1->x_position == cl2->x_position &&
        cl1->y_position == cl2->y_position &&
        cl1->level == cl2->level) {
        return 1;
    }
    return 0;
}
