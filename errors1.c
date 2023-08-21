#include "shell.h"

/**
 * convert_string_to_integer - converts a string to an integer
 * @string: the string to be converted
 * Return: 0 if no numbers in string, converted number otherwise
 *       -1 on error
 */
int convert_string_to_integer(char *string)
{
    int index = 0;
    unsigned long int result = 0;

    if (*string == '+')
        string++;  /* TODO: why does this make main return 255? */
    for (index = 0; string[index] != '\0'; index++)
    {
        if (string[index] >= '0' && string[index] <= '9')
        {
            result *= 10;
            result += (string[index] - '0');
            if (result > INT_MAX)
                return (-1);
        }
        else
            return (-1);
    }
    return (result);
}

/**
 * print_error_message - prints an error message
 * @information: the parameter & return information struct
 * @error_string: string containing specified error type
 */
void print_error_message(info_t *information, char *error_string)
{
    _eputs(information->filename);
    _eputs(": ");
    print_decimal(information->line_count, STDERR_FILENO);
    _eputs(": ");
    _eputs(information->argv[0]);
    _eputs(": ");
    _eputs(error_string);
}

/**
 * print_decimal - prints a decimal (integer) number (base 10)
 * @input: the input
 * @file_descriptor: the file descriptor to write to
 * Return: number of characters printed
 */
int print_decimal(int input, int file_descriptor)
{
    int (*custom_putchar)(char) = _putchar;
    int i, count = 0;
    unsigned int absolute_value, current;

    if (file_descriptor == STDERR_FILENO)
        custom_putchar = _eputchar;
    if (input < 0)
    {
        absolute_value = -input;
        custom_putchar('-');
        count++;
    }
    else
        absolute_value = input;
    current = absolute_value;
    for (i = 1000000000; i > 1; i /= 10)
    {
        if (absolute_value / i)
        {
            custom_putchar('0' + current / i);
            count++;
        }
        current %= i;
    }
    custom_putchar('0' + current);
    count++;

    return (count);
}

/**
 * convert_number_to_string - converter function, a clone of itoa
 * @number: number
 * @base: base
 * @flags: argument flags
 * Return: string
 */
char *convert_number_to_string(long int number, int base, int flags)
{
    static char *character_array;
    static char buffer[50];
    char sign = 0;
    char *pointer;
    unsigned long n = number;

    if (!(flags & CONVERT_UNSIGNED) && number < 0)
    {
        n = -number;
        sign = '-';
    }
    character_array = flags & CONVERT_LOWERCASE ? "0123456789abcdef" : "0123456789ABCDEF";
    pointer = &buffer[49];
    *pointer = '\0';

    do {
        *--pointer = character_array[n % base];
        n /= base;
    } while (n != 0);

    if (sign)
        *--pointer = sign;
    return (pointer);
}

/**
 * remove_comments_from_string - function replaces first instance of '#' with '\0'
 * @buffer: address of the string to modify
 */
void remove_comments_from_string(char *buffer)
{
    int index;

    for (index = 0; buffer[index] != '\0'; index++)
        if (buffer[index] == '#' && (!index || buffer[index - 1] == ' '))
        {
            buffer[index] = '\0';
            break;
        }
}

