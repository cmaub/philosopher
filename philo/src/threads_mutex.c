/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_mutex.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 16:15:32 by cmaubert          #+#    #+#             */
/*   Updated: 2025/01/15 17:04:03 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	handle_mutex_error(int status, t_philo_code code, t_mtx *mutex)
{
	(void)mutex;
	if (status == 0)
		return ;
	if (status == EINVAL || (LOCK == code || UNLOCK == code))
	{
		pthread_mutex_destroy(mutex);
		str_exit("The value specified by mutex is invalid", NULL);
	}
	else if (status == EINVAL && INIT == code)
		str_exit("The value specified by attr is invalid", NULL);
	else if (status == EDEADLK)
	{
		pthread_mutex_destroy(mutex);
		str_exit("A deadlock would occur if the thread blocked waiting for mutex.", NULL);
	}
	else if (status == ENOMEM)
	{
		pthread_mutex_destroy(mutex);
		str_exit("The process cannot allocate enough memory to create another mutex.", NULL);
	}
	else if (status == EPERM)
	{
		pthread_mutex_destroy(mutex);
		str_exit("The current thread does not hold a lock on mutex.", NULL);
	}
	else if (status == EBUSY)
	{
		pthread_mutex_destroy(mutex);
		str_exit("Mutex is locked", NULL);
	}
}

//revoir la gestion des erreurs
void	handle_mutex(t_mtx *mutex, t_philo_code code)
{
	if (LOCK == code)
	 	handle_mutex_error(pthread_mutex_lock(mutex), code, mutex);
	else if (UNLOCK == code)
		handle_mutex_error(pthread_mutex_unlock(mutex), code, mutex);
	else if (INIT == code)
		handle_mutex_error(pthread_mutex_init(mutex, NULL), code, mutex);
	else if (DESTROY == code)
		handle_mutex_error(pthread_mutex_destroy(mutex), code, mutex);
	else
		str_exit("Error with the code of mutex", NULL); //
}


int handle_thread_error(int status, t_philo_code code)
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

int	handle_thread(pthread_t *thread, void *(*routine)(void *), void *data, t_philo_code code)
{
	if (CREATE == code)
	 	return (handle_thread_error(pthread_create(thread, NULL, routine, data), code));
	else if (JOIN == code)
		return (handle_thread_error(pthread_join(*thread, NULL), code));
	else
	{
		printf("Error with the code of mutex");
		return (FALSE);
	}
	return (FALSE);
}
