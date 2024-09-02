/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unsig_nbr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 21:11:58 by walnaimi          #+#    #+#             */
/*   Updated: 2024/09/02 21:12:00 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_unsig_nbr(unsigned int n, int *check)
{
	int	count;

	count = 0;
	if (n >= 10)
	{
		count += ft_nbr(n / 10, check);
		count += ft_char(n % 10 + '0', check);
	}
	else
		count += ft_char(n % 10 + '0', check);
	return (count);
}
