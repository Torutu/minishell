#include "../../includes/minishell.h"

int	ft_strcmp(char *s1, char *s2)
{
	unsigned int i;
	i = 0;
	
	while (s1[i] == s2[i] && s1[i])
		i++;
	return (s1[i] - s2[i]);
}

int ft_charinstr(char c, const char *str)
{
	while (*str)
	{
		if (c == *str)
			return (1);
		str++;
	}
	return (0);
}

int ft_strlencount(const char *str, char c, int numtof)
{
    int i;
    i = 0;
    int found;
    found = 0;
    while(str[i] && found != numtof)
    {
        if(str[i] == c)
            found++;
        i++;
    }
    return(i);
}

void handle_quote(const char *target, t_data *data, t_token *cur_tok)
{
    (void) cur_tok;
    // Check the type of quote and update data accordingly
    if (target[data->sindex] == '\'')
        data->in_quotes = 1;         // Single quote state
    else
        data->in_quotes = 2;         // Double quote state
    data->quote_char = target[data->sindex]; // Record the quote character
    data->sindex++;                       // Skip the opening quote
}

char *validate_and_process_token(const char *target, t_data *data)
{
    if (data->in_quotes)// Check for unmatched quotes
    {
        data->status = 4;
        return (NULL);
    }
    if (data->sindex == 0 && !data->in_quotes)// Check if no token was found
        return (NULL);
    data->ctoken = ft_substr(target, data->token_start, data->sindex - data->token_start);// Allocate token and copy substring, excluding the opening and closing quotes if present
    if (!data->ctoken)
        return (NULL);
    //printf("data->ctoken:[%s]\n", data->ctoken);
    data->cnew_token = expand_env_variables(data->ctoken, data);// Expand environment variables in the token
    if (data->cnew_token)
    {
        free(data->ctoken); // Free the old token with quotes
        data->ctoken = data->cnew_token; // Update token to the expanded version
        //free(data->cnew_token);
    }
    else
    {
        free(data->ctoken); // Handle failure to expand
        data->ctoken = NULL;
    }
    return (data->ctoken);
}

int handle_special_chars(const char *target, const char *special_chars, t_data *data)
{
    // Check for multi-character special tokens first
    if (strncmp(&target[data->sindex], ">>", 2) == 0 || strncmp(&target[data->sindex], "<<", 2) == 0)
    {
        data->sindex += 2; // Skip the multi-character token
        return (2); // Indicate that a multi-character special token was found and handled
    }
    // Check for single-character special tokens
    if (ft_charinstr(target[data->sindex], special_chars))
    {
        data->sindex++; // Skip the single-character token
        return (1); // Indicate that a single-character special token was found and handled
    }
    return (0); // No special character found
}



/*char *remove_quotes(const char *str, t_data *data)
{
    int i;
    int j;
    int len = ft_strlen(str);
    char *new_str = NULL;
    char *temp_str = NULL;

    new_str = (char *)malloc(len + 1); // Max possible length, Allocate space for the result
    if (!new_str)
        return (NULL);

    i = 0;
    j = 0;
    while (str[i])
    {
        if (str[i] == '"' || str[i] == '\'')
        {
            data->quote_char = str[i];// Skip the opening quote
            i++;
            while (str[i] && str[i] != data->quote_char)
                new_str[j++] = str[i++];// Append characters between quotes
            if (str[i] == data->quote_char)
                i++;// Skip the closing quote
        }
        else
            new_str[j++] = str[i++];// Copy characters outside of quotes
    }
    new_str[j] = '\0'; // Null-terminate the new string
    temp_str = (char *)malloc(j + 1);// Allocate and copy the final cleaned string
    if (!temp_str)
    {
        free(new_str);
        return (NULL);
    }
    ft_memcpy(temp_str, new_str, j + 1);
    free(new_str); // Free the old string

    return (temp_str);
}*/
void handle_quoted_segment(const char *str, int *i, int *j, char *new_str, t_data *data)
{
    data->quote_char = str[*i]; // Use a local variable for the quote character
    (*i)++; // Skip the opening quote
    while (str[*i] && str[*i] != data->quote_char)
        new_str[(*j)++] = str[(*i)++]; // Append characters between quotes
    if (str[*i] == data->quote_char)
        (*i)++; // Skip the closing quote
}

char *remove_quotes(const char *str, t_data *data)
{
    int i;
    int j;
    i = 0;
    j = 0;
    char *new_str = (char *)malloc(ft_strlen(str) + 1);
    if (!new_str)
        return NULL;

    while (str[i])
    {
        if (str[i] == '"' || str[i] == '\'')
            handle_quoted_segment(str, &i, &j, new_str, data);
        else
            new_str[j++] = str[i++];
    }
    new_str[j] = '\0';
    return new_str;
}

