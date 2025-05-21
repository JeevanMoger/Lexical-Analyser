#include "main.h"

/**
 * is_keyword - Checks if a given string is a keyword.
 * @str: The string to check.
 *
 * This function compares the input string with a list of 32 C keywords.
 * If the string matches any keyword, it returns SUCCESS; otherwise, FAILURE.
 *
 * Return: SUCCESS if the string is a keyword, otherwise FAILURE.
 */
Status is_keyword(char *str) 
{
    char keywords[32][15] = {
        "auto", "break", "case", "char", "const", "continue", "default", "do",
        "double", "else", "enum", "extern", "float", "for", "goto", "if",
        "int", "long", "register", "return", "short", "signed", "sizeof",
        "static", "struct", "switch", "typedef", "union", "unsigned", "void",
        "volatile", "while"
    };

    // Loop through all keywords and compare with the given string
    for (int i = 0; i < 32; i++)
    {
        if (strcmp(str, keywords[i]) == 0)
        {
            return SUCCESS;
        }
    }
    return FAILURE;
}

/**
 * is_operator - Checks if a given character is an operator.
 * @ch: The character to check.
 *
 * This function checks if the input character is one of the common C operators.
 * It returns SUCCESS if the character is an operator, otherwise FAILURE.
 *
 * Return: SUCCESS if the character is an operator, otherwise FAILURE.
 */
Status is_operator(char ch) 
{
    char operators[] = "+-*/%<>=!&|^()[]";

    // Loop through the list of operators and compare with the given character
    for (int i = 0; i < strlen(operators); i++)
    {
        if (ch == operators[i])
            return SUCCESS;
    }
    return FAILURE;
}

/**
 * validate_octal - Validates if a string represents a valid octal number.
 * @num: The string containing the octal number.
 * @len: The length of the string.
 *
 * This function checks if all characters in the string are between '0' and '7'.
 * If any non-octal digit is found, it returns FAILURE.
 *
 * Return: SUCCESS if the string is a valid octal number, otherwise FAILURE.
 */
Status validate_octal(char *num, int len)
{
    // Iterate through each character and check if it is a valid octal digit
    for (int i = 0; i < len; i++)
    {
        if (!(num[i] >= '0' && num[i] <= '7'))
            return FAILURE;
    }
    return SUCCESS;
}

/**
 * validate_char - Validates if a string represents a valid character literal.
 * @lexeme: The string to validate.
 *
 * This function checks if the input string is a valid character literal.
 * A valid character literal must be:
 * - A single character enclosed in single quotes (e.g., 'a').
 * - A valid escape sequence like '\n', '\t', '\0', or '\''.
 * If the string does not meet these conditions, it returns FAILURE.
 *
 * Return: SUCCESS if the string is a valid character literal, otherwise FAILURE.
 */
Status validate_char(char *lexeme) 
{
    // Check for normal character literals like 'a'
    if (strlen(lexeme) == 3 && lexeme[0] == '\'' && lexeme[2] == '\'' && lexeme[1] != '\\') 
    {
        return SUCCESS;
    }

    // Check for valid escape sequences like '\n', '\t', '\0', '\''
    if (strlen(lexeme) == 4 && lexeme[0] == '\'' && lexeme[1] == '\\' &&
        (lexeme[2] == 'n' || lexeme[2] == 't' || lexeme[2] == '0' || lexeme[2] == '\'') && lexeme[3] == '\'') 
    {
        return SUCCESS;
    }

    return FAILURE;
}
