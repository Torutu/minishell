/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hex.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 21:11:40 by walnaimi          #+#    #+#             */
/*   Updated: 2024/09/02 21:12:04 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_hex(unsigned long n, int caps, int *check)
{
	int	count;

	count = 0;
	if (n >= 16)
	{
		count += ft_hex((n / 16), caps, check);
		count += ft_hex((n % 16), caps, check);
		if (count == -1)
			return (-1);
	}
	else if ((caps == 0) && (n >= 10 && n <= 15))
		count += ft_char(n + 87, check);
	else if ((caps == 1) && (n >= 10 && n <= 15))
		count += ft_char(n + 55, check);
	else if (n < 10)
		count += ft_nbr(n, check);
	return (count);
}
