#include "shell.h"

/**
 * split_string_by_delim - splits a string into words. Repeat delimiters are ignored
 * @input_str: the input string
 * @delimiter_str: the delimiter string
 * Return: a pointer to an array of strings, or NULL on failure
 */
char **split_string_by_delim(char *input_str, char *delimiter_str)
{
	int i, j, k, m, num_words = 0;
	char **result;

	if (input_str == NULL || input_str[0] == 0)
		return (NULL);
	if (!delimiter_str)
		delimiter_str = " ";
	for (i = 0; input_str[i] != '\0'; i++)
		if (!is_delim(input_str[i], delimiter_str) && (is_delim(input_str[i + 1], delimiter_str) || !input_str[i + 1]))
			num_words++;

	if (num_words == 0)
		return (NULL);
	result = malloc((1 + num_words) * sizeof(char *));
	if (!result)
		return (NULL);
	for (i = 0, j = 0; j < num_words; j++)
	{
		while (is_delim(input_str[i], delimiter_str))
			i++;
		k = 0;
		while (!is_delim(input_str[i + k], delimiter_str) && input_str[i + k])
			k++;
		result[j] = malloc((k + 1) * sizeof(char));
		if (!result[j])
		{
			for (k = 0; k < j; k++)
				free(result[k]);
			free(result);
			return (NULL);
		}
		for (m = 0; m < k; m++)
			result[j][m] = input_str[i++];
		result[j][m] = 0;
	}
	result[j] = NULL;
	return (result);
}

/**
 * split_string_by_delim2 - splits a string into words
 * @input_str: the input string
 * @delimiter: the delimiter character
 * Return: a pointer to an array of strings, or NULL on failure
 */
char **split_string_by_delim2(char *input_str, char delimiter)
{
	int i, j, k, m, num_words = 0;
	char **result;

	if (input_str == NULL || input_str[0] == 0)
		return (NULL);
	for (i = 0; input_str[i] != '\0'; i++)
		if ((input_str[i] != delimiter && input_str[i + 1] == delimiter) ||
		    (input_str[i] != delimiter && !input_str[i + 1]) || input_str[i + 1] == delimiter)
			num_words++;
	if (num_words == 0)
		return (NULL);
	result = malloc((1 + num_words) * sizeof(char *));
	if (!result)
		return (NULL);
	for (i = 0, j = 0; j < num_words; j++)
	{
		while (input_str[i] == delimiter && input_str[i] != delimiter)
			i++;
		k = 0;
		while (input_str[i + k] != delimiter && input_str[i + k] && input_str[i + k] != delimiter)
			k++;
		result[j] = malloc((k + 1) * sizeof(char));
		if (!result[j])
		{
			for (k = 0; k < j; k++)
				free(result[k]);
			free(result);
			return (NULL);
		}
		for (m = 0; m < k; m++)
			result[j][m] = input_str[i++];
		result[j][m] = 0;
	}
	result[j] = NULL;
	return (result);
}

