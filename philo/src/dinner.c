/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:54:53 by cmaubert          #+#    #+#             */
/*   Updated: 2025/01/08 15:39:38 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// ./philo num_of_philo time_to_die time_to_eat time_to_sleep [num_eat]
// ./philo 5 800 200 200 [5]

// ./philo 5 100 500 1000 3 : pb le philo ne meurt pas tout de suite

int	all_philos_full(t_data *data)
{
	int i;

	i = 0;
	handle_mutex(&data->full_lock, LOCK);
	while (i < data->philo_nbr)
	{
		if (data->philos[i].full == FALSE)
		{
			handle_mutex(&data->full_lock, UNLOCK);
			return (FALSE);
		}
		i++;
	}
	if (i == data->philo_nbr)
	{
		handle_mutex(&data->full_lock, UNLOCK);
		return (TRUE);
	}
	handle_mutex(&data->full_lock, UNLOCK);
	return (FALSE);
}

void	synchronise_threads(t_data	*data)
{
	while (!get_bool(&data->data_lock, &data->philo_readies))
		usleep(10);
}

void	took_forks(t_philo *philo)
{
	handle_mutex(&philo->first_fork->fork, LOCK);
	if (!all_philos_full(philo->data) && !dinner_end(philo->data))
		print_status(TAKE_FIRST_FORK, philo);
	handle_mutex(&philo->second_fork->fork, LOCK);
	if (!all_philos_full(philo->data) && !dinner_end(philo->data))
		print_status(TAKE_SECOND_FORK, philo);
}

void	eat(t_philo *philo)
{
	if (((!all_philos_full(philo->data) && philo->data->num_meals > 0)
		|| philo->data->num_meals == -1) // attention au -1 --> revoir peut etre la valeur
		&& !dinner_end(philo->data))
	{
		took_forks(philo);
		if (!all_philos_full(philo->data) && !dinner_end(philo->data))
		{
			increase_nb_meals(philo);
			print_status(EATING, philo);
			set_last_meal(philo);
			ft_usleep(philo->data->time_to_eat, philo->data);
		}
		handle_mutex(&philo->first_fork->fork, UNLOCK);
		handle_mutex(&philo->second_fork->fork, UNLOCK);
	}
	handle_mutex(&philo->data->full_lock, LOCK);
	if (philo->data->num_meals > 0
		&& philo->nb_meals_eaten == philo->data->num_meals)		
			philo->full = TRUE;
	handle_mutex(&philo->data->full_lock, UNLOCK);	
}

void	think(t_philo *philo) //revoir idee de desynchronisation
{
	if (!all_philos_full(philo->data) && !dinner_end(philo->data))
	{
		if (!all_philos_full(philo->data) && !dinner_end(philo->data))
			print_status(THINKING, philo);
		if (philo->data->philo_nbr % 2 != 0)
			ft_usleep(philo->data->time_to_eat * 0.5, philo->data);
	}
}

void	ft_sleep(t_philo *philo)
{
	if (!all_philos_full(philo->data) && !dinner_end(philo->data))
	{
		print_status(SLEEPING, philo);
		ft_usleep(philo->data->time_to_sleep, philo->data);
	}
}

void	*philo_routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	synchronise_threads(philo->data); //spinlock a voir si je remplace par un mutex
	set_last_meal(philo);
	increase_thr_running_nb(&philo->data->data_lock, &philo->data->threads_running_nb);
	if (philo->id % 2 != 0)
		ft_usleep(philo->data->time_to_eat / 2, philo->data);
	while (!dinner_end(philo->data) && !all_philos_full(philo->data))
	{
		eat(philo);
		ft_sleep(philo);
		think(philo);
	}
	return (NULL);
}

void	*lonely_philo_routine(void *data)
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

void	dinner(t_data *data)
{
	int	i;

	i = -1;
	if (data->philo_nbr == 1)
	{
		if (!handle_thread(&data->philos[0].thread_id, lonely_philo_routine, &data->philos[0], CREATE))
			return ;
	}
	else
	{
		while (++i < data->philo_nbr)
		{
			if (!handle_thread(&data->philos[i].thread_id, philo_routine, &data->philos[i], CREATE))
			{
				while (i > 0)
					handle_thread(&data->philos[i].thread_id, NULL, NULL, JOIN);
				return ;
			}
		}
	}
	if (!handle_thread(&data->monitor, monitor_routine, data, CREATE))
	{
		while (i > 0)
			handle_thread(&data->philos[i].thread_id, NULL, NULL, JOIN);
		return ;
	}
	data->start_time = gettime();
	set_bool(&data->data_lock, &data->philo_readies, TRUE);
	handle_thread(&data->monitor, NULL, NULL, JOIN);
	i = -1;
	while (++i < data->philo_nbr)
		handle_thread(&data->philos[i].thread_id, NULL, NULL, JOIN);
	set_bool(&data->end_lock, &data->end, TRUE);
}

