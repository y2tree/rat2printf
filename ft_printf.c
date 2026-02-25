/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylerges <ylerges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 22:05:27 by ylerges           #+#    #+#             */
/*   Updated: 2026/02/25 01:07:13 by ylerges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_putptr(void *ptr)
{
	int				count;
	unsigned long	addr;

	if (!ptr)
		return ((int)write(1, "(nil)", 5));
	addr = (unsigned long)ptr;
	count = (int)write(1, "0x", 2);
	count += ft_putnbr_base(addr, "0123456789abcdef");
	return (count);
}

static int	ft_putnbr(int n)
{
	int		count;
	long	nb;

	count = 0;
	nb = n;
	if (nb < 0)
	{
		count += ft_putchar('-');
		nb = -nb;
	}
	count += ft_putnbr_base(nb, "0123456789");
	return (count);
}

static int	check_format(va_list *args, const char c)
{
	if (c == 'c')
		return (ft_putchar((char)va_arg(*args, int)));
	else if (c == 's')
		return (ft_putstr(va_arg(*args, char *)));
	else if (c == 'd' || c == 'i')
		return (ft_putnbr(va_arg(*args, int)));
	else if (c == 'u')
		return (ft_putnbr_base(va_arg(*args, unsigned int), "0123456789"));
	else if (c == 'x')
		return (ft_putnbr_base(va_arg(*args, unsigned int),
				"0123456789abcdef"));
	else if (c == 'X')
		return (ft_putnbr_base(va_arg(*args, unsigned int),
				"0123456789ABCDEF"));
	else if (c == 'p')
		return (ft_putptr(va_arg(*args, void *)));
	else if (c == '%')
		return (ft_putchar('%'));
	return (0);
}

int	ft_printf(const char *input, ...)
{
	va_list	args;
	int		i;
	int		count;

	if (!input)
		return (-1);
	i = 0;
	count = 0;
	va_start(args, input);
	while (input[i])
	{
		if (input[i] == '%' && input[i + 1])
		{
			i++;
			count += check_format(&args, input[i]);
		}
		else
			count += ft_putchar(input[i]);
		i++;
	}
	va_end(args);
	return (count);
}
