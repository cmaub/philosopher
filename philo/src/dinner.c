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

static void	eat(t_philo *philo)
{
	if (all_philos_full(philo->data) == TRUE)
	{ // mettre a jour end
		handle_mutex(&philo->data->end_lock, LOCK);
		philo->data->end = TRUE ;
		handle_mutex(&philo->data->end_lock, UNLOCK);
		printf("eat = all_philo_full");
		return ;
	}
	handle_mutex(&philo->first_fork->fork, LOCK);
	print_status(TAKE_FIRST_FORK, philo);
	handle_mutex(&philo->second_fork->fork, LOCK);
	print_status(TAKE_SECOND_FORK, philo);
	set_last_meal(philo);
	increase_long(&philo->philo_mutex, &philo->nb_meals_eaten);
	print_status(EATING, philo);
	ft_usleep(philo->data->time_to_eat, philo->data);
	handle_mutex(&philo->data->full_lock, LOCK);
	if (philo->data->num_meals > 0
		&& philo->nb_meals_eaten == philo->data->num_meals)		
			philo->full = TRUE;
	handle_mutex(&philo->data->full_lock, UNLOCK);		// checker si tous full
	handle_mutex(&philo->first_fork->fork, UNLOCK);
	handle_mutex(&philo->second_fork->fork, UNLOCK);
}

// revoir la fonction THINK
static void	think(t_philo *philo/*, int bool*/)
{
	long	t_to_think;
	long	t_to_sleep;
	long	t_to_eat;

	// if (bool == FALSE)
	// {
	// 	print_status(THINKING, philo);
	// }
	// si le nb est pair aucun philo ne peux manger plus de 2 fois sans penser
	// donc pas besoin d'ajouter du temps
	if (all_philos_full(philo->data) == TRUE)
	{
			printf("**** think = all_philo_full");
			return ;
	}
	if (philo->data->philo_nbr % 2 == 0)
	{
		print_status(THINKING, philo);
		return ;
	}
	// philo impair
	t_to_eat = philo->data->time_to_eat;
	t_to_sleep = philo->data->time_to_sleep;
	t_to_think = t_to_eat * 2 - t_to_sleep;
	if (t_to_think < 0)
		t_to_think = 0;
	ft_usleep(t_to_think * 0.50, philo->data);
	print_status(THINKING, philo);
}

static void	synchronise_threads(t_data	*data)
{
	//tous les threads passent par la, tant qu'il y en a un qui n'est pas pret reste dans la boucle
	while (!get_bool(&data->data_lock, &data->threads_readies))
		;
}

void	*lonely_philo_routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	handle_mutex(&philo->philo_mutex, LOCK);
	dprintf(2, "lonely_philo_routine\n");
	handle_mutex(&philo->philo_mutex, UNLOCK);
	synchronise_threads(philo->data);
	set_last_meal(philo);
	// increase_long(&philo->data->data_lock, &philo->data->threads_running_nb);;
	print_status(TAKE_FIRST_FORK, philo);
	while (!dinner_finished(philo->data))
		usleep(200);
	return (NULL);

}

// PB The value specified by mutex is invalid ./philo 199 402 200 200
void	*philo_routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	//spinlock
	synchronise_threads(philo->data);
	set_last_meal(philo);
	increase_long(&philo->data->data_lock, &philo->data->threads_running_nb);
	// desyncroniser les philos ? pour lancer les pairs et impair differemment
	// desynchronise_threads(philo);
	while (dinner_finished(philo->data) == FALSE)
	{
		eat(philo);
		if (all_philos_full(philo->data) == TRUE || dinner_finished(philo->data) == TRUE)
		{
			printf("***sleep = all_philo_full 1\n");
			break ;
		}
		print_status(SLEEPING, philo);
		ft_usleep(philo->data->time_to_sleep, philo->data);
		if (all_philos_full(philo->data) == TRUE || dinner_finished(philo->data) == TRUE)
		{
			printf("***sleep = all_philo_full 2\n");
			break ;
		}
		think(philo);/*, FALSE*/
	}
	return (NULL);
}

void	dinner(t_data *data)
{
	int	i;

	i = -1;
	if (data->num_meals == 0)
		return ;
	if (data->philo_nbr == 1)
		handle_thread(&data->philos[0].thread_id, lonely_philo_routine, &data->philos[0], CREATE);
	else
	{
		while (++i < data->philo_nbr)
			handle_thread(&data->philos[i].thread_id, philo_routine, &data->philos[i], CREATE);
	}
	// moniteur qui check la mort des philo
	handle_thread(&data->monitor, monitor_routine, data, CREATE);
	// une fois que tous les philos sont crees on peut commencer le repas, mise a jout de threads_readies
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
