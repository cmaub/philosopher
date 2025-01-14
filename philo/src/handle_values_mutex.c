/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_values_mutex.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 15:24:02 by cmaubert          #+#    #+#             */
/*   Updated: 2025/01/14 12:41:10 by cmaubert         ###   ########.fr       */
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

void	set_end_true(t_data *data)
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

// int	dinner_end(t_data *arg)
// {
// 	long	current_time;
// 	long	elapsed;
// 	long	last_meal_time;
// 	int		i;
// 	t_data *data;
	
// 	data = (t_data *)arg;
// 	current_time = gettime(MILLISECOND);
// 	i = -1;
// 	handle_mutex(&data->end_lock, LOCK);
// 	if (data->end == TRUE)
// 		return (handle_mutex(&data->end_lock, UNLOCK), TRUE);
// 	while (++i < data->philo_nbr)
// 	{
// 		handle_mutex(&data->philos[i].last_meal_lock, LOCK);
// 		last_meal_time = data->philos[i].last_meal_t;
// 		handle_mutex(&data->philos[i].last_meal_lock, UNLOCK);
// 		elapsed = current_time - last_meal_time;
// 		if (elapsed > data->time_to_die) // et que philo nest pas full ++ sil est full
// 		{
// 			handle_mutex(&data->end_lock, LOCK); // verifier le lock
// 			data->end = TRUE ;
// 			handle_mutex(&data->end_lock, UNLOCK);
// 			print_status(DIED, &data->philos[i]);
// 			return (TRUE);
// 		}
// 	}
// 	return (FALSE);
// }

int	dinner_end(t_data *data)
{		
	handle_mutex(&data->end_lock, LOCK);
	if (data->end == TRUE)
		return (handle_mutex(&data->end_lock, UNLOCK), TRUE);
	handle_mutex(&data->end_lock, UNLOCK);
	return (FALSE);
}