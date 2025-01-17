/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 16:34:03 by cmaubert          #+#    #+#             */
/*   Updated: 2025/01/17 10:30:45 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_destroy_mutex(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->philo_nbr)
	{
		handle_mutex(&data->philos[i].meal_lock, DESTROY);
		handle_mutex(&data->forks[i].fork, DESTROY);
	}
	handle_mutex(&data->data_lock, DESTROY);
	handle_mutex(&data->print_lock, DESTROY);
	handle_mutex(&data->end_lock, DESTROY);
	handle_mutex(&data->full_lock, DESTROY);
	free(data->philos);
	free(data->forks);
}

void	free_struct(char *error, t_data *data)
{
	printf("%s\n", error);
	if (data)
	{
		if (data->philos)
			free(data->philos);
		if (data->forks)
			free(data->forks);
	}
}

void	*try_malloc(size_t size)
{
	void	*p;

	p = malloc(size);
	if (p == NULL)
		return (printf("fatal: out of memory\n"), NULL);
	memset(p, 0, size);
	return (p);
}

long	ft_atol(char *str)
{
	long	num;
	int		i;

	num = 0;
	i = 0;

	while (str[i] >= '0' && str[i] <= '9')
	{
		if (num > (INT_MAX / 10))
			return (-1);
		num = (num * 10) + (str[i] - '0');
		i++;
	}
	return (num);
}
