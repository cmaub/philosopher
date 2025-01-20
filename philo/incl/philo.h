/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:08:08 by cmaubert          #+#    #+#             */
/*   Updated: 2025/01/20 17:50:05 by cmaubert         ###   ########.fr       */
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

typedef pthread_mutex_t	t_mtx;
typedef pthread_t		t_thrd;
typedef struct s_data	t_data;

typedef enum e_code
{
	LOCK,
	UNLOCK,
	INIT,
	DESTROY,
	CREATE,
	JOIN,
}		t_code;

typedef enum e_philo_action
{
	EATING,
	SLEEPING,
	THINKING,
	TAKE_FIRST_FORK,
	TAKE_SECOND_FORK,
	DIED,
}		t_philo_action;

typedef struct s_fork
{
	t_mtx	fork;
	int		fork_id;
}		t_fork;

typedef struct s_philo
{
	int			nb;
	t_thrd		id;
	long		nb_meals_eaten;
	int			full; //peut-etre besoin de crer mutex pour full pour chaque philo plutot que dutiliser un mtx dans data ?
	long		last_meal_t;
	t_fork		*second_fork;
	t_fork		*first_fork;
	t_mtx		meal_lock;
	t_data		*data;
}		t_philo;

typedef struct s_data
{
	long		philo_nbr;
	long		time_to_die;
	long		time_to_eat;
	long		time_to_sleep;
	long		num_meals;
	long		start_time;
	int			end;
	t_fork		*forks;
	t_philo		*philos;
	int			philo_readies;
	t_thrd		monitor;
	long		threads_running_nb;
	t_mtx		end_lock;
	t_mtx		full_lock;
	t_mtx		data_lock;
	t_mtx		print_lock;
	t_mtx		time_lock;
	long		nb_full;
}		t_data;


void	ft_usleep2(long time_in_ms, t_data *data);

long	ft_atol(char *str);
// PARSE
int		parse(t_data *data, char **av);
// INIT
int		data_initializer(t_data *data);
// DINER
void	dinner(t_data *data);
int		threads_running(t_mtx *mutex, long *threads_running, long philo_nbr);
// ACTIONS
void	think(t_philo *philo);
void	ft_sleep(t_philo *philo);
void	eat(t_philo *philo);
// MONITOR
int		all_philos_full(t_data *data);
int		dinner_end(t_data *data);
void	*monitor_routine(void *data);
// CREATE THREADS
int		handle_mutex(t_mtx *mutex, t_code code);
int		handle_thread(t_thrd *thrd, void *(*f)(void *), void *data, t_code c);
// HANDLE VALUES WITH MTX
int		get_bool(t_mtx *mutex, int *value);
void	set_bool(t_mtx *mutex, int *dest, int value);
long	get_long(t_mtx *mutex, long *value);
void	set_last_meal(t_philo *philo);
void	increase_nb_meals(t_philo *philo);
// SYNCHRO
void	incr_thr_run_nb(t_mtx *mutex, long *value);
void	synchronise_threads(t_data	*data);
// PRINT
void	print_status(t_philo_action action, t_philo *philo);
// TIME
void	ft_usleep(long time_in_ms, t_data *data);
long	gettime(void);
// UTILS
void	free_struct(char *error, t_data *data);
void	free_destroy_mutex(t_data *data);
void	*try_malloc(size_t size);

#endif
