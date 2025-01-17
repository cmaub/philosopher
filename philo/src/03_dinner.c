/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   03_dinner.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:54:53 by cmaubert          #+#    #+#             */
/*   Updated: 2025/01/17 16:17:39 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	threads_running(t_mtx *mutex, long *threads_running_nbr, long philo_nbr)
{
	handle_mutex(mutex, LOCK);
	if (*threads_running_nbr == philo_nbr)
		return (TRUE);
	handle_mutex(mutex, UNLOCK);
	return (FALSE);
}

void	*philo_routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	synchronise_threads(philo->data);
	set_last_meal(philo);
	incr_thr_run_nb(&philo->data->data_lock, &philo->data->threads_running_nb);
	if (philo->nb % 2 != 0)
		ft_usleep(philo->data->time_to_eat / 2, philo->data);
	while (!dinner_end(philo->data) && !all_philos_full(philo->data))
	{
		eat(philo);
		ft_sleep(philo);
		think(philo);
	}
	return (NULL);
}

void	*lonely_philo(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	synchronise_threads(philo->data);
	set_last_meal(philo);
	print_status(TAKE_FIRST_FORK, philo);
	while (!dinner_end(philo->data))
		usleep(200);
	return (NULL);
}

int	create_philos_threads(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->philo_nbr)
	{
		if (!handle_thread(&data->philos[i].id,
				philo_routine, &data->philos[i], CREATE))
		{
			while (i > 0)
				handle_thread(&data->philos[i].id, NULL, NULL, JOIN);
			return (FALSE);
		}
	}
	if (!handle_thread(&data->monitor, monitor_routine, data, CREATE))
	{
		while (i > 0)
			handle_thread(&data->philos[i].id, NULL, NULL, JOIN);
		return (FALSE);
	}
	return (TRUE);
}

void	dinner(t_data *data)
{
	int	i;

	i = -1;
	if (data->philo_nbr == 1)
	{
		if (!handle_thread(&data->philos[0].id, lonely_philo, &data->philos[0], CREATE))
			return ;
		if (!handle_thread(&data->monitor, monitor_routine, data, CREATE))
		{
			handle_thread(&data->philos[0].id, NULL, NULL, JOIN);
			return ;
		}
	}
	else
	{
		if (!create_philos_threads(data))
			return ;
	}
	data->start_time = gettime();
	set_bool(&data->data_lock, &data->philo_readies, TRUE);
	handle_thread(&data->monitor, NULL, NULL, JOIN);
	i = -1;
	while (++i < data->philo_nbr)
		handle_thread(&data->philos[i].id, NULL, NULL, JOIN);
}
