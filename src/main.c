/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 10:12:51 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/21 13:24:35 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_gang(t_data *data)
{
	if(data->tok_res != NULL)
	{
		free(data->tok_res);
		data->tok_res = NULL;
	}
	if(data->ctoken != NULL)
	{
		free(data->ctoken);
		data->ctoken = NULL;
	}
	if(data->temp_str != NULL)
	{
		free(data->temp_str);
		data->temp_str = NULL;
	}
	if(data->new_str != NULL)
	{
		free(data->new_str);
		data->new_str = NULL;
	}
	free_tokens(data->token);
	// if(data->tok_str != NULL)
	// {
	// 	free(data->tok_str);
	// 	data->tok_str = NULL;
	// }
}

int	main(int argc, char **argv, char **env)
{
	t_data			*data;
	t_env			**env_ll;
	static int		status;
	
	(void)argv;
	data = ft_calloc(1, sizeof(t_data));
	env_ll = ft_calloc(1, sizeof(t_env *));
	if (!data  || !env_ll)
		return (1);
	if (argc == 1)
	{
		initializer(data, env_ll, env);
		while (666)
		{
			status = sniff_line(data);
			if (status == NULL_LINE)
			{
				printf("exit\n");
				break ;
			}
			else if (status == 963)
				continue ;
			else
				execution(data, env_ll);
			free_gang(data);
		}
		free_gang(data);
		free(data->line_read);
	}
	else
		ft_putstr_fd(ERR_ARG, 2);
	free_gang(data);
	free_array(data->binary_paths);
	free_ll((*env_ll));
	free_null(env_ll);
	free(data);
	return (0);
}
