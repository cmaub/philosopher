/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_values_mutex.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 15:24:02 by cmaubert          #+#    #+#             */
/*   Updated: 2025/01/13 18:01:50 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


void	increase_long(t_mtx *mutex, long *value)
{
	handle_mutex(mutex, LOCK);
	(*value) = (*value)+ 1;
	handle_mutex(mutex, UNLOCK);
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

// void	set_end(t_data *data, int i)
// {
// 	handle_mutex(&data->end_lock, LOCK);
// 	data->end = TRUE;
// 	handle_mutex(&data->end_lock, UNLOCK);
// }

void	set_end(t_data *data)
{
	handle_mutex(&data->end_lock, LOCK);
	data->end = TRUE;
	handle_mutex(&data->end_lock, UNLOCK);
}

void	set_last_meal(t_philo *philo)
{
	handle_mutex(&philo->last_meal_lock, LOCK);
	philo->last_meal_t = gettime(MILLISECOND);
	handle_mutex(&philo->last_meal_lock, UNLOCK);
}

// LONG
void	set_long(t_mtx *mutex, long *dest, long value)
{
	handle_mutex(mutex, LOCK);
	*dest = value;
	handle_mutex(mutex, UNLOCK);
}

long	get_long(t_mtx *mutex, long *value)
{
	long result;

	handle_mutex(mutex, LOCK);
	result = *value;
	handle_mutex(mutex, UNLOCK);
	return (result);
}

int	dinner_finished(t_data *data)
{
	handle_mutex(&data->end_lock, LOCK);
	if (data->end == TRUE)
		return (handle_mutex(&data->end_lock, UNLOCK), TRUE);
	handle_mutex(&data->end_lock, UNLOCK);
	return (FALSE);
}

