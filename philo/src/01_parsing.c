/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   01_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 16:40:34 by cmaubert          #+#    #+#             */
/*   Updated: 2025/01/17 12:47:02 by cmaubert         ###   ########.fr       */
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

int	is_valid_input(char *str)
{
	int		len;

	while (is_space(*str))
		str++;
	if (*str == '+')
		str++;
	else if (*str == '-')
		return (FALSE);
	while (*str)
	{
		if (!ft_isalnum(*str))
			return (FALSE);
		str++;
	}
	len = 0;
	while (str[len])
		len++;
	return (TRUE);
}

int	check_all_av(char **av)
{
	int	i;

	i = 1;
	while (av[i])
	{
		if (!is_valid_input(av[i]))
			return (FALSE);
		i++;
	}
	return (TRUE);
}

int	parse(t_data *data, char **av)
{
	if (!check_all_av(av))
		return (printf("Error with the parsing of the data\n"), FALSE);
	data->philo_nbr = ft_atol(av[1]);
	if (data->philo_nbr > 200 || data->philo_nbr <= 0)
		return (printf("philo number must be less than 200 and more than 0\n"),
			FALSE);
	data->time_to_die = ft_atol(av[2]);
	data->time_to_eat = ft_atol(av[3]);
	data->time_to_sleep = ft_atol(av[4]);
	if (data->time_to_die < 60 || data->time_to_eat < 60
		|| data->time_to_sleep < 60)
	{
		printf("The value is superior at INT_MAX and major than 60ms\n");
		return (FALSE);
	}
	if (av[5])
	{
		data->num_meals = ft_atol(av[5]);
		if (data->num_meals <= 0)
			return (printf("number of meals must be more than 0\n"), FALSE);
	}
	else
		data->num_meals = -1;
	return (TRUE);
}
