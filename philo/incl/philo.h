/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:08:08 by cmaubert          #+#    #+#             */
/*   Updated: 2025/01/10 12:10:23 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# define FALSE 0
# define TRUE 1

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>
# include <limits.h>
# include <errno.h>

// ./philo num_of_philo time_to_die time_to_eat time_to_sleep [num_eat]
// ◦ number_of_philosophers: The number of philosophers and also the number of forks.
// ◦ time_to_die (in milliseconds): If a philosopher didn’t start eating time_to_die milliseconds since the beginning of their last meal or the beginning of the sim- ulation, they die.
// ◦ time_to_eat (in milliseconds): The time it takes for a philosopher to eat. During that time, they will need to hold two forks.
// ◦ time_to_sleep (in milliseconds): The time a philosopher will spend sleeping.
// ◦ number_of_times_each_philosopher_must_eat (optional argument): If all philosophers have eaten at least number_of_times_each_philosopher_must_eat times, the simulation stops. If not specified, the simulation stops when a philosopher dies.

typedef enum e_philo_code
{
	LOCK,
	UNLOCK,
	INIT,
	DESTROY,
	CREATE,
	JOIN,
	DETACH,
}		t_philo_code;

typedef enum e_time
{
	SECOND,
	MILLISECOND,
	MICROSECOND,
}	t_time ;

typedef enum e_philo_action
{
	EATING,
	SLEEPING,
	THINKING,
	TAKE_FIRST_FORK,
	TAKE_SECOND_FORK,
	DIED,
}	t_philo_action ;

typedef pthread_mutex_t	t_mtx;
typedef struct s_data	t_data;

typedef struct	s_fork
{
	t_mtx	fork;
	int		fork_id;
}		t_fork;

typedef struct s_philo
{
	int			id;
	pthread_t	thread_id;
	long		nb_meals_eaten;
	int 		full; // nb max de repas
	long		last_meal_t;
	t_fork		*second_fork; //t_mtx *second_fork
	t_fork		*first_fork; // t_mtx *first_fork
	t_mtx		philo_mutex; // pour les races avec le moniteur
	t_data		*data;
}	t_philo ;

typedef struct s_data {
	long	philo_nbr;
	long	time_to_die;
	long	time_to_eat;
	long	time_to_sleep;
	long	num_meals; // Nombre de repas que chaque philosophe doit manger -> optionnel
	long	start_time;
	int		end; // Flag pour indiquer si la simulation est terminée (philo meurt  ou tous sont pleins)
	// t_mtx *forks;
	t_fork	*forks; // tableau de fourchettes
	t_philo	*philos; // tableau de philosophes
	int	threads_readies;
	t_mtx	data_mega_lock; // table mutex / pour ev
	t_mtx	data_lock; // table mutex / pour eviter les races quand on lit dans la struct donnee
	t_mtx	print_lock; // Mutex pour protéger les sorties sur la console
	pthread_t	monitor;
	pthread_t	supervisor; // to do -> check la mort et les repas des philos
	long		threads_running_nb;
} t_data;

void	erro_exit(char *error);
void	*try_malloc(size_t size);
void	parse(t_data *data, char **av);
void	handle_thread(pthread_t *thread, void *(*routine)(void *), void *data, t_philo_code code);
void	handle_mutex(t_mtx *mutex, t_philo_code code);
void	data_initializer(t_data *data);
void	philo_initializer(t_data *data);
void	dinner(t_data *data);
long	gettime(int unit);
void	ft_usleep(long time_in_ms, t_data *data);
void	print_status(t_philo_action action, t_philo *philo);

// monitor
void	increase_long(t_mtx *mutex, long *value);
void	*monitor_routine(void *data);
int	threads_running(t_mtx *mutex, long *threads_running, long philo_nbr);

// set values mutex
int	dinner_finished(t_data *data);
int	get_bool(t_mtx *mutex, int *value);
void	set_bool(t_mtx *mutex, int *dest, int value);
void	set_long(t_mtx *mutex, long *dest, long value);
long	get_long(t_mtx *mutex, long *value);


void print_data(t_data *data);

#endif
