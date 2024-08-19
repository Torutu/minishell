#include "../../includes/minishell.h"

/**
 * Retrieves the value of an environment variable from the given input string.
 * 
 * This function takes a pointer to the input string, the start position of the variable name, 
 * the length of the variable name, and a pointer to the data structure containing the environment variables.
 * 
 * It returns a pointer to the value of the environment variable, or NULL if the variable is not found.
 * 
 * @param input The input string containing the environment variable to retrieve.
 * @param var_start The start position of the variable name in the input string.
 * @param var_len The length of the variable name.
 * @param data A pointer to the data structure containing the environment variables.
 * @return A pointer to the value of the environment variable, or NULL if the variable is not found.
 */
char *get_env_value(const char *input, size_t var_start, size_t var_len, t_data *data)
{
    char var_name[var_len + 1];
    strncpy(var_name, &input[var_start], var_len);//ft_strncpy that shit
    var_name[var_len] = '\0';

    return ft_getenv(var_name, data->envll);
}

/**
 * Expands an environment variable in the given input string.
 * 
 * This function takes a pointer to the current position in the input string, 
 * a pointer to the data structure containing the environment variables, 
 * and a pointer to store the length of the expanded variable.
 * 
 * It returns a pointer to the expanded environment variable, or NULL if the variable is not found.
 * 
 * @param input The input string containing the environment variable to expand.
 * @param i A pointer to the current position in the input string.
 * @param data A pointer to the data structure containing the environment variables.
 * @param new_len A pointer to store the length of the expanded variable.
 * @return A pointer to the expanded environment variable, or NULL if the variable is not found.
 */
char *expand_env_variable(const char *input, size_t *i, t_data *data, size_t *new_len)
{
    size_t var_start = ++(*i);
    while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
        (*i)++; // Find end of variable name

    size_t var_len = *i - var_start;
    if (var_len > 0)
    {
        char *env_value = get_env_value(input, var_start, var_len, data);
        if (env_value)
        {
            *new_len = ft_strlen(env_value);
            return (env_value);
        }
        else
        {
            *new_len = 0;
            return (NULL);
        }
    }
    *new_len = 0;
    return (NULL);
}

/**
 * Handles the status variable and appends it to the result string.
 *
 * @param data Pointer to the t_data structure.
 * @param result Pointer to the result string.
 * @param j Pointer to the current index in the result string.
 *
 * @return void
 *
 * @throws None
 */
void handle_status_variable(t_data *data, char *result, size_t *j) 
{
    char *status_str = ft_itoa(data->status); // Convert integer to string
    if (status_str)
    {
        size_t k = 0;
        while (status_str[k])
            result[(*j)++] = status_str[k++]; // Append each character to result
        free(status_str); // Free the dynamically allocated memory
    }
}

/**
 * Handles a dollar sign in the input string, expanding it to its corresponding value.
 *
 * This function takes a constant input string, a pointer to the current index in the string,
 * a t_data structure, a result string, and a pointer to the current index in the result string.
 * It checks the character following the dollar sign and handles it accordingly, either by
 * appending the dollar sign to the result string, handling a status variable, or handling an
 * environment variable.
 *
 * @param input The input string to parse.
 * @param i A pointer to the current index in the input string.
 * @param data A t_data structure containing environment variable information.
 * @param result The result string to append to.
 * @param j A pointer to the current index in the result string.
 *
 * @return None
 */
void handle_dollar_sign(const char *input, size_t *i, t_data *data, char *result, size_t *j)
{
    if (input[*i + 1] == ' ' || input[*i + 1] == '\0') 
        result[(*j)++] = input[(*i)++];
    else if (data->d_quote_o && (input[*i + 1] == '"' || input[*i + 1] == '\''))
        result[(*j)++] = input[(*i)++];
    else if (input[*i + 1] == '$')
    {
        (*i) += 2;
        result[(*j)++] = '$';
    }
    else if (input[*i + 1] == '?')
    {
        (*i) += 2;
        handle_status_variable(data, result, j);
    }
    else
        handle_env_variable(input, i, data, result, j);
}


/**
 * Expands environment variables in a given input string.
 *
 * This function takes a constant input string and a t_data structure as parameters.
 * It allocates memory for the expanded result, then iterates over the input string,
 * handling single and double quotes, and expanding environment variables.
 * If memory allocation fails, it returns NULL.
 *
 * @param input The input string to expand environment variables in.
 * @param data The t_data structure containing environment variable information.
 *
 * @return A dynamically allocated string containing the expanded input string,
 *         or NULL if memory allocation fails.
 */
char *expand_env_variables(const char *input, t_data *data)
{
    setup_env_variables(input, data);
    char *result;
    size_t i;
    size_t j;
    result = (char *)malloc(MAX_ARG_STR); // must change to allocate size of env list and multiply if inside quotes
    if (!result)
        return NULL;
    i = 0;
    j = 0;
    while (input[i])
    {
        if (input[i] == '\'' && !data->d_quote_o)
            single_q(input, data, result, &i, &j);
        else if (input[i] == '"' && !data->s_quote_o)
            double_q(input, data, result, &i, &j);
        else if (input[i] == '$' && !data->s_quote_o)
            handle_dollar_sign(input, &i, data, result, &j);
        else
            result[j++] = input[i++];
    }
    result[j] = '\0'; // Null-terminate the result string
    return (result);
}
