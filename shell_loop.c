#include "shell.h"

/**
 * shell_loop - main shell loop
 * @info: the parameter & return info struct
 * @argument_vector: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int shell_loop(info_t *info, char **argument_vector)
{
	ssize_t read_result = 0;
	int builtin_result = 0;

	while (read_result != -1 && builtin_result != -2)
	{
		clear_info(info);
		if (is_interactive(info))
			_puts("$ ");
		_eputchar(BUF_FLUSH);
		read_result = get_input(info);
		if (read_result != -1)
		{
			set_info(info, argument_vector);
			builtin_result = find_builtin(info);
			if (builtin_result == -1)
				find_command(info);
		}
		else if (is_interactive(info))
			_putchar('\n');
		free_info(info, 0);
	}
	write_history(info);
	free_info(info, 1);
	if (!is_interactive(info) && info->status)
		exit(info->status);
	if (builtin_result == -2)
	{
		if (info->error_number == -1)
			exit(info->status);
		exit(info->error_number);
	}
	return (builtin_result);
}

/**
 * find_builtin - finds a builtin command
 * @info: the parameter & return info struct
 *
 * Return: -1 if builtin not found,
 *         0 if builtin executed successfully,
 *         1 if builtin found but not successful,
 *         -2 if builtin signals exit()
 */
int find_builtin(info_t *info)
{
	int i, builtin_result = -1;
	builtin_table builtintbl[] = {
		{"exit", _myexit},
		{"env", _myenv},
		{"help", _myhelp},
		{"history", _myhistory},
		{"setenv", _mysetenv},
		{"unsetenv", _myunsetenv},
		{"cd", _mycd},
		{"alias", _myalias},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
		if (_strcmp(info->argument_vector[0], builtintbl[i].type) == 0)
		{
			info->line_count++;
			builtin_result = builtintbl[i].func(info);
			break;
		}
	return (builtin_result);
}

/**
 * find_command - finds a command in PATH
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void find_command(info_t *info)
{
	char *path = NULL;
	int i, non_delim_count;

	info->command_path = info->argument_vector[0];
	if (info->line_count_flag == 1)
	{
		info->line_count++;
		info->line_count_flag = 0;
	}
	for (i = 0, non_delim_count = 0; info->arguments[i]; i++)
		if (!is_delimiter(info->arguments[i], " \t\n"))
			non_delim_count++;
	if (!non_delim_count)
		return;

	path = find_path(info, _getenv(info, "PATH="), info->argument_vector[0]);
	if (path)
	{
		info->command_path = path;
		fork_command(info);
	}
	else
	{
		if ((is_interactive(info) || _getenv(info, "PATH=")
			|| info->argument_vector[0][0] == '/') && is_executable_command(info, info->argument_vector[0]))
			fork_command(info);
		else if (*(info->arguments) != '\n')
		{
			info->status = 127;
			print_error(info, "not found\n");
		}
	}
}

/**
 * fork_command - forks a child process to run the command
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void fork_command(info_t *info)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(info->command_path, info->argument_vector, get_environ(info)) == -1)
		{
			free_info(info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(info->status));
		if (WIFEXITED(info->status))
		{
			info->status = WEXITSTATUS(info->status);
			if (info->status == 126)
				print_error(info, "Permission denied\n");
		}
	}
}

