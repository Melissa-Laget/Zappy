/*
** EPITECH PROJECT, 2023
** distribute_ressource_one
** File description:
** distribute_ressource_one
*/

#include "server.h"

void distribute_food(server_params_t *params)
{
    DistributionParams foodParams = {
        .params = params,
        .total_resource = params->width * params->height * 0.5,
        .resource = 'f'
    };
    distribute_resources(foodParams.params,
        foodParams.total_resource, foodParams.resource);
}

void distribute_linemate(server_params_t *params)
{
    DistributionParams linemateParams = {
        .params = params,
        .total_resource = params->width * params->height * 0.3,
        .resource = 'l'
    };
    distribute_resources(linemateParams.params, linemateParams.total_resource,
        linemateParams.resource);
}

void distribute_deraumere(server_params_t *params)
{
    DistributionParams deraumereParams = {
        .params = params,
        .total_resource = params->width * params->height * 0.15,
        .resource = 'd'
    };
    distribute_resources(deraumereParams.params,
        deraumereParams.total_resource,
        deraumereParams.resource);
}

void distribute_sibur(server_params_t *params)
{
    DistributionParams siburParams = {
        .params = params,
        .total_resource = params->width * params->height * 0.1,
        .resource = 's'
    };
    distribute_resources(siburParams.params,
        siburParams.total_resource,
        siburParams.resource);
}

void distribute_mendiane(server_params_t *params)
{
    DistributionParams mendianeParams = {
        .params = params,
        .total_resource = params->width * params->height * 0.1,
        .resource = 'm'
    };
    distribute_resources(mendianeParams.params,
        mendianeParams.total_resource,
        mendianeParams.resource);
}
