/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 18:56:33 by cmaubert          #+#    #+#             */
/*   Updated: 2025/01/20 17:50:11 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	assign_fork(t_philo *philo, t_fork *forks, int position)
{
	int	philo_nbr;

	philo_nbr = philo->data->philo_nbr;
	if (philo->nb % 2 != 0)
	{
		philo->first_fork = &forks[(position + 1) % philo_nbr];
		philo->second_fork = &forks[position];
	}
	if (philo->nb % 2 == 0)
	{
		philo->first_fork = &forks[position];
		philo->second_fork = &forks[(position + 1) % philo_nbr];
	}
}

static int	philo_initializer(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->philo_nbr)
	{
		data->philos[i].nb = i + 1;
		data->philos[i].full = FALSE;
		data->philos[i].nb_meals_eaten = 0;
		data->philos[i].last_meal_t = gettime();
		data->philos[i].data = data;
		if (!handle_mutex(&data->philos[i].meal_lock, INIT))
		{
			while (i > 0)
				handle_mutex(&data->philos[i].meal_lock, DESTROY);
			handle_mutex(&data->data_lock, DESTROY);
			handle_mutex(&data->end_lock, DESTROY);
			handle_mutex(&data->full_lock, DESTROY);
			handle_mutex(&data->print_lock, DESTROY);
			while (i++ < data->philo_nbr)
				handle_mutex(&data->forks[i].fork, DESTROY);
			return (FALSE);
		}
		assign_fork(&data->philos[i], data->forks, i);
	}
	return (TRUE);
}

static int	init_forks(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->philo_nbr)
	{
		if (!handle_mutex(&data->forks[i].fork, INIT))
		{
			while (i > 0)
				handle_mutex(&data->forks[i].fork, DESTROY);
			handle_mutex(&data->data_lock, DESTROY);
			handle_mutex(&data->end_lock, DESTROY);
			handle_mutex(&data->full_lock, DESTROY);
			handle_mutex(&data->print_lock, DESTROY);
			return (FALSE);
		}
		data->forks[i].fork_id = i;
	}
	return (TRUE);
}

int	init_data_mtx(t_data *data)
{
	if (!handle_mutex(&data->data_lock, INIT))
		return (FALSE);
	if (!handle_mutex(&data->end_lock, INIT))
	{
		handle_mutex(&data->data_lock, DESTROY);
		return (FALSE);
	}
	if (!handle_mutex(&data->full_lock, INIT))
	{
		handle_mutex(&data->data_lock, DESTROY);
		handle_mutex(&data->end_lock, DESTROY);
		return (FALSE);
	}
	if (!handle_mutex(&data->print_lock, INIT))
	{
		handle_mutex(&data->data_lock, DESTROY);
		handle_mutex(&data->end_lock, DESTROY);
		handle_mutex(&data->full_lock, DESTROY);
		return (FALSE);
	}
	if (!handle_mutex(&data->time_lock, INIT))
	{
		handle_mutex(&data->data_lock, DESTROY);
		handle_mutex(&data->end_lock, DESTROY);
		handle_mutex(&data->full_lock, DESTROY);
		handle_mutex(&data->time_lock, DESTROY);
		return (FALSE);
	}
	return (TRUE);
}

int	data_initializer(t_data *data)
{
	data->end = FALSE;
	data->philo_readies = FALSE;
	data->threads_running_nb = 0;
	data->nb_full = 0;
	data->philos = try_malloc(sizeof(t_philo) * data->philo_nbr);
	if (!data->philos)
		return (FALSE);
	data->forks = try_malloc(sizeof(t_fork) * data->philo_nbr);
	if (!data->forks)
		return (free(data->philos), FALSE);
	data->start_time = gettime();
	if (!init_data_mtx(data))
		return (FALSE);
	if (!init_forks(data))
		return (FALSE);
	if (!philo_initializer(data))
		return (FALSE);
	return (TRUE);
}
