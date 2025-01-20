/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 17:12:59 by cmaubert          #+#    #+#             */
/*   Updated: 2025/01/20 17:51:07 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// ./philo num_of_philo time_to_die time_to_eat time_to_sleep [num_eat]
// ./philo 5 800 200 200 [5]

#include "philo.h"


void	print_status(t_philo_action action, t_philo *philo)
{
	long	elapsed;

	handle_mutex(&philo->data->print_lock, LOCK);
	elapsed = gettime() - get_long(&philo->data->time_lock, &philo->data->start_time);
	if (!all_philos_full(philo->data) && !dinner_end(philo->data))
	{
		if (TAKE_FIRST_FORK == action)
			printf("%ld %d has taken a fork \n", elapsed, philo->nb);
		if (TAKE_SECOND_FORK == action)
			printf("%ld %d has taken a fork \n", elapsed, philo->nb);
		if (EATING == action)
			printf("%ld %d is eating\n", elapsed, philo->nb);
		else if (SLEEPING == action)
			printf("%ld %d is sleeping\n", elapsed, philo->nb);
		else if (THINKING == action)
			printf("%ld %d is thinking\n", elapsed, philo->nb);
	}
	if (DIED == action)
			printf("%ld %d died\n", elapsed, philo->nb);
	handle_mutex(&philo->data->print_lock, UNLOCK);
}
