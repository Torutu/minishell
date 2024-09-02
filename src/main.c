/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 10:53:35 by walnaimi          #+#    #+#             */
/*   Updated: 2024/09/02 18:28:41 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_mod = 0;

/**
 * The main function of the minishell program.
 *
 * This function takes in the program arguments and the environment variables,
 * and starts the minishell program loop.
 *
 * @param argc The number of arguments passed to the program.
 * @param argv The array of arguments passed to the program.
 * @param env The array of environment variables.
 *
 * @return The exit status of the program.
 */
int	main(int argc, char **argv, char **env)
{
	t_data	*data;
	t_env	**env_ll;
	int		status;

	status = 0;
	if (argc != 1)
		exit(err_msg(NULL, ERR_ARG, 1));
	(void)argv;
	if (!env || !*env)
		return (1);
	env = add_shell_lvl(env);
	data = ft_calloc(1, sizeof(t_data));
	if (!data)
		return (1);
	env_ll = ft_calloc(1, sizeof(t_env *));
	if (!env_ll)
	{
		free_null(data);
		return (1);
	}
	initializer(data, env_ll, env);
	status = wow_loop(data, env_ll);
	super_free(data, env_ll);
	return (status);
}
