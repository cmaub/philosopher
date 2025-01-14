/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 10:38:14 by cmaubert          #+#    #+#             */
/*   Updated: 2025/01/14 12:22:22 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	threads_running(t_mtx *mutex, long *threads_running, long philo_nbr)
{
	handle_mutex(mutex, LOCK);
	if (*threads_running == philo_nbr)
	{
		return (TRUE);
	}
	handle_mutex(mutex, UNLOCK);
	return (FALSE);
}

int		philo_died(t_philo *philo)
{
	long	elapsed;

	elapsed = gettime(MILLISECOND) - get_long(&philo->philo_mutex, &philo->last_meal_t);
	if (elapsed > get_long(&philo->philo_mutex, &philo->data->time_to_die))
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
	
// monitor -> arrete le programme quand : tous le philos ont mangé le numbre de repas a manger (num_eat) ou quand ils ont un temps supérieur a time_to_die entre deux repas
void *monitor_routine(void *arg)
{
	int i;
	long current_time;
	long last_meal_time;
	long elapsed;
	t_data *data = (t_data *)arg;

	// while (threads_running(&data->data_lock,
	// 	&data->threads_running_nb, data->philo_nbr) == FALSE)
	// {
	// 	if (threads_running(&data->data_lock,
	// 		&data->threads_running_nb, data->philo_nbr) == TRUE)
	// 	{

	// 		printf("\n\n\n******************break\n\n\n");
	// 		printf("threads_running_nb = %ld\n", data->threads_running_nb);
	// 		break ;
	// 	}
	// }
	// 
	synchronise_threads(data);
	while (dinner_end(data) == FALSE)
	{
		i = -1;
		while (++i < data->philo_nbr && !dinner_end(data)) // revoir peut-etre la condition
		{
			current_time = gettime(MILLISECOND);
			handle_mutex(&data->philos[i].last_meal_lock, LOCK);
			last_meal_time = data->philos[i].last_meal_t;
			handle_mutex(&data->philos[i].last_meal_lock, UNLOCK);
			elapsed = current_time - last_meal_time;
			if (elapsed > data->time_to_die) // et que philo nest pas full ++ sil est full
			{
				handle_mutex(&data->end_lock, LOCK); // verifier le lock
				data->end = TRUE ;
				handle_mutex(&data->end_lock, UNLOCK);
				print_status(DIED, &data->philos[i]);
				break;
			}
			if (all_philos_full(data) == TRUE)
			{
				handle_mutex(&data->end_lock, LOCK);
				data->end = TRUE;
				printf("*** Philosophes rassasies !\n");
				handle_mutex(&data->end_lock, UNLOCK);
				break; //??????
			}
		}
		usleep(10); // Sleep pour reduire l'activite du CPU ? a tester avec diff valeurs
	}
	return (NULL);
}
