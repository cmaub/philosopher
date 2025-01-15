/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 16:30:40 by cmaubert          #+#    #+#             */
/*   Updated: 2025/01/15 16:45:03 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// ./philo num_of_philo time_to_die time_to_eat time_to_sleep [num_eat]
// ./philo 5 800 200 200 [5]
// 1 s = 1000 milliseconde
// 1 s = 1000 000 microseconde
// 1 ms = milliseconde = 10puissance-3 = 1/1000 = 1000 microsecondes
// 1 us	= microseconde = 10puissance-6 = 1/1000000 = 0,001 milliseconde

// usleep = microseconde
// sortie de gettimeofday convertie en milliseconde
// int gettimeofday(struct timeval *restrict tp, void *restrict tzp);

// struct timeval 
// {
//     time_t      tv_sec;     /* seconds */
//     suseconds_t tv_usec;    /* microseconds */
// };

long gettime()
{
	struct timeval tv;

	if (gettimeofday(&tv, NULL))
		str_exit("gettimeofday failed", NULL);
	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

void	ft_usleep(long time_in_ms, t_data *data)
{
	(void)data;
	long	start_time;
	long	elapsed;
	long	remaining;

	start_time = gettime();
	if (dinner_end(data) == TRUE)
		return ;
	while (gettime() - start_time < time_in_ms)
	{
		// if (dinners_end(data))
		// 	break ;
		elapsed = gettime() - start_time;
		remaining = time_in_ms - elapsed;
		// pour optimiser alterner entre methode si temps restant est petit ou grand
		// Si le temps restant est supérieur à 1 milliseconde utiliser usleep
		// pour mettre en pause le programme pendant la moitié du temps
		// restant pour reduire la consommation de ressources
		if (remaining > 1)
			usleep(remaining * 1000 / 2);
		else
		{
			//SPINLOCK : occuper activement le programme jusqu'a ce qu'une condition soit remplie
			while (gettime() - start_time < time_in_ms)
				;
		}
	}
}