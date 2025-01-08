/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_values_mutex.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 15:24:02 by cmaubert          #+#    #+#             */
/*   Updated: 2025/01/08 15:41:04 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// INT changer en booleen ?
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
	return (get_bool(&data->data_lock, &data->end));
}

