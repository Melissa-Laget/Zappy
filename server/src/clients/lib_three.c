/*
** EPITECH PROJECT, 2023
** lib_three
** File description:
** lib_three
*/

#include "server.h"

int generate_rand_position(int upper_limit)
{
    return rand() % upper_limit;
}

void free_command_args(char **args)
{
    for (int i = 0; args[i] != NULL; i++)
        free(args[i]);
    free(args);
}

int count_args(char **args)
{
    if (args == NULL) {
        printf("args is NULL. Exiting count_args\n");
        return -1;
    }
    int arg_count = 0;
    while (args[arg_count] != NULL)
        arg_count++;
    return arg_count;
}

char **duplicate_args(char **args)
{
    int arg_count = count_args(args);
    if (arg_count == -1)
        return NULL;
    char **new_args = calloc(arg_count + 1, sizeof(char *));
    if (new_args == NULL)
        return NULL;
    int i = 0;
    for (i = 0; i < arg_count; i++) {
        new_args[i] = msprintf("%s", args[i]);
        if (new_args[i] == NULL)
            break;
    }
    if (i != arg_count) {
        for (int j = 0; j < i; j++)
            free(new_args[j]);
        free(new_args);
        return NULL;
    }
    new_args[arg_count] = NULL;
    return new_args;
}

int length_of_int(int *array)
{
    int len = 0;
    while (array && array[len] != 0)
        len++;

    return len;
}
