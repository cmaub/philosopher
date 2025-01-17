/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   04_actions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 10:26:48 by cmaubert          #+#    #+#             */
/*   Updated: 2025/01/17 15:56:21 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	took_forks(t_philo *philo)
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
			|| philo->data->num_meals == -1)
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
		print_status(SLEEPING, philo);
		ft_usleep(philo->data->time_to_sleep, philo->data);
	}
}
