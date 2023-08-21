#include "shell.h"

/**
 * obtain_history_file - gets the history file
 * @information: parameter struct
 *
 * Return: allocated string containing history file
 */
char *obtain_history_file(info_t *information)
{
	char *file_buffer, *directory;

	directory = _getenv(information, "HOME=");
	if (!directory)
		return (NULL);
	file_buffer = malloc(sizeof(char) * (_strlen(directory) + _strlen(HIST_FILE) + 2));
	if (!file_buffer)
		return (NULL);
	file_buffer[0] = 0;
	_strcpy(file_buffer, directory);
	_strcat(file_buffer, "/");
	_strcat(file_buffer, HIST_FILE);
	return (file_buffer);
}

/**
 * write_to_history - creates a file, or appends to an existing file
 * @information: the parameter struct
 *
 * Return: 1 on success, else -1
 */
int write_to_history(info_t *information)
{
	ssize_t file_descriptor;
	char *filename = obtain_history_file(information);
	list_t *node = NULL;

	if (!filename)
		return (-1);

	file_descriptor = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (file_descriptor == -1)
		return (-1);
	for (node = information->history; node; node = node->next)
	{
		_puts_to_fd(node->string, file_descriptor);
		_put_to_fd('\n', file_descriptor);
	}
	_put_to_fd(BUF_FLUSH, file_descriptor);
	close(file_descriptor);
	return (1);
}

/**
 * read_from_history - reads history from file
 * @information: the parameter struct
 *
 * Return: history count on success, 0 otherwise
 */
int read_from_history(info_t *information)
{
	int i, last = 0, line_count = 0;
	ssize_t file_descriptor, read_length, file_size = 0;
	struct stat stats;
	char *buffer = NULL, *filename = obtain_history_file(information);

	if (!filename)
		return (0);

	file_descriptor = open(filename, O_RDONLY);
	free(filename);
	if (file_descriptor == -1)
		return (0);
	if (!fstat(file_descriptor, &stats))
		file_size = stats.st_size;
	if (file_size < 2)
		return (0);
	buffer = malloc(sizeof(char) * (file_size + 1));
	if (!buffer)
		return (0);
	read_length = read(file_descriptor, buffer, file_size);
	buffer[file_size] = 0;
	if (read_length <= 0)
		return (free(buffer), 0);
	close(file_descriptor);
	for (i = 0; i < file_size; i++)
		if (buffer[i] == '\n')
		{
			buffer[i] = 0;
			build_history_list(information, buffer + last, line_count++);
			last = i + 1;
		}
	if (last != i)
		build_history_list(information, buffer + last, line_count++);
	free(buffer);
	information->history_count = line_count;
	while (information->history_count-- >= HIST_MAX)
		delete_node_at_index(&(information->history), 0);
	renumber_history(information);
	return (information->history_count);
}

/**
 * construct_history_list - adds entry to a history linked list
 * @information: Structure containing potential arguments. Used to maintain
 * @buffer: buffer
 * @line_count: the history line count, history_count
 *
 * Return: Always 0
 */
int construct_history_list(info_t *information, char *buffer, int line_count)
{
	list_t *node = NULL;

	if (information->history)
		node = information->history;
	add_node_end(&node, buffer, line_count);

	if (!information->history)
		information->history = node;
	return (0);
}

/**
 * renumber_history_count - renumbers the history linked list after changes
 * @information: Structure containing potential arguments. Used to maintain
 *
 * Return: the new history count
 */
int renumber_history_count(info_t *information)
{
	list_t *node = information->history;
	int i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}
	return (information->history_count = i);
}

