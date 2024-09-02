/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 21:08:53 by walnaimi          #+#    #+#             */
/*   Updated: 2024/09/02 21:10:41 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strchr(const char *s, int c)
{
	char	ch;
	char	*str;
	int		i;

	i = 0;
	str = (char *)s;
	ch = (char)c;
	while (str[i] != ch)
	{
		if (str[i] == '\0')
		{
			return (0);
		}
		i++;
	}
	return (&str[i]);
}
