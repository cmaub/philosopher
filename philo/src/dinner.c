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

void	increase_nb_meals(t_philo *philo)
{
	handle_mutex(&philo->philo_mutex, LOCK);
	philo->nb_meals_eaten = philo->nb_meals_eaten + 1;
	handle_mutex(&philo->philo_mutex, UNLOCK);
}

void	synchronise_threads(t_data	*data)
{
	//tous les threads passent par la, tant qu'il y en a un qui n'est pas pret reste dans la boucle
	while (!get_bool(&data->data_lock, &data->threads_readies))
		;
}

void	took_forks(t_philo *philo)
{
	handle_mutex(&philo->first_fork->fork, LOCK);
	// printf("philo->id = %d, took 1st fork = %d\n", philo->id, philo->first_fork->fork_id);
	if (!all_philos_full(philo->data) && !dinner_end(philo->data))
		print_status(TAKE_FIRST_FORK, philo);
	handle_mutex(&philo->second_fork->fork, LOCK);
	// printf("philo->id = %d, took 2nd fork = %d\n", philo->id, philo->second_fork->fork_id);
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
			print_status(EATING, philo);
		set_last_meal(philo);
		increase_nb_meals(philo);
		ft_usleep(philo->data->time_to_eat, philo->data);
		handle_mutex(&philo->first_fork->fork, UNLOCK);
		handle_mutex(&philo->second_fork->fork, UNLOCK);
	}
	handle_mutex(&philo->data->full_lock, LOCK);
	if (philo->data->num_meals > 0
		&& philo->nb_meals_eaten == philo->data->num_meals)		
			philo->full = TRUE;
	handle_mutex(&philo->data->full_lock, UNLOCK);	
}

void	think(t_philo *philo)
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
		if (!all_philos_full(philo->data) && !dinner_end(philo->data))
			print_status(SLEEPING, philo);
		ft_usleep(philo->data->time_to_sleep, philo->data);
	}
}

// PB The value specified by mutex is invalid ./philo 199 402 200 200
// PB quand philo doit mourrir il affiche des fois la mort plusieurs fois
void	*philo_routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	synchronise_threads(philo->data); //spinlock a voir si je remplace par un mutex
	set_last_meal(philo);
	increase_long(&philo->data->data_lock, &philo->data->threads_running_nb);
	if (philo->id % 2 == 0)
		ft_usleep(philo->data->time_to_eat / 2, philo->data);
	// if (philo->id == philo->data->philo_nbr)
	// 	ft_usleep(200, philo->data);
		// ft_usleep(400, philo->data);
	while (!dinner_end(philo->data) && !all_philos_full(philo->data))
	{
		// rajouter une condition qui verifie que le 
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
	increase_long(&philo->data->data_lock, &philo->data->threads_running_nb);;
	print_status(TAKE_FIRST_FORK, philo);
	while (!dinner_end(philo->data))
		usleep(200);
	return (NULL);
}

void	dinner(t_data *data)
{
	int	i;

	i = -1;
	// print_data(data);
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
	data->start_time = gettime(MILLISECOND);
	set_bool(&data->data_lock, &data->threads_readies, TRUE);
	handle_thread(&data->monitor, NULL, NULL, JOIN); // leak si mis apres le join des philos
	i = -1;
	while (++i < data->philo_nbr)
		handle_thread(&data->philos[i].thread_id, NULL, NULL, JOIN);
	set_bool(&data->end_lock, &data->end, TRUE);
}

// void *monitor_routine(void *arg)
// {
// 	int i;
// 	long current_time;
// 	long last_meal_time;
// 	long elapsed;
// 	int	end;

// 	t_data *data = (t_data *)arg;
// 	end = get_bool(&data->data_lock, &data->end);
// 	while (end == FALSE)
// 	{
// 		end = get_bool(&data->data_lock, &data->end);
// 		if (end == TRUE)
// 			break ;
// 		i = -1;
// 		while (++i <data->philo_nbr)
// 		{
// 			current_time = gettime(MILLISECOND);
// 			last_meal_time = get_long(&data->philos[i].philo_mutex, &data->philos[i].last_meal_t);
// 			elapsed = current_time - last_meal_time;
// 			if (elapsed >= data->time_to_die)
// 			{
// 				set_bool(&data->philos[i].philo_mutex, &data->end, TRUE);
// 				print_status(DIED, &data->philos[i]);
// 				break;
// 			}
// 			if (all_philos_full(data) == TRUE)
// 			{
// 				set_bool(&data->data_mega_lock, &data->end, TRUE);
// 				printf("*** Philosophes rassasies !\n");
// 				break;
// 			}
// 		}
// 		usleep(1000); // Sleep pour reduire l'activite du CPU ?
// 	}
// 	return NULL;
// }
