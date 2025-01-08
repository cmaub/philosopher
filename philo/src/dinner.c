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

static void	eat(t_philo *philo)
{
	// prendre les fourchettes
	handle_mutex(&philo->first_fork->fork, LOCK);
	print_status(TAKE_FIRST_FORK, philo);
	handle_mutex(&philo->second_fork->fork, LOCK);
	print_status(TAKE_SECOND_FORK, philo);
	// mettre a jour last meal
	set_long(&philo->philo_mutex, &philo->last_meal_t, gettime(MILLISECOND));
	// mettre a jour meals counter
	philo->nb_meals_eaten++;
	// imprimer status
	print_status(EATING, philo);
	ft_usleep(philo->data->time_to_eat, philo->data);
	if (philo->data->num_meals > 0
		&& philo->nb_meals_eaten == philo->data->num_meals)
		set_bool(&philo->philo_mutex, &philo->full, TRUE);
	handle_mutex(&philo->first_fork->fork, UNLOCK);
	handle_mutex(&philo->second_fork->fork, UNLOCK);
}

static void	think(t_philo *philo)
{
	print_status(THINKING, philo);
}

static void	synchronise_threads(t_data	*data)
{
	//tous les threads passent par la, tant qu'il y en a un qui n'est pas pret reste dans la boucle
	while (!get_bool(&data->data_lock, &data->threads_readies))
		;
}

void	*routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	synchronise_threads(philo->data);
	// donner l'heure du dernier repas
	while (!dinner_finished(philo->data))
	{
		if (philo->full)
			break ;
		//EAT
		eat(philo);
		//SLEEP
		print_status(SLEEPING, philo);
		ft_usleep(philo->data->time_to_sleep, philo->data);
		//THINK
		think(philo);
	}

	return (NULL);
}

void	dinner(t_data *data)
{
	int	i;

	i = -1;
	if (data->num_meals == 0)
		return ;
	else if ( data->num_meals == 1)
		; // todo
	else
	{
		while (++i < data->philo_nbr)
			handle_thread(&data->philos[i].thread_id, routine, &data->philos[i], CREATE);
	}
	data->start_time  = gettime(MILLISECOND);
	printf("debut de la simulation = %ld\n", data->start_time);
	// passer a table une fois que tous les philos sont creer on peut commencer le repas
	set_bool(&data->data_lock, &data->threads_readies, TRUE);
	// print_data(data);
	i = -1;
	while (++i < data->philo_nbr)
		handle_thread(&data->philos[i].thread_id, NULL, NULL, JOIN);
	// si on passe ca tous les philos sont rassasies
}
// if no meal return = 0
	// If only one fonction appart
