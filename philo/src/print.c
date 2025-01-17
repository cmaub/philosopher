/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 17:12:59 by cmaubert          #+#    #+#             */
/*   Updated: 2025/01/17 16:20:27 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// ./philo num_of_philo time_to_die time_to_eat time_to_sleep [num_eat]
// ./philo 5 800 200 200 [5]

#include "philo.h"

void	print_status(t_philo_action action, t_philo *philo)
{
	long	elapsed;

	handle_mutex(&philo->data->print_lock, LOCK);
	elapsed = gettime() - philo->data->start_time;
	if (TAKE_FIRST_FORK == action && !all_philos_full(philo->data))
		printf("%ld %d has taken a fork \n", elapsed, philo->nb);
	if (TAKE_SECOND_FORK == action && !all_philos_full(philo->data))
		printf("%ld %d has taken a fork \n", elapsed, philo->nb);
	if (EATING == action)
		printf("%ld %d is eating\n", elapsed, philo->nb);
	else if (SLEEPING == action && !all_philos_full(philo->data))
		printf("%ld %d is sleeping\n", elapsed, philo->nb);
	else if (THINKING == action && !all_philos_full(philo->data))
		printf("%ld %d is thinking\n", elapsed, philo->nb);
	else if (DIED == action)
		printf("%ld %d died\n", elapsed, philo->nb);
	handle_mutex(&philo->data->print_lock, UNLOCK);
}
