/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 16:34:03 by cmaubert          #+#    #+#             */
/*   Updated: 2025/01/15 17:02:49 by cmaubert         ###   ########.fr       */
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
	i = -1;
	handle_mutex(&data->data_lock, DESTROY);
	handle_mutex(&data->print_lock, DESTROY);
	handle_mutex(&data->end_lock, DESTROY);
	handle_mutex(&data->full_lock, DESTROY);
	free(data->philos);
	free(data->forks);
}

void	str_exit(char *error, t_data *data)
{
	printf("%s\n", error);
	if (data)
	{
		if (data->philos)
			free(data->philos);
		if (data->forks)
			free(data->forks);
	}
	//comment verifier si mtx existe et quil faut les destroy ?
	exit(EXIT_FAILURE);
}

void	*try_malloc(size_t size, t_data *data)
{
	void	*p;

	p = malloc(size);
	if (p == NULL)
		str_exit("fatal: out of memory", data); //
	memset(p, 0, size);
	return (p);
}
