#include "shell.h"

/**
 * display_history - displays the history list, one command by line, preceded
 *                  with line numbers, starting at 0.
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * Return: Always 0
 */
int display_history(info_t *information)
{
    print_list(information->history);
    return (0);
}

/**
 * unset_custom_alias - sets alias to string
 * @info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int unset_custom_alias(info_t *information, char *string)
{
    char *position, character;
    int result;

    position = _strchr(string, '=');
    if (!position)
        return (1);
    character = *position;
    *position = 0;
    result = delete_node_at_index(&(information->alias),
                                  get_node_index(information->alias, node_starts_with(information->alias, string, -1)));
    *position = character;
    return (result);
}

/**
 * set_custom_alias - sets alias to string
 * @info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int set_custom_alias(info_t *information, char *string)
{
    char *position;

    position = _strchr(string, '=');
    if (!position)
        return (1);
    if (!*++position)
        return (unset_custom_alias(information, string));

    unset_custom_alias(information, string);
    return (add_node_end(&(information->alias), string, 0) == NULL);
}

/**
 * print_custom_alias - prints an alias string
 * @node: the alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int print_custom_alias(list_t *node)
{
    char *position = NULL, *alias_string = NULL;

    if (node)
    {
        position = _strchr(node->str, '=');
        for (alias_string = node->str; alias_string <= position; alias_string++)
            _putchar(*alias_string);
        _putchar('\'');
        _puts(position + 1);
        _puts("'\n");
        return (0);
    }
    return (1);
}

/**
 * manage_custom_alias - mimics the alias builtin (man alias)
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * Return: Always 0
 */
int manage_custom_alias(info_t *information)
{
    int index = 0;
    char *position = NULL;
    list_t *alias_node = NULL;

    if (information->argc == 1)
    {
        alias_node = information->alias;
        while (alias_node)
        {
            print_custom_alias(alias_node);
            alias_node = alias_node->next;
        }
        return (0);
    }
    for (index = 1; information->argv[index]; index++)
    {
        position = _strchr(information->argv[index], '=');
        if (position)
            set_custom_alias(information, information->argv[index]);
        else
            print_custom_alias(node_starts_with(information->alias, information->argv[index], '='));
    }

    return (0);
}

