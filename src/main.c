/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 10:12:51 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/19 14:59:14 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_gang(t_data *data)
{
	if(data->vtoken != NULL)
	{
		free(data->vtoken);
		data->vtoken = NULL;
	}
	if(data->ctoken != NULL)
	{
		free(data->ctoken);
		data->ctoken = NULL;
	}
	free_tokens(data->token);
}

int	main(int argc, char **argv, char **env)
{
	t_data			*data;
	t_env			*env_ll;
	static int		status;
	
	(void)argv;
	data = ft_calloc(1, sizeof(t_data));
	if (!data)
		return (1);
	if (argc == 1)
	{
		env_ll = NULL;
		initializer(data, &env_ll, env);
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
				execution(data, &env_ll);
		}
		free_gang(data);
		free(data);
		//free(data->line_read);
	}
	else
		ft_putstr_fd(ERR_ARG, 2);
	return (0);
}
