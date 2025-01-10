/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 18:56:33 by cmaubert          #+#    #+#             */
/*   Updated: 2025/01/10 12:23:03 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// creer une asymetrie pour eviter les interblocage dans la maniere dont les philos prennent les fourchettes
static void	assign_fork(t_philo *philo, t_fork *forks, int position)
{
	int	philo_nbr;

	philo_nbr = philo->data->philo_nbr;
	philo->first_fork = &forks[position + 1 % philo_nbr]; // prend fourchette a sa droite ->  L'utilisation de % philo_nbr garantit que la position reste dans les limites du tableau, même pour le dernier philosophe
	philo->second_fork = &forks[position]; // a gauche
	if (philo->id % 2 == 0) // verifier pour le dernier dans le cas d'un chiffre impair de philo
	{
		philo->first_fork = &forks[position]; // gauche
		philo->second_fork = &forks[(position + 1) % philo_nbr]; // droite
	}
}

void	philo_initializer(t_data *data)
{
	int	i;
	t_philo	*philo;

	i = -1;
	while (++i < data->philo_nbr)
	{
		philo = data->philos + i;
		philo->id = i + 1;
		philo->full = FALSE;
		philo->nb_meals_eaten = 0;
		philo->last_meal_t = gettime(MILLISECOND);
		philo->data = data;
		handle_mutex(&philo->philo_mutex, INIT);
		assign_fork(philo, data->forks, i);
	}

}

void	data_initializer(t_data *data)
{
	int	i;

	i = -1;
	data->end = FALSE;
	data->threads_readies = FALSE;
	data->threads_running_nb = 0;
	data->philos = try_malloc(sizeof(t_philo) * data->philo_nbr);
	data->forks = try_malloc(sizeof(t_fork) * data->philo_nbr);
	data->start_time  = gettime(MILLISECOND);
	handle_mutex(&data->data_lock, INIT);
	handle_mutex(&data->data_mega_lock, INIT);
	handle_mutex(&data->print_lock, INIT);
	while (++i < data->philo_nbr)
	{
		handle_mutex(&data->forks[i].fork, INIT);
		data->forks[i].fork_id = i; // commence par 0 mais philo->id commence par 1
	}
	// printf("data->threads_running_nb = %ld\n", data->threads_running_nb);
	philo_initializer(data);
	// printf("AFTER data->threads_running_nb = %ld\n", data->threads_running_nb);

	// print_data(data);
}


