/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 17:34:00 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/18 14:03:47 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"



void initialize_tokenization(t_data *data)
{
    data->sindex = 0;
    data->token_start = 0; // Start data->sindex of the token
    data->quote = 0;
    data->in_quotes = 0;
    data->quote_char = '\0';
}

char *skip_starting_delim(const char *str, const char *delim, char **target)
{
    if (str)
        *target = (char *)str;  // Cast away const for internal usage; ensure this cast is safe
    if (!*target || **target == '\0')
        return NULL;

    while (**target && ft_charinstr(**target, delim))  // Skip leading delimiters
    {
        (*target)++;
        if (**target == '\0')
            return NULL;
    }
    return *target;  // Return the updated target pointer
}

void remove_quotes_and_skip_delimiters(const char *delim, t_data *data, char **target)
{
    //char *stripped_token;
    data->ctoken = remove_quotes(data->ctoken, data); // Remove quotes from the token
    //data->ctoken = stripped_token;
    //free(stripped_token); // Free the stripped token
    *target += data->sindex; // Move target pointer past the token

    while (**target && ft_charinstr(**target, delim)) // Skip trailing delimiters for next call
        (*target)++;
}

void process_quoting_and_delimiters(const char *target, const char *delim, t_data *data, t_token *cur_tok)
{
    while (target[data->sindex])
    {
        if (data->in_quotes)
        {
            if (target[data->sindex] == data->quote_char)
            {
                data->in_quotes = 0;        // End quote state
                data->quote_char = '\0';   // Clear quote character
                data->sindex++;            // Move past the closing quote
                continue;
            }
        }
        else if (target[data->sindex] == '"' || target[data->sindex] == '\'')
        {
            handle_quote(target, data, cur_tok);
            continue;
        }
        else if (ft_charinstr(target[data->sindex], delim) && !data->in_quotes)
            break;
        data->sindex++;  // Move to the next character
    }
}

int unmatched_quote_check(t_data *data)
{
    //printf("data->echoed:%d\n",data->echoed);
    if (data->in_quotes)
    {
        printf("unmatched quote 😳\n");
        return FAILURE;
    }
    return SUCCESS;
}

char *ft_strtok(char *str, const char *delim, t_data *data, t_token *cur_tok)
{
    static char *target;
    initialize_tokenization(data);
    if(str)
        modify_str(str);
    if(skip_starting_delim(str, delim, &target) == NULL)
        return (NULL);
    
    process_quoting_and_delimiters(target, delim, data, cur_tok);
    
    if (unmatched_quote_check(data) == FAILURE)
    {
        data->status = 963;
        return (NULL);
    }
    char *token;
    
    token = validate_and_process_token(target, data);
    //printf("token:[%s]\n", token);//debug
    if (!token)
        return (NULL);
    
    remove_quotes_and_skip_delimiters(delim, data, &target);
    free(token);
    //printf("ctoken:[%s]\n", data->ctoken);//debug
    return (data->ctoken);
}
