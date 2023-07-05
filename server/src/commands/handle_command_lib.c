/*
** EPITECH PROJECT, 2023
** lib_handle_command
** File description:
** handle_command_lib
*/

#include "server.h"

void handle_memory_error(char **words, int word_index)
{
    for (int i = 0; i < word_index; i++)
        free(words[i]);
    free(words);
}

int process_word_tokens(char **words, char *buffer, int *word_index)
{
    char *word = strtok(buffer, " \n\r");
    while (word) {
        words[*word_index] = strdup(word);
        if (!words[*word_index]) {
            handle_memory_error(words, *word_index);
            return -1;
        }
        word = strtok(NULL, " \n\r");
        (*word_index)++;
    }
    return 0;
}

char **get_args_from_buffer(char *buffer)
{
    if (!buffer)
        return NULL;

    int word_count = 0;
    for (int i = 0; buffer[i]; i++)
        if (buffer[i] == ' ' || buffer[i] == '\n' || buffer[i] == '\r')
            word_count++;
    char **words = malloc((word_count + 2) * sizeof(char *));
    if (!words)
        return NULL;
    int word_index = 0;
    if (process_word_tokens(words, buffer, &word_index) == -1)
        return NULL;

    words[word_index] = NULL;
    return words;
}

void free_array(char **array)
{
    if (!array)
        return;
    for (int i = 0; array[i]; i++)
        free(array[i]);
    free(array);
}
