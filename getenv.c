#include "shell.h"

/**
 * copy_environment - returns the string array copy of our environment
 * @information: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: String array containing environment variables
 */
char **copy_environment(info_t *information)
{
	if (!information->environment || information->environment_changed)
	{
		information->environment = list_to_strings(information->environment_list);
		information->environment_changed = 0;
	}

	return (information->environment);
}

/**
 * remove_environment_variable - Remove an environment variable
 * @information: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @variable: the string environment variable property
 * Return: 1 on delete, 0 otherwise
 */
int remove_environment_variable(info_t *information, char *variable)
{
	list_t *node = information->environment_list;
	size_t index = 0;
	char *position;

	if (!node || !variable)
		return (0);

	while (node)
	{
		position = starts_with(node->string, variable);
		if (position && *position == '=')
		{
			information->environment_changed = delete_node_at_index(&(information->environment_list), index);
			index = 0;
			node = information->environment_list;
			continue;
		}
		node = node->next;
		index++;
	}
	return (information->environment_changed);
}

/**
 * set_environment_variable - Initialize a new environment variable,
 *             or modify an existing one
 * @information: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @variable: the string environment variable property
 * @value: the string environment variable value
 * Return: Always 0
 */
int set_environment_variable(info_t *information, char *variable, char *value)
{
	char *buffer = NULL;
	list_t *node;
	char *position;

	if (!variable || !value)
		return (0);

	buffer = malloc(_strlen(variable) + _strlen(value) + 2);
	if (!buffer)
		return (1);
	_strcpy(buffer, variable);
	_strcat(buffer, "=");
	_strcat(buffer, value);
	node = information->environment_list;
	while (node)
	{
		position = starts_with(node->string, variable);
		if (position && *position == '=')
		{
			free(node->string);
			node->string = buffer;
			information->environment_changed = 1;
			return (0);
		}
		node = node->next;
	}
	add_node_end(&(information->environment_list), buffer, 0);
	free(buffer);
	information->environment_changed = 1;
	return (0);
}

