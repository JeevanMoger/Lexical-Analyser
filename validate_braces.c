#include "main.h"

/**
 * validate_braces - Validates the balance of parentheses, braces, and brackets in a file.
 * @fdata: Pointer to the file to be checked.
 *
 * This function reads characters from the given file and keeps track of 
 * opening and closing parentheses '()', curly braces '{}', and square brackets '[]'.
 * If any of these symbols are unbalanced, an error message is printed,
 * and the function returns FAILURE. Otherwise, it returns SUCCESS.
 *
 * Return: SUCCESS if all symbols are balanced, otherwise FAILURE.
 */
Status validate_braces(FILE* fdata)
{
    char ch;
    int paren_count = 0;   // Counter for parentheses ()
    int braces_count = 0;  // Counter for curly braces {}
    int sqr_count = 0;     // Counter for square brackets []

    // Read characters from the file one by one
    while ((ch = fgetc(fdata)) != EOF)
    {
        // Update counters based on encountered characters
        if (ch == '(')
        {
            paren_count++;
        }
        if (ch == ')')
        {
            paren_count--;
        }
        if (ch == '{')
        {
            braces_count++;
        }
        if (ch == '}')
        {
            braces_count--;
        }
        if (ch == '[')
        {
            sqr_count++;
        }
        if (ch == ']')
        {
            sqr_count--;
        }
    }

    // Check for unmatched parentheses
    if (paren_count > 0)
    {
        printf(RED"ERROR : EXPECTING ')' \n"RESET);
        return FAILURE;
    }
    else if (paren_count < 0)
    {
        printf(RED"ERROR : EXPECTING '('\n"RESET);
        return FAILURE;
    }

    // Check for unmatched curly braces
    if (braces_count > 0)
    {
        printf(RED"ERROR : EXPECTING '}' \n"RESET);
        return FAILURE;
    }
    else if (braces_count < 0)
    {
        printf(RED"ERROR : EXPECTING '{'\n"RESET);
        return FAILURE;
    }

    // Check for unmatched square brackets
    if (sqr_count > 0)
    {
        printf(RED"ERROR : EXPECTING ']' \n"RESET);
        return FAILURE;
    }
    else if (sqr_count < 0)
    {
        printf(RED"ERROR : EXPECTING '[' \n"RESET);
        return FAILURE;
    }

    return SUCCESS;
}
