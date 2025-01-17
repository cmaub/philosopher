/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   05_monitor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 10:38:14 by cmaubert          #+#    #+#             */
/*   Updated: 2025/01/17 12:31:45 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	dinner_end(t_data *data)
{		
	handle_mutex(&data->end_lock, LOCK);
	if (data->end == TRUE)
		return (handle_mutex(&data->end_lock, UNLOCK), TRUE);
	handle_mutex(&data->end_lock, UNLOCK);
	return (FALSE);
}

int		philo_died(t_philo *philo)
{
	long	elapsed;

	elapsed = gettime() - get_long(&philo->meal_lock, &philo->last_meal_t);
	if (elapsed > philo->data->time_to_die)
		return (TRUE);
	else
		return (FALSE);
}

int	is_full(t_philo *philo)
{
	handle_mutex(&philo->data->full_lock, LOCK);
	if (philo->full == TRUE)
	{
		handle_mutex(&philo->data->full_lock, UNLOCK);
		return (TRUE);
	}
	handle_mutex(&philo->data->full_lock, UNLOCK);
	return (FALSE);
}

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

void	*monitor_routine(void *arg)
{
	int i;
	t_data *data = (t_data *)arg;

	while (dinner_end(data) == FALSE)
	{
		i = -1;
		while (++i < data->philo_nbr) // revoir peut-etre la condition
		{
			if (philo_died(&data->philos[i]))
			{
				handle_mutex(&data->end_lock, LOCK);
				data->end = TRUE ;
				handle_mutex(&data->end_lock, UNLOCK);
				print_status(DIED, &data->philos[i]);
				break;
			}
			if (all_philos_full(data))
			{
				handle_mutex(&data->end_lock, LOCK);
				data->end = TRUE;
				// printf("*** Philosophes rassasies !\n"); // imprimer ? lock avec le print mtx
				handle_mutex(&data->end_lock, UNLOCK);
				break;
			}
		}
		usleep(10);
	}
	return (NULL);
}
