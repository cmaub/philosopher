/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 16:40:34 by cmaubert          #+#    #+#             */
/*   Updated: 2025/01/15 17:11:18 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// ./philo num_of_philo time_to_die time_to_eat time_to_sleep [num_eat]
// ./philo 5 800 200 200 [5]

static int	is_space(char c)
{
	if (c && (c == ' ' || c == '\t'
			|| c == '\n' || c == '\r'
			|| c == '\v' || c == '\f'))
		return (1);
	return (0);
}

static int	ft_isalnum(int c)
{
	if (c >= '0' && c <= '9')
		return (TRUE);
	else
		return (FALSE);
}

static void	is_valid_input(char *str)
{
	int		len;

	while (is_space(*str))
		str++;
	if (*str == '+')
		str++;
	else if (*str == '-')
		str_exit("Accept only positive values", NULL);
	while (*str)
	{
		if (!ft_isalnum(*str))
			str_exit("Accept only digits", NULL);
		str++;
	}
	len = 0; 
	while (str[len])
		len++;
}

static long	ft_atol(char *str)
{
	long	num;
	int		i;

	num = 0;
	i = 0;
	is_valid_input(str);
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (num > (INT_MAX / 10))
			str_exit("The value is superior at INT_MAX", NULL);
		num = (num * 10) + (str[i] - '0');
		i++;
	}
	return (num);
}

void	parse(t_data *data, char **av)
{
	data->philo_nbr = ft_atol(av[1]);
	if (data->philo_nbr > 200 || data->philo_nbr <= 0 )
		str_exit("philo number must be less than 200 and more than 0", NULL); //pourquoi ? verifier
	data->time_to_die = ft_atol(av[2]);
	data->time_to_eat = ft_atol(av[3]);
	data->time_to_sleep = ft_atol(av[4]);
	if (data->time_to_die < 60 //pourquoi ? verifier
		|| data->time_to_eat < 60
			|| data->time_to_sleep < 60)
			str_exit("Timestamps must be major than 60ms", NULL);
	if (av[5])
	{
		data->num_meals = ft_atol(av[5]);
		if (data->num_meals <= 0) //pourquoi ? verifier
			str_exit("number of meals must be more tan 0", NULL);
	}
	else
		data->num_meals = -1;
}
