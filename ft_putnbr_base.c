/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylerges <ylerges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 13:46:00 by yneuillem         #+#    #+#             */
/*   Updated: 2026/02/12 22:05:50 by ylerges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putnbr_base(unsigned long nbr, char *base)
{
	int	count;
	int	base_len;

	count = 0;
	base_len = ft_strlen(base);
	if (nbr >= (unsigned long)base_len)
		count += ft_putnbr_base(nbr / base_len, base);
	count += ft_putchar(base[nbr % base_len]);
	return (count);
}
