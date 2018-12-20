/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchicote <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/26 13:17:14 by cchicote          #+#    #+#             */
/*   Updated: 2015/11/26 17:15:05 by cchicote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

void	ft_putendl(char const *s)
{
	int index;

	index = 0;
	while (s[index] != '\0')
	{
		write(1, &s[index], 1);
		index++;
	}
	write(1, "\n", 1);
}
