/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 17:34:16 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/18 13:50:54 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Checks the type of a given token and returns the corresponding status.
 * 
 * @param token The token to be checked.
 * @param current_token The current token being processed.
 * @param data The data structure containing info about the shell state.
 * 
 * @return SUCCESS if the token type is found, FAILURE otherwise.
 */
int chunky_checker(char *token, t_token *current_token, t_data *data)
{
    if (check_builtin(token, current_token, data) == SUCCESS)
        return (SUCCESS);
    if (check_echo_flag(token, current_token, data) == SUCCESS)
        return (SUCCESS);
    if (check_flag(token, current_token, data) == SUCCESS)
        return (SUCCESS);
    if (check_pipe(token, current_token, data) == SUCCESS)
        return (SUCCESS);
    if (check_redirect(token, current_token, data) == SUCCESS)
        return (SUCCESS);
    if (check_command(token, current_token, data) == SUCCESS)
        return (SUCCESS);
    if (check_argument(token, current_token, data) == SUCCESS)
        return (SUCCESS);
    
    printf("couldn't find type for token\n");
    return (FAILURE);
}

t_token *create_and_link_next_token(t_token *current_token, t_data *data)
{
    t_token *new_token = init_token();// Create and initialize the next token
    current_token->next = new_token;
    new_token->prev = current_token;
    data->id++;// Increment the id in the data structure
    return new_token;// Return the newly created token as the new current_token
}

/**
 * Expands the value of a token if necessary,
 * replacing environment variables with their actual values.
 * 
 * @param current_token The token to be expanded.
 * @param data The data structure containing information about the shell state.
 * 
 * @return None
 */
void expand_token_if_needed(t_token *current_token, t_data *data)
{
    if (current_token->expand && current_token->value && data->quote != 1)//TO EXPAND OR NOT TO EXPAND
    {
        char *expanded_value = expand_env_variables(current_token->value, data);// Expand environment variables
        if (expanded_value)
        {
            if (current_token->value)// Free the old value if it exists
                free(current_token->value);
            current_token->value = expanded_value;// Update the token's value with the expanded value
        }
        else
            current_token->value = ft_strdup("");  // Assign an empty string or handle as needed
    }
}

t_token *initialize_tokens(t_data *data)
{
    t_token *first_node = init_token();
    data->current_token = first_node;
    data->prev_token = NULL;
    return first_node;
}

int line_tokenization(t_data *data)
{
    data->first_node = initialize_tokens(data);

    data->vtoken = ft_strtok(data->line_read, data->deli, data, data->current_token);
    if (data->status == 963)
        return (FAILURE);
    while (data->vtoken != NULL && data->status != 963)
    {
        data->current_token->id = data->id;
        data->current_token->prev = data->prev_token;
        data->current_token->value = ft_strdup(data->vtoken);
        if (chunky_checker(data->current_token->value, data->current_token, data) == FAILURE)
            return (FAILURE);
        data->vtoken = ft_strtok(NULL, data->deli, data, data->current_token);
        if (data->status == 963)
            return (FAILURE);
        if (data->vtoken != NULL && data->status != 963)
        {
            data->current_token = create_and_link_next_token(data->current_token, data);
            data->prev_token = data->current_token->prev;
        }
    }
    data->token = data->first_node;
    return (SUCCESS); // print_tokens(data); // debug
}



