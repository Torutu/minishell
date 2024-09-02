/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap_tokens.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 01:08:46 by walnaimi          #+#    #+#             */
/*   Updated: 2024/09/02 01:08:46 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	swap_type(t_token *a, t_token *b)
{
	t_type	tmp_type;

	tmp_type = a->type;
	a->type = b->type;
	b->type = tmp_type;
}

void	swap_value(t_token *a, t_token *b)
{
	char	*tmp_value;

	tmp_value = a->value;
	a->value = b->value;
	b->value = tmp_value;
}

// void	swap_path(t_token *a, t_token *b)
// {
// 	char	*tmp_path;

// 	tmp_path = a->path;
// 	a->path = b->path;
// 	b->path = tmp_path;
// }

void	swap_echo(t_token *a, t_token *b)
{
	bool	tmp_echo;

	tmp_echo = a->echo;
	a->echo = b->echo;
	b->echo = tmp_echo;
}

void	swap_in_q(t_token *a, t_token *b)
{
	bool	tmp_in_q;

	tmp_in_q = a->in_q;
	a->in_q = b->in_q;
	b->in_q = tmp_in_q;
}

void	swap_empty(t_token *a, t_token *b)
{
	bool	tmp_empty;

	tmp_empty = a->empty;
	a->empty = b->empty;
	b->empty = tmp_empty;
}
