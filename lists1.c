#include "shell.h"

/**
 * get_list_length - determines the length of a linked list
 * @node: pointer to the first node
 *
 * Return: size of the list
 */
size_t get_list_length(const list_t *node)
{
	size_t length = 0;

	while (node)
	{
		node = node->next;
		length++;
	}
	return (length);
}

/**
 * convert_list_to_strings - returns an array of strings from the list->str
 * @head: pointer to the first node
 *
 * Return: array of strings
 */
char **convert_list_to_strings(list_t *head)
{
	list_t *current_node = head;
	size_t length = get_list_length(head), i;
	char **string_array;
	char *string;

	if (!head || !length)
		return (NULL);
	string_array = malloc(sizeof(char *) * (length + 1));
	if (!string_array)
		return (NULL);
	for (i = 0; current_node; current_node = current_node->next, i++)
	{
		string = malloc(_strlen(current_node->str) + 1);
		if (!string)
		{
			for (size_t j = 0; j < i; j++)
				free(string_array[j]);
			free(string_array);
			return (NULL);
		}

		string = _strcpy(string, current_node->str);
		string_array[i] = string;
	}
	string_array[i] = NULL;
	return (string_array);
}

/**
 * display_list - prints all elements of a list_t linked list
 * @node: pointer to the first node
 *
 * Return: size of the list
 */
size_t display_list(const list_t *node)
{
	size_t length = 0;

	while (node)
	{
		_puts(convert_number_to_string(node->number, 10, 0));
		_putchar(':');
		_putchar(' ');
		_puts(node->string ? node->string : "(nil)");
		_puts("\n");
		node = node->next;
		length++;
	}
	return (length);
}

/**
 * find_node_starts_with - returns a node whose string starts with a prefix
 * @head: pointer to the list head
 * @prefix: string to match
 * @character: the next character after the prefix to match
 *
 * Return: matching node or NULL
 */
list_t *find_node_starts_with(list_t *head, char *prefix, char character)
{
	char *position = NULL;

	while (head)
	{
		position = starts_with(head->string, prefix);
		if (position && ((character == -1) || (*position == character)))
			return (head);
		head = head->next;
	}
	return (NULL);
}

/**
 * get_node_index - gets the index of a node
 * @head: pointer to the list head
 * @node: pointer to the node
 *
 * Return: index of the node or -1
 */
ssize_t get_node_index(list_t *head, list_t *node)
{
	size_t index = 0;

	while (head)
	{
		if (head == node)
			return (index);
		head = head->next;
		index++;
	}
	return (-1);
}

