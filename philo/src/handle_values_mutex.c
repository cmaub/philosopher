/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_values_mutex.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 15:24:02 by cmaubert          #+#    #+#             */
/*   Updated: 2025/01/17 10:31:47 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	increase_nb_meals(t_philo *philo)
{
	handle_mutex(&philo->meal_lock, LOCK);
	philo->nb_meals_eaten = philo->nb_meals_eaten + 1;
	handle_mutex(&philo->meal_lock, UNLOCK);
}

void	set_bool(t_mtx *mutex, int *dest, int value)
{
	handle_mutex(mutex, LOCK);
	*dest = value;
	handle_mutex(mutex, UNLOCK);
}

int	get_bool(t_mtx *mutex, int *value)
{
	int result;

	handle_mutex(mutex, LOCK);
	result = *value;
	handle_mutex(mutex, UNLOCK);
	return (result);
}

void	set_last_meal(t_philo *philo)
{
	handle_mutex(&philo->meal_lock, LOCK);
	philo->last_meal_t = gettime();
	handle_mutex(&philo->meal_lock, UNLOCK);
}

long	get_long(t_mtx *mutex, long *value)
{
	long result;

	handle_mutex(mutex, LOCK);
	result = *value;
	handle_mutex(mutex, UNLOCK);
	return (result);
}

