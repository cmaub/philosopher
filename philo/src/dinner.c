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

// revoir la fonction THINK
static void	think(t_philo *philo/*, int bool*/)
{
	long	t_to_think;
	long	t_to_sleep;
	long	t_to_eat;

	// if (bool == FALSE)
	// {
	// 	print_status(THINKING, philo);
	// }
	// si le nb est pair aucun philo ne peux manger plus de 2 fois sans penser
	// donc pas besoin d'ajouter du temps
	if (philo->data->philo_nbr % 2 == 0)
	{
		print_status(THINKING, philo);
		return ;
	}
	// philo impair
	t_to_eat = philo->data->time_to_eat;
	t_to_sleep = philo->data->time_to_sleep;
	t_to_think = t_to_eat * 2 - t_to_sleep;
	if (t_to_think < 0)
		t_to_think = 0;
	ft_usleep(t_to_think * 0.50, philo->data);
	print_status(THINKING, philo);
}

static void	synchronise_threads(t_data	*data)
{
	//tous les threads passent par la, tant qu'il y en a un qui n'est pas pret reste dans la boucle
	while (!get_bool(&data->data_lock, &data->threads_readies))
		;
}

// void	desynchronise_threads(t_philo *philo)
// {
// 	if (philo->data->philo_nbr % 2 == 0)
// 	{
// 		if (philo->id % 2 == 0)
// 			ft_usleep(3000, philo->data);
// 	}
// 	else
// 	{
// 		if (philo->id % 2 == 0)
// 			think(philo, TRUE);
// 	}
// }

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
	long current_time;
	long last_meal_time;
	long elapsed;
	t_data *data = (t_data *)arg;

	while (get_bool(&data->data_lock, &data->end) == FALSE)
	{
		handle_mutex(&data->data_lock, LOCK);
		i = -1;
		while (++i < data->philo_nbr) // rajout d'une condition de sortie de boucle?
		{

			current_time = gettime(MILLISECOND);
			last_meal_time = get_long(&data->philos[i].philo_mutex, &data->philos[i].last_meal_t);
			elapsed = current_time - last_meal_time;
			if (elapsed > data->time_to_die)
			{
				//data race
				// handle_mutex(&data->data_mega_lock, UNLOCK);
				set_bool(&data->philos[i].philo_mutex, &data->end, TRUE);
				// handle_mutex(&data->data_mega_lock, UNLOCK);
				print_status(DIED, &data->philos[i]);
				break;
			}
			if (all_philos_full(data) == TRUE)
			{
				// data race
				set_bool(&data->data_mega_lock, &data->end, TRUE);
				printf("*** Philosophes rassasies !\n");
				break;
			}
		}
		handle_mutex(&data->data_lock, UNLOCK);
		usleep(1000); // Sleep pour reduire l'activite du CPU ?
	}
	return NULL;
}

// PB PHILO IMPAIR n'ont pas de routine
// PB The value specified by mutex is invalid ./philo 199 402 200 200
void	*philo_routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	//spinlock
	synchronise_threads(philo->data);
	// mettre a jour last meal ici ?
	set_long(&philo->philo_mutex, &philo->last_meal_t, gettime(MILLISECOND));
	increase_long(&philo->data->data_lock, &philo->data->threads_running_nb);
	// desyncroniser les philos
	// desynchronise_threads(philo);
	while (get_bool(&philo->philo_mutex, &philo->data->end) == FALSE)
	{
		// trouver comment optimiser ++ PB DE DATA RACE AVEC LES GET_BOOL
		if (get_bool(&philo->philo_mutex, &philo->full) == TRUE || get_bool(&philo->philo_mutex, &philo->data->end) == TRUE)
			break ;
		eat(philo);
		if (get_bool(&philo->philo_mutex, &philo->full) == TRUE || get_bool(&philo->philo_mutex, &philo->data->end) == TRUE)
			break ;
		print_status(SLEEPING, philo);
		if (get_bool(&philo->philo_mutex, &philo->full) == TRUE || get_bool(&philo->philo_mutex, &philo->data->end) == TRUE)
			break ;
		ft_usleep(philo->data->time_to_sleep, philo->data);
		if (get_bool(&philo->philo_mutex, &philo->full) == TRUE || get_bool(&philo->philo_mutex, &philo->data->end) == TRUE)
			break ;
		think(philo);/*, FALSE*/
	}
	return (NULL);
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
	// une fois que tous les philos sont crees on peut commencer le repas
	set_bool(&data->data_lock, &data->threads_readies, TRUE);
	handle_thread(&data->monitor, NULL, NULL, JOIN); // leak si mis apres le join des philos
	i = -1;
	while (++i < data->philo_nbr)
		handle_thread(&data->philos[i].thread_id, NULL, NULL, JOIN);
	set_bool(&data->data_lock, &data->end, TRUE);
}

// void *monitor_routine(void *arg)
// {
// 	int i;
// 	long current_time;
// 	long last_meal_time;
// 	long elapsed;
// 	int	end;

// 	t_data *data = (t_data *)arg;
// 	end = get_bool(&data->data_lock, &data->end);
// 	while (end == FALSE)
// 	{
// 		end = get_bool(&data->data_lock, &data->end);
// 		if (end == TRUE)
// 			break ;
// 		i = -1;
// 		while (++i <data->philo_nbr)
// 		{
// 			current_time = gettime(MILLISECOND);
// 			last_meal_time = get_long(&data->philos[i].philo_mutex, &data->philos[i].last_meal_t);
// 			elapsed = current_time - last_meal_time;
// 			if (elapsed >= data->time_to_die)
// 			{
// 				set_bool(&data->philos[i].philo_mutex, &data->end, TRUE);
// 				print_status(DIED, &data->philos[i]);
// 				break;
// 			}
// 			if (all_philos_full(data) == TRUE)
// 			{
// 				set_bool(&data->data_mega_lock, &data->end, TRUE);
// 				printf("*** Philosophes rassasies !\n");
// 				break;
// 			}
// 		}
// 		usleep(1000); // Sleep pour reduire l'activite du CPU ?
// 	}
// 	return NULL;
// }
