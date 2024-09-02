/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nbr.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 21:11:44 by walnaimi          #+#    #+#             */
/*   Updated: 2024/09/02 21:12:03 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_nbr(int n, int *check)
{
	int	count;

	count = 0;
	if (n == -2147483648)
		count += write(1, "-2147483648", 11);
	else if (n < 0)
	{
		count += ft_char('-', check);
		count += ft_nbr(-n, check);
	}
	else if (n >= 10)
	{
		count += ft_nbr(n / 10, check);
		count += ft_char(n % 10 + '0', check);
	}
	else
		count += ft_char(n % 10 + '0', check);
	return (count);
}
