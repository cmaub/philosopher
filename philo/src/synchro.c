/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   synchro.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 10:24:24 by cmaubert          #+#    #+#             */
/*   Updated: 2025/01/17 15:56:35 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	incr_thr_run_nb(t_mtx *mutex, long *value)
{
	handle_mutex(mutex, LOCK);
	(*value) = (*value) + 1;
	handle_mutex(mutex, UNLOCK);
}

void	synchronise_threads(t_data	*data)
{
	while (!get_bool(&data->data_lock, &data->philo_readies))
		usleep(10);
}
