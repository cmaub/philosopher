/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_threads_mutex.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 16:15:32 by cmaubert          #+#    #+#             */
/*   Updated: 2025/01/17 10:59:20 by cmaubert         ###   ########.fr       */
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


int handle_thread_error(int status, t_code code)
{
	if (status == EAGAIN)
		printf("The system lacked the necessary resources to create another thread, or the system-imposed limit on the total number of threads in a process\n");
	if (status == EPERM)
		printf("The caller does not have appropriate permission to set the required scheduling parameters or scheduling policy.\n");
	if (status == EINVAL && code == CREATE)
		printf("The value specified by attr is invalid.\n");
	if (status == EINVAL && code == JOIN)
		printf("The implementation has detected that the value specified by thread does not refer to a joinable thread.\n");
	if (status == ESRCH && code == JOIN)
		printf("No thread could be found corresponding to that specified by the given thread ID, thread.\n");
	if (status == EDEADLK)
		printf("A deadlock was detected or the value of thread\n");
	if (status == 0)
		return (TRUE);
	else
		return (FALSE);
}

int	handle_thread(t_thrd *thread, void *(*routine)(void *), void *data, t_code code)
{
	if (CREATE == code)
	 	return (handle_thread_error(pthread_create(thread, NULL, routine, data), code));
	else if (JOIN == code)
		return (handle_thread_error(pthread_join(*thread, NULL), code));
	else
	{
		printf("Error with the code of thread");
		return (FALSE);
	}
	return (FALSE);
}
