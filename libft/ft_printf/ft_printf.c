/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 21:11:52 by walnaimi          #+#    #+#             */
/*   Updated: 2024/09/02 21:12:02 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_printf(const char *format, ...)
{
	int		count;
	int		check;
	va_list	args;

	check = 0;
	count = 0;
	va_start(args, format);
	count = format_check(format, args, &check);
	if (check == -1)
	{
		va_end(args);
		return (-1);
	}
	va_end(args);
	return (count);
}
