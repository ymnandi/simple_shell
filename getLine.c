#include "shell.h"

/**
 * buffer_chained_commands - buffers chained commands
 * @information: parameter struct
 * @buffer: address of buffer
 * @length: address of length var
 *
 * Return: bytes read
 */
ssize_t buffer_chained_commands(info_t *information, char **buffer, size_t *length)
{
	ssize_t bytes_read = 0;
	size_t current_length = 0;

	if (!*length) /* if nothing left in the buffer, fill it */
	{
		/*bfree((void **)information->command_buffer);*/
		free(*buffer);
		*buffer = NULL;
		signal(SIGINT, handle_SIGINT);
#if USE_GETLINE
		bytes_read = getline(buffer, &current_length, stdin);
#else
		bytes_read = _getline(information, buffer, &current_length);
#endif
		if (bytes_read > 0)
		{
			if ((*buffer)[bytes_read - 1] == '\n')
			{
				(*buffer)[bytes_read - 1] = '\0'; /* remove trailing newline */
				bytes_read--;
			}
			information->line_count_flag = 1;
			remove_comments(*buffer);
			build_history_list(information, *buffer, information->history_count++);
			/* if (_strchr(*buffer, ';')) is this a command chain? */
			{
				*length = bytes_read;
				information->command_buffer = buffer;
			}
		}
	}
	return (bytes_read);
}

/**
 * get_input_line - gets a line minus the newline
 * @information: parameter struct
 *
 * Return: bytes read
 */
ssize_t get_input_line(info_t *information)
{
	static char *buffer; /* the ';' command chain buffer */
	static size_t start, end, length;
	ssize_t bytes_read = 0;
	char **buffer_ptr = &(information->arguments), *position;

	_putchar(BUF_FLUSH);
	bytes_read = buffer_chained_commands(information, &buffer, &length);
	if (bytes_read == -1) /* EOF */
		return (-1);
	if (length) /* we have commands left in the chain buffer */
	{
		end = start; /* init new iterator to current buffer position */
		position = buffer + start; /* get pointer for return */

		check_command_chain(information, buffer, &end, start, length);
		while (end < length) /* iterate to semicolon or end */
		{
			if (is_command_chain(information, buffer, &end))
				break;
			end++;
		}

		start = end + 1; /* increment past nulled ';'' */
		if (start >= length) /* reached end of buffer? */
		{
			start = length = 0; /* reset position and length */
			information->command_buffer_type = CMD_NORMAL;
		}

		*buffer_ptr = position; /* pass back pointer to current command position */
		return (_strlen(position)); /* return length of current command */
	}

	*buffer_ptr = buffer; /* else not a chain, pass back buffer from _getline() */
	return (bytes_read); /* return length of buffer from _getline() */
}

/**
 * read_buffer - reads a buffer
 * @information: parameter struct
 * @buffer: buffer
 * @index: size
 *
 * Return: bytes read
 */
ssize_t read_buffer(info_t *information, char *buffer, size_t *index)
{
	ssize_t bytes_read = 0;

	if (*index)
		return (0);
	bytes_read = read(information->read_fd, buffer, READ_BUFFER_SIZE);
	if (bytes_read >= 0)
		*index = bytes_read;
	return (bytes_read);
}

/**
 * _getline_custom - gets the next line of input from STDIN
 * @information: parameter struct
 * @pointer: address of pointer to buffer, preallocated or NULL
 * @buffer_length: size of preallocated ptr buffer if not NULL
 *
 * Return: size
 */
int _getline_custom(info_t *information, char **pointer, size_t *buffer_length)
{
	static char buffer[READ_BUFFER_SIZE];
	static size_t start, length;
	size_t k;
	ssize_t bytes_read = 0, size = 0;
	char *ptr = NULL, *new_ptr = NULL, *character;

	ptr = *pointer;
	if (ptr && buffer_length)
		size = *buffer_length;
	if (start == length)
		start = length = 0;

	bytes_read = read_buffer(information, buffer, &length);
	if (bytes_read == -1 || (bytes_read == 0 && length == 0))
		return (-1);

	character = _strchr(buffer + start, '\n');
	k = character ? 1 + (unsigned int)(character - buffer) : length;
	new_ptr = _realloc(ptr, size, size ? size + k : k + 1);
	if (!new_ptr) /* MALLOC FAILURE! */
		return (ptr ? free(ptr), -1 : -1);

	if (size)
		_strncat(new_ptr, buffer + start, k - start);
	else
		_strncpy(new_ptr, buffer + start, k - start + 1);

	size += k - start;
	start = k;
	ptr = new_ptr;

	if (buffer_length)
		*buffer_length = size;
	*pointer = ptr;
	return (size);
}

/**
 * handle_SIGINT - blocks ctrl-C
 * @signal_number: the signal number
 *
 * Return: void
 */
void handle_SIGINT(__attribute__((unused))int signal_number)
{
	_puts("\n");
	_puts("$ ");
	_putchar(BUF_FLUSH);
}

