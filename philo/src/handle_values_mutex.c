/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_values_mutex.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 15:24:02 by cmaubert          #+#    #+#             */
/*   Updated: 2025/01/10 10:56:43 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


void	increase_long(t_mtx *mutex, long *value)
{
	handle_mutex(mutex, LOCK);
	(*value) = (*value)+ 1;
	// printf("increase_long = %ld\n", *value);
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

int	dinner_finished(t_data *data)
{
	int	result;
	result = get_bool(&data->data_lock, &data->end);
	return (result);
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



