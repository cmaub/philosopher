/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 16:34:03 by cmaubert          #+#    #+#             */
/*   Updated: 2025/01/10 16:03:30 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_destroy_mutex(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->philo_nbr)
		handle_mutex(&data->philos[i].philo_mutex, DESTROY);
	i = -1;
	while (++i < data->philo_nbr)
		handle_mutex(&data->forks[i].fork, DESTROY);
	handle_mutex(&data->data_lock, DESTROY);
	handle_mutex(&data->print_lock, DESTROY);
	handle_mutex(&data->data_mega_lock, DESTROY);
	free(data->philos);
	free(data->forks);
}

void	erro_exit(char *error)
{
	printf("%s\n", error);
	exit(EXIT_FAILURE);
}

void	*try_malloc(size_t size)
{
	void	*p;

	p = malloc(size);
	if (p == NULL)
		erro_exit("fatal: out of memory");
	memset(p, 0, size);
	return (p);
}

// int gettimeofday(struct timeval *restrict tp, void *restrict tzp);

// long	gettime(t_time time_code)
// {
// 	struct timeval tv;

// 	if (gettimeofday(&tv, NULL))
// 		erro_exit("gettimeofday failed");
// 	if (SECOND == time_code)
// 		return (tv.tv_sec + (tv.tv_usec / 1e6)); // microsecondes divisees par 1000000
// 	else if (MILLISECOND == time_code)
// 		return ((tv.tv_sec * 1e3) + (tv.tv_usec / 1e3));
// 	else if (MICROSECOND == time_code)
// 		return ((tv.tv_sec * 1e6) + tv.tv_usec);
// 	else
// 		erro_exit("Wrong time code");
// 	return (1);
// }

long gettime(int unit)
{
	struct timeval tv;

	if (gettimeofday(&tv, NULL))
		erro_exit("gettimeofday failed");
	if (unit == MILLISECOND)
		return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (tv.tv_sec * 1000000) + tv.tv_usec;
}

// ./philo num_of_philo time_to_die time_to_eat time_to_sleep [num_eat]
// ./philo 5 800 200 200 [5]
	// 1 ms 	= milliseconde = 10puissance-3 = 1/1000 = 1000 microsecondes
	// 1 us	= microseconde = 10puissance-6 = 1/1000000 = 0,001 milliseconde
	// usleep = microseconde
// sortie de gettimeofday convertie en milliseconde
void	ft_usleep(long time_in_ms, t_data *data)
{
	long	start_time;
	long	elapsed;
	long	remaining;

	start_time = gettime(MILLISECOND);
	while (gettime(MILLISECOND) - start_time < time_in_ms)
	{
		if (dinner_finished(data))
			break ;
		elapsed = gettime(MILLISECOND) - start_time;
		remaining = time_in_ms - elapsed;
		// pour optimiser alterner entre methode si temps restant est petit ou grand
		// Si le temps restant est supérieur à 1 microsecondes (1 milliseconde) utiliser usleep
		// pour mettre en pause le programme pendant la moitié du temps
		// restant pour reduire la consommation de ressources
		if (remaining > 1)
			usleep(remaining * 1000 / 2);
		else
		{
			//SPINLOCK : occuper activement le programme jusqu'a ce qu'une condition soit remplie
			while (gettime(MILLISECOND) - start_time < time_in_ms)
				;
		}
	}
}
