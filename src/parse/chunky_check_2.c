#include "../../includes/minishell.h"

int check_pipe(char *token, t_token *current_token, t_data *data)
{
    if (ft_pipe_check(token, current_token) == SUCCESS)
    {
        data->cmd_ignore = false;
        current_token->echo = false;
        data->echoed = false;
        return SUCCESS;
    }
    return FAILURE;
}

int check_redirect(char *token, t_token *current_token, t_data *data)
{
    if (ft_redirect_op_check(token, current_token) == SUCCESS)
    {
        current_token->echo = false;
        data->echoed = false;
        return SUCCESS;
    }
    return FAILURE;
}

int check_command(char *token, t_token *current_token, t_data *data)
{
    if (!data->echoed && !data->cmd_ignore && (ft_command_check(token, current_token, data) == SUCCESS))
    {
        data->cmd_ignore = true;
        return SUCCESS;
    }
    return FAILURE;
}

int check_argument(char *token, t_token *current_token, t_data *data)
{
    if (ft_argument_check(token, current_token) == SUCCESS)
    {
        if (current_token->id == 1 && current_token->prev->type == ARGUMENT)
            data->cmd_ignore = true;
        if (data->echoed == true)
            current_token->echo = true;
        return SUCCESS;
    }
    return FAILURE;
}