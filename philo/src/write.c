/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 17:12:59 by cmaubert          #+#    #+#             */
/*   Updated: 2025/01/08 17:33:51 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// ./philo num_of_philo time_to_die time_to_eat time_to_sleep [num_eat]
// ./philo 5 800 200 200 [5]

#include "philo.h"

void	print_status(t_philo_action action, t_philo *philo)
{
	long	elapsed;

	elapsed = gettime(MILLISECOND) - philo->data->start_time;
	if (philo->full || dinner_finished(philo->data))
		return ;
	// fonction de debug (1:25)
	handle_mutex(&philo->data->print_lock, LOCK);
	if ((TAKE_FIRST_FORK == action|| TAKE_SECOND_FORK == action))
		printf("%ld ms Philosopher %d has taken a fork\n", elapsed, philo->id);
	else if (EATING == action)
		printf("%ld ms Philosopher %d is eating\n", elapsed, philo->id);
	else if (SLEEPING == action)
		printf("%ld ms Philosopher %d is sleeping\n", elapsed, philo->id);
	else if (THINKING == action)
		printf("%ld ms Philosopher %d is thinking\n", elapsed, philo->id);
	else if (DIED == action)
		printf("%ld ms Philosopher %d is dead\n", elapsed, philo->id);
	handle_mutex(&philo->data->print_lock, UNLOCK);
}
