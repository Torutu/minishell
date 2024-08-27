/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 10:03:43 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/27 03:17:39 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_null(void *ptr)
{
	free(ptr);
	ptr = NULL;
	return ;
}

void	malloc_check_message(void *ptr)
{
	if (!ptr)
		exit(err_msg(NULL, MALLOC, -1));
	else
		return ;
}

void	super_free(t_data *data, t_env **env_ll)
{
	free_null(data->fin_tok);
	free_array(data->binary_paths);
	free_all_ll(env_ll);
	free(data);
}

int	wow_loop(t_data *data, t_env **env_ll)
{
	int	status;

	status = 0;
	while (666)
	{
		status = sniff_line(data);
		if (status == NULL_LINE)
		{
			status = 0;
			printf("bye bye!\n");
			break ;
		}
		else if (status == 0)
			execution(data, env_ll);
		free_gang(data);
	}
	return (status);
}

char	**add_shell_lvl(char **env)
{
	int		i;
	int		shell_level;
	char	*new_level;
	char	*new_env_entry;

	shell_level = 0;
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], "SHLVL=", 6))
		{
			shell_level = ft_atoi(env[i] + 6) + 1;
			new_level = ft_itoa(shell_level);
			if (!new_level)
				return (NULL);
			new_env_entry = ft_strjoin("SHLVL=", new_level);
			free_null(new_level);
			if (!new_env_entry)
				return (NULL);
			env[i] = ft_strdup(new_env_entry);
			free_null(new_env_entry);
		}
		i++;
	}
	return (env);
}
