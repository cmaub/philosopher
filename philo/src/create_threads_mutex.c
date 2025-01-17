/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_threads_mutex.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 16:15:32 by cmaubert          #+#    #+#             */
/*   Updated: 2025/01/17 15:52:59 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	handle_mutex(t_mtx *mutex, t_code code)
{
	if (LOCK == code)
	{
		if (pthread_mutex_lock(mutex))
			return (FALSE);
	}
	else if (UNLOCK == code)
	{
		if (pthread_mutex_unlock(mutex))
			return (FALSE);
	}
	else if (INIT == code)
	{
		if (pthread_mutex_init(mutex, NULL))
			return (FALSE);
	}
	else if (DESTROY == code)
	{
		if (pthread_mutex_destroy(mutex))
			return (FALSE);
	}
	else
		printf("Error with the code of mutex\n");
	return (TRUE);
}

int	handle_thread_error(int status)
{
	if (status == 0)
		return (TRUE);
	else
		return (FALSE);
}

int	handle_thread(t_thrd *thread, void *(*f)(void *), void *data, t_code code)
{
	if (CREATE == code)
		return (handle_thread_error(pthread_create(thread, NULL, f, data)));
	else if (JOIN == code)
		return (handle_thread_error(pthread_join(*thread, NULL)));
	else
	{
		printf("Error with the code of thread");
		return (FALSE);
	}
	return (FALSE);
}
