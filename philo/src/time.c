/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 16:30:40 by cmaubert          #+#    #+#             */
/*   Updated: 2025/01/20 17:36:36 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// ./philo num_of_philo time_to_die time_to_eat time_to_sleep [num_eat]
// ./philo 5 800 200 200 [5]
// 1 ms = milliseconde = 10puissance-3 = 1/1000 = 1000 microsecondes
// 1 us	= microseconde = 10puissance-6 = 1/1000000 = 0,001 milliseconde
// 1 s = 1000 milliseconde
// 1 s = 1000 000 microseconde

// usleep = sortie en microseconde
// sortie de gettimeofday convertie en milliseconde
// int gettimeofday(struct timeval *restrict tp, void *restrict tzp);

// struct timeval
// {
//     time_t      tv_sec;     /* seconds */
//     suseconds_t tv_usec;    /* microseconds */
// };

long	gettime(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		printf("gettimeofday failed\n");
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ft_usleep(long time_in_ms, t_data *data)
{
	long	start;

	start = gettime();
	if (((gettime() - start) > data->time_to_die))
		return ;
	if ((gettime() - start) > data->time_to_die)
		return ;
	while (gettime() - start < time_in_ms)
	{
		if ((gettime() - start) > data->time_to_die)
			return ;
		usleep(50);
	}
}

