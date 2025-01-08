/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 16:34:03 by cmaubert          #+#    #+#             */
/*   Updated: 2025/01/08 17:56:48 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void print_data(t_data *data) {
    printf("Number of Philosophers: %ld\n", data->philo_nbr);
    printf("Time to Die: %ld ms\n", data->time_to_die);
    printf("Time to Eat: %ld ms\n", data->time_to_eat);
    printf("Time to Sleep: %ld ms\n", data->time_to_sleep);
    printf("Number of Meals: %ld\n", data->num_meals);
    printf("Simulation Start Time: %ld\n", data->start_time);
    printf("Simulation Ended: %s\n", data->end ? "Yes" : "No");

    // Affichage des fourchettes (si pertinent)
    for (int i = 0; i < data->philo_nbr; i++) {
        printf("Fork %d Address: %p\n", i, (void *)&data->forks[i]);
    }

    printf("\nPhilosophers Details:\n");
    for (int i = 0; i < data->philo_nbr; i++) {
        t_philo *philo = &data->philos[i];
        printf("Philosopher ID: %d\n", philo->id);
        printf("Thread ID: %ld\n", (long)philo->thread_id);
        printf("Number of Meals Eaten: %ld\n", philo->nb_meals_eaten);
        printf("Is Full: %d\n", philo->full);
        printf("Last Meal Time: %ld\n", philo->last_meal_t);
        printf("First Fork Address: %p\n", (void *)philo->first_fork);
        printf("Second Fork Address: %p\n", (void *)philo->second_fork);
        printf("\n");
    }
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

long	gettime(t_time time_code)
{
	struct timeval tv;

	if (gettimeofday(&tv, NULL))
		erro_exit("gettimeofday failed");
	if (SECOND == time_code)
		return (tv.tv_sec + (tv.tv_usec / 1e6)); // microsecondes divisees par 1000000
	else if (MILLISECOND == time_code)
		return ((tv.tv_sec * 1e3) + (tv.tv_usec / 1e3));
	else if (MICROSECOND == time_code)
		return ((tv.tv_sec * 1e6) + tv.tv_usec);
	else
		erro_exit("Wrong time code");
	return (1);
}

void	ft_usleep(long time_in_ms, t_data *data)
{
	long	start_time;
	long	elapsed;
	long	remaining;

	start_time = gettime(MICROSECOND);
	while (gettime(MICROSECOND) - start_time < time_in_ms)
	{
		if (dinner_finished(data))
			break ;
		elapsed = gettime(MICROSECOND) - start_time;
		remaining = time_in_ms - elapsed;
		// pour optimiser alterner entre methode si temps restant est petit ou grand
		if (remaining > 1e3)
			usleep(remaining / 2); // Si le temps restant est supérieur à 1000 microsecondes (1 milliseconde) utiliser usleep pour mettre en pause le programme pendant la moitié du temps restant pour reduire la consommation de ressources
		else
		{
			//SPINLOCK : occuper activement le programme jusqua ce qu'une condition soit remplie
			while (gettime(MICROSECOND) - start_time < time_in_ms)
				;
		}
	}
}
