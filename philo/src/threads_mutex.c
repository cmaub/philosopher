/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_mutex.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 16:15:32 by cmaubert          #+#    #+#             */
/*   Updated: 2025/01/08 16:30:15 by cmaubert         ###   ########.fr       */
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
		// pthread_mutex_destroy(mutex);
		erro_exit("The value specified by mutex is invalid");
	}
	else if (status == EINVAL && INIT == code)
	{
		// pthread_mutex_destroy(mutex);
		erro_exit("The value specified by attr is invalid");
	}
	else if (status == EDEADLK)
	{
		// pthread_mutex_destroy(mutex);
		erro_exit("A deadlock would occur if the thread blocked waiting for mutex.");
	}
	else if (status == ENOMEM)
	{
		// pthread_mutex_destroy(mutex);
		erro_exit("The process cannot allocate enough memory to create another mutex.");
	}
	else if (status == EPERM)
	{
		// pthread_mutex_destroy(mutex);
		erro_exit("The current thread does not hold a lock on mutex.");
	}
	else if (status == EBUSY)
	{
		// pthread_mutex_destroy(mutex);
		erro_exit("Mutex is locked");
	}
}

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
		erro_exit("Error with the code of mutex");
}


static void handle_thread_error(int status, t_philo_code code)
{
	if (status == EAGAIN)
		erro_exit("The system lacked the necessary resources to create another thread, or the system-imposed limit on the total number of threads in a process");
	if (status == EPERM)
		erro_exit("The caller does not have appropriate permission to set the required scheduling parameters or scheduling policy.");
	if (status == EINVAL && code == CREATE)
		erro_exit("The value specified by attr is invalid.");
	if (status == EINVAL && code == JOIN)
		erro_exit("The implementation has detected that the value specified by thread does not refer to a joinable thread.");
	if (status == ESRCH && code == JOIN)
		erro_exit("No thread could be found corresponding to that specified by the given thread ID, thread.");
	if (status == EDEADLK)
		erro_exit("A deadlock was detected or the value of thread");
	if (status == EINVAL && code == DETACH)
		erro_exit("The implementation has detected that the value specified by thread does not refer to a joinable thread.");
	if (status == ESRCH && code == DETACH)
		erro_exit("No thread could be found corresponding to that specified by the given thread ID, thread");
}

void	handle_thread(pthread_t *thread, void *(*routine)(void *), void *data, t_philo_code code) //pointeur to fonction
{
	if (CREATE == code)
	 	handle_thread_error(pthread_create(thread, NULL, routine, data), code);
	else if (JOIN == code)
		handle_thread_error(pthread_join(*thread, NULL), code);
	else if (DETACH == code)
		handle_thread_error(pthread_detach(*thread), code);
	else
		erro_exit("Error with the code of mutex");
}
