/*
** EPITECH PROJECT, 2023
** distribute_ressource_one
** File description:
** distribute_ressource_one
*/

#include "server.h"

void distribute_phiras(server_params_t *params)
{
    DistributionParams phirasParams = {
        .params = params,
        .total_resource = params->width * params->height * 0.08,
        .resource = 'p'
    };
    distribute_resources(phirasParams.params, phirasParams.total_resource,
        phirasParams.resource);
}

void distribute_thystame(server_params_t *params)
{
    DistributionParams thystameParams = {
        .params = params,
        .total_resource = params->width * params->height * 0.05,
        .resource = 't'
    };
    distribute_resources(thystameParams.params, thystameParams.total_resource,
        thystameParams.resource);
}

int get_random_coordinate(int limit)
{
    return rand() % limit;
}
