/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:54:53 by cmaubert          #+#    #+#             */
/*   Updated: 2025/01/08 15:39:38 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// void    change_start_time(t_mega *mega)
// {
//     mega->ready = 1;
//     mega->start = get_current_time();
// }

// long    start_simulation(long ac, t_mega *mega, long nb_philo)
// {
//     long    i;

//     i = -1;
//     pthread_mutex_lock(&mega->mega_mtx); // le mega_mtx est lock ici
//     if (ac == 6 && mega->meals_max == 0)
//         return (printf("No dinner tonight\n"), clean(mega, 1), FALSE);
//     else if (nb_philo == 1)
//     {
//         if (pthread_create(mega->philo[0].p, NULL, &solo, &mega->philo[0]))
//             return (clean(mega, 1), FALSE);
//     }
//     else
//     {
//         while (++i < nb_philo)
//         {
//             if (pthread_create(&mega->philo->p[i], NULL, &sim, &mega->philo[i]))
//                 return (clean(mega, 1), FALSE);
//         }
//     }
//     change_start_time(mega);
//     pthread_create(&mega->p0, NULL, &monitor, mega);
//     pthread_mutex_unlock(&mega->mega_mtx);         // le mega_mutx est delock ici
//     if (!join_monitor(mega))
//         return (clean(mega, 1), FALSE);
//     return (clean(mega, 1), TRUE);
// }


// ./philo num_of_philo time_to_die time_to_eat time_to_sleep [num_eat]
// ./philo 5 800 200 200 [5]

// monitor -> arrete le programme quand : tous le philos ont mangé le numbre de repas a manger (num_eat) ou quand ils ont un temps supérieur a time_to_die entre deux repas

static void	eat(t_philo *philo)
{
	// prendre les fourchettes
	handle_mutex(&philo->first_fork->fork, LOCK);
	print_status(TAKE_FIRST_FORK, philo);
	handle_mutex(&philo->second_fork->fork, LOCK);
	print_status(TAKE_SECOND_FORK, philo);
	// mettre a jour last meal
	set_long(&philo->philo_mutex, &philo->last_meal_t, gettime(MILLISECOND));
	// mettre a jour meals counter
	increase_long(&philo->philo_mutex, &philo->nb_meals_eaten);
	// philo->nb_meals_eaten = philo->nb_meals_eaten + 1;
	// imprimer status
	print_status(EATING, philo);
	ft_usleep(philo->data->time_to_eat, philo->data);
	if (philo->data->num_meals > 0
		&& philo->nb_meals_eaten == philo->data->num_meals)
		set_bool(&philo->philo_mutex, &philo->full, TRUE);
	handle_mutex(&philo->first_fork->fork, UNLOCK);
	handle_mutex(&philo->second_fork->fork, UNLOCK);
	// A FAIRE pour reposer les fourchette
}

static void	think(t_philo *philo)
{
	print_status(THINKING, philo);
}

static void	synchronise_threads(t_data	*data)
{
	//tous les threads passent par la, tant qu'il y en a un qui n'est pas pret reste dans la boucle
	while (!get_bool(&data->data_lock, &data->threads_readies))
		;
}

void	*lonely_philo_routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	handle_mutex(&philo->philo_mutex, LOCK);
	dprintf(2, "lonely_philo_routine\n");
	handle_mutex(&philo->philo_mutex, UNLOCK);
	synchronise_threads(philo->data);
	set_long(&philo->philo_mutex, &philo->last_meal_t, gettime(MILLISECOND));
	increase_long(&philo->data->data_lock, &philo->data->threads_running_nb);;
	print_status(TAKE_FIRST_FORK, philo);
	while (!dinner_finished(philo->data))
		usleep(200);
	return (NULL);

}

void	*supervisor_routine(void *arg)
{
	// (void)data;
	t_philo	*philo;
	long	elapsed;

	philo = (t_philo *)arg;
	elapsed = gettime(MILLISECOND) - get_long(&philo->philo_mutex, &philo->last_meal_t);

	// // spinlock tant que tous les thread ne sont pas lances
	while (threads_running(&philo->data->data_lock,
		&philo->data->threads_running_nb, philo->data->philo_nbr) == FALSE)
	{
		// printf("data->threads_running_nb = %ld\n", philo->data->threads_running_nb);
		if (threads_running(&philo->data->data_lock,
			&philo->data->threads_running_nb, philo->data->philo_nbr) == TRUE)
		{
			printf("\n\n\n******************break\n\n\n");
			break ;
		}
	}
	while (!philo->data->end)
	{
		handle_mutex(&philo->philo_mutex, LOCK);
		// printf("gettime(MILLISECOND) = %ld\n", gettime(MILLISECOND));
		printf("get_long(&philo->philo_mutex, &philo->last_meal_t) = %ld", get_long(&philo->philo_mutex, &philo->last_meal_t));
		// printf("elapsed = %ld\n", elapsed);
		// printf("philo->data->time_to_die = %ld\n", philo->data->time_to_die);
		if (elapsed >= philo->data->time_to_die) // flag si philo en trein de manger
		{
			set_bool(&philo->data->data_lock, &philo->data->end, TRUE);
			print_status(DIED, philo);
		}
		if (philo->data->num_meals == philo->nb_meals_eaten)
		{
			set_bool(&philo->data->data_lock, &philo->data->end, TRUE);
			printf("***philos ont tous manges\n");
		}
		handle_mutex(&philo->philo_mutex, UNLOCK);
	}
	return (NULL);
}

void	*philo_routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	//spinlock
	synchronise_threads(philo->data);
	// mettre a jour last meal ici ?
	set_long(&philo->philo_mutex, &philo->last_meal_t, gettime(MILLISECOND));
	increase_long(&philo->data->data_lock, &philo->data->threads_running_nb);
	while (get_bool(&philo->philo_mutex, &philo->data->end) == FALSE)
	{
		if (philo->full)
			break ;
		eat(philo);
		print_status(SLEEPING, philo);
		ft_usleep(philo->data->time_to_sleep, philo->data);
		think(philo);
	}
	return (NULL);
}

int	all_philos_full(t_data *data)
{
	int i;

	i = 0;
	while (i < data->philo_nbr)
	{
		if (!get_bool(&data->philos[i].philo_mutex, &data->philos[i].full))
			return (FALSE);
		i++;
	}
	return (TRUE);
}

void *monitor_routine(void *arg)
{
	int i;
	t_data *data = (t_data *)arg;

	while (!data->end)
	{
		handle_mutex(&data->data_lock, LOCK);
		i = -1;
		while (++i <data->philo_nbr)
		{
			long current_time = gettime(MILLISECOND);
			long last_meal_time = get_long(&data->philos[i].philo_mutex, &data->philos[i].last_meal_t);
			long elapsed = current_time - last_meal_time;
			// printf("elapsed = %ld\n", elapsed);
			// printf("data->time_to_die = %ld\n", data->time_to_die);
			// printf("last_meal_time = %ld\n", last_meal_time);
			if (elapsed >= data->time_to_die)
			{
				set_bool(&data->philos[i].philo_mutex, &data->end, TRUE);
				print_status(DIED, &data->philos[i]);
				break;
			}
			if (all_philos_full(data) == TRUE)
			{
				set_bool(&data->data_mega_lock, &data->end, TRUE);
				printf("*** Philosophes rassasies !\n");
				break;
			}
		}
		handle_mutex(&data->data_lock, UNLOCK);
		usleep(1000); // Sleep for a short period to reduce CPU usage
	}
	return NULL;
}

void	dinner(t_data *data)
{
	int	i;

	i = -1;
	if (data->num_meals == 0)
		return ;

	if (data->philo_nbr == 1)
		handle_thread(&data->philos[0].thread_id, lonely_philo_routine, &data->philos[0], CREATE);
	else
	{
		while (++i < data->philo_nbr)
			handle_thread(&data->philos[i].thread_id, philo_routine, &data->philos[i], CREATE);
	}
	// moniteur qui check la mort des philo
	handle_thread(&data->monitor, monitor_routine, data, CREATE);
	// une fois que tous les philos sont creer on peut commencer le repas
	set_bool(&data->data_lock, &data->threads_readies, TRUE);
	// handle_thread(&data->monitor, monitor_routine, data, CREATE); // monitor routine a creer
	i = -1;
	while (++i < data->philo_nbr)
		handle_thread(&data->philos[i].thread_id, NULL, NULL, JOIN);
	// si on passe ca tous les philos sont rassasies
	handle_thread(&data->monitor, NULL, NULL, JOIN);

}

