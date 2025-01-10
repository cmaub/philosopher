/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 10:38:14 by cmaubert          #+#    #+#             */
/*   Updated: 2025/01/10 13:29:00 by cmaubert         ###   ########.fr       */
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

	// if (get_bool(&philo->philo_mutex, &philo->full))
	// 	return (FALSE);
	elapsed = gettime(MILLISECOND) - get_long(&philo->philo_mutex, &philo->last_meal_t);
	// handle_mutex(&philo->philo_mutex, LOCK);
	// printf("elapsed = %ld\n", elapsed);
	// printf("philo->data->time_to_die = %ld\n", philo->data->time_to_die);
	// printf("philo->last_meal_t = %ld\n", philo->last_meal_t);
	// handle_mutex(&philo->philo_mutex, UNLOCK);
	if (elapsed > philo->data->time_to_die)
		return (TRUE);
	else
		return (FALSE);
}

void	*monitor_routine2(void *arg)
{
	// (void)data;
	t_data	*data;
	int		i;
	// long	elapsed;

	data = (t_data *)arg;
	printf("COUCOU");
	// // spinlock tant que tous les thread ne sont pas arrives ici
	while (threads_running(&data->data_lock,
		&data->threads_running_nb, data->philo_nbr) == FALSE)
	{
		printf("COUCOU");
		if (threads_running(&data->data_lock,
			&data->threads_running_nb, data->philo_nbr) == TRUE)
		{
			printf("\n\n\n******************break\n\n\n");
			break ;
		}
	}
	// handle_mutex(&data->data_lock, LOCK);
	// printf("*****threads_running = %ld\n", data->threads_running_nb);
	// handle_mutex(&data->data_lock, UNLOCK);
	// PROBLEME -> on ne rentre jamais dans la boucle + data race
	while (data->end == FALSE)
	{
		i = -1;
		while(++i < data->philo_nbr && data->end == FALSE)
		{
			if (philo_died(data->philos + i))
			{
				set_bool(&data->data_lock, &data->end, TRUE);
				print_status(DIED, data->philos + i);
			}
			if (data->num_meals == data->philos->nb_meals_eaten)
			{
				set_bool(&data->data_mega_lock, &data->end, TRUE);
				printf("*** philos ont tous manges\n");
			}
		}
	}
	// while (!data->end)
	// {
	// 	handle_mutex(&data->data_lock, LOCK);
	// 	// printf("gettime(MILLISECOND) = %ld\n", gettime(MILLISECOND));
	// 	// printf("get_long(&philo->philo_mutex, &philo->last_meal_t) = %ld", get_long(&data->philos->philo_mutex, &data->philos->last_meal_t));
	// 	// printf("elapsed = %ld\n", elapsed);
	// 	// printf("philo->data->time_to_die = %ld\n", philo->data->time_to_die);
	// 	i = -1;
	// 	while (++i < data->philo_nbr && !dinner_finished(data))
	// 	{

	// 		// last_meal_time = get_long(&data->philos[i]->philo_mutex, &data->philos[i]->last_meal_t);
	// 		elapsed = gettime(MILLISECOND) - get_long(&data->philos->philo_mutex, &data->philos[i].last_meal_t);
	// 		if (elapsed >= data->time_to_die)
	// 		{
	// 			set_bool(&data->data_mega_lock, &data->end, TRUE);
	// 			print_status(DIED, data->philos + i);

	// 		}
	// 		printf("1 ICI\n");
	// 		set_bool(&data->data_mega_lock, &data->end, TRUE);
	// 		printf("2 ICI\n");

	// 		if (data->num_meals == data->philos->nb_meals_eaten)
	// 		{
	// 			set_bool(&data->data_mega_lock, &data->end, TRUE);
	// 			printf("***philos ont tous manges\n");
	// 		}
	// 	}
	// 	handle_mutex(&data->data_lock, UNLOCK);
	// }
	return (NULL);
}
