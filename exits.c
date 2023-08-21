#include "shell.h"

/**
 **copy_string_with_limit - copies a string with a length limit
 *@destination: the destination string to be copied to
 *@source: the source string
 *@limit: the maximum number of characters to be copied
 *Return: the copied string
 */
char *copy_string_with_limit(char *destination, char *source, int limit)
{
	int i, j;
	char *result = destination;

	i = 0;
	while (source[i] != '\0' && i < limit - 1)
	{
		destination[i] = source[i];
		i++;
	}
	if (i < limit)
	{
		j = i;
		while (j < limit)
		{
			destination[j] = '\0';
			j++;
		}
	}
	return (result);
}

/**
 **concatenate_strings_with_limit - concatenates two strings with a length limit
 *@destination: the first string
 *@source: the second string
 *@limit: the maximum number of bytes to be used
 *Return: the concatenated string
 */
char *concatenate_strings_with_limit(char *destination, char *source, int limit)
{
	int i, j;
	char *result = destination;

	i = 0;
	j = 0;
	while (destination[i] != '\0')
		i++;
	while (source[j] != '\0' && j < limit)
	{
		destination[i] = source[j];
		i++;
		j++;
	}
	if (j < limit)
		destination[i] = '\0';
	return (result);
}

/**
 **find_character_in_string - locates a character in a string
 *@string: the string to be parsed
 *@character: the character to look for
 *Return: (string) a pointer to the memory area string
 */
char *find_character_in_string(char *string, char character)
{
	do {
		if (*string == character)
			return (string);
	} while (*string++ != '\0');

	return (NULL);
}

