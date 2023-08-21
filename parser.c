#include "shell.h"

/**
 * is_executable_command - determines if a file is an executable command
 * @information: the information struct
 * @file_path: path to the file
 *
 * Return: 1 if true, 0 otherwise
 */
int is_executable_command(info_t *information, char *file_path)
{
	struct stat stats;

	(void)information;
	if (!file_path || stat(file_path, &stats))
		return (0);

	if (stats.st_mode & S_IFREG)
	{
		return (1);
	}
	return (0);
}

/**
 * duplicate_substring - duplicates characters within a substring
 * @source: the source string
 * @start: starting index of substring
 * @end: ending index of substring
 *
 * Return: pointer to new buffer containing duplicated substring
 */
char *duplicate_substring(char *source, int start, int end)
{
	static char buffer[1024];
	int i = 0, k = 0;

	for (k = 0, i = start; i < end; i++)
		if (source[i] != ':')
			buffer[k++] = source[i];
	buffer[k] = 0;
	return (buffer);
}

/**
 * find_command_path - finds the command's path in the PATH string
 * @information: the information struct
 * @path_string: the PATH string
 * @command: the command to find
 *
 * Return: full path of the command if found, or NULL
 */
char *find_command_path(info_t *information, char *path_string, char *command)
{
	int i = 0, current_pos = 0;
	char *path;

	if (!path_string)
		return (NULL);
	if ((_strlen(command) > 2) && starts_with(command, "./"))
	{
		if (is_executable_command(information, command))
			return (command);
	}
	while (1)
	{
		if (!path_string[i] || path_string[i] == ':')
		{
			path = duplicate_substring(path_string, current_pos, i);
			if (!*path)
				_strcat(path, command);
			else
			{
				_strcat(path, "/");
				_strcat(path, command);
			}
			if (is_executable_command(information, path))
				return (path);
			if (!path_string[i])
				break;
			current_pos = i;
		}
		i++;
	}
	return (NULL);
}

