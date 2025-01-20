/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   04_actions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 10:26:48 by cmaubert          #+#    #+#             */
/*   Updated: 2025/01/20 17:32:49 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	take_forks(t_philo *philo)
{
	handle_mutex(&philo->first_fork->fork, LOCK);
	if (!all_philos_full(philo->data) && !dinner_end(philo->data))
		print_status(TAKE_FIRST_FORK, philo);
	handle_mutex(&philo->second_fork->fork, LOCK);
	if (!all_philos_full(philo->data) && !dinner_end(philo->data))
		print_status(TAKE_SECOND_FORK, philo);
}

/*
LOGIQUE DE EAT
-prendre fourchetttes
-mettre a jour : last_meal_time, nb_de_meals_eaten
-condition if :
	philo ne doit pas etre full
	le nb de meals a maer doit etre sup a 0
	le nb de meals mange par le philo est == au nb de meals eaten
		mettre a full
		message
		check si tous sont full et 
			-> SI tous rassasies mettre TRUE a end 
			-> sinon ft_sleep du temps de eat_time
- sinon (donc quand pas de limite de meals ou que le philo nest pas full)
	message, ft_usleep

*/

// VERSION 3
// void	eat(t_philo *philo)
// {
// 	take_forks(philo);
// 	increase_nb_meals(philo);
// 	set_last_meal(philo);
// 	if (!dinner_end(philo->data) && philo->data->num_meals < 0)
// 	{
// 		print_status(EATING, philo);
// 		ft_usleep(philo->data->time_to_eat, philo->data);
// 	}
// 	else if (philo->data->num_meals > 0 
// 			&& !all_philos_full(philo->data))
// 	{
// 			print_status(EATING, philo);
// 			// handle_mutex(&philo->data->full_lock, LOCK);
// 			if (philo->data->num_meals > 0
// 				&& philo->nb_meals_eaten == philo->data->num_meals && all_philos_full(philo->data))
// 			{
// 				printf("philo_nb = %d\n", philo->nb);
// 				philo->full = TRUE;
// 				// handle_mutex(&philo->data->full_lock, UNLOCK);
// 			}
// 			else
// 				ft_usleep(philo->data->time_to_eat, philo->data);
// 			// handle_mutex(&philo->data->full_lock, UNLOCK);	
// 	}
// 	handle_mutex(&philo->first_fork->fork, UNLOCK);
// 	handle_mutex(&philo->second_fork->fork, UNLOCK);
// }

// VERSION 1
// void	eat(t_philo *philo)
// {
// 	if (((!all_philos_full(philo->data) && philo->data->num_meals > 0)
// 			|| philo->data->num_meals == -1)
// 		&& !dinner_end(philo->data))
// 	{
// 		take_forks(philo);
// 		if (!all_philos_full(philo->data)&& !dinner_end(philo->data))
// 		{
// 			increase_nb_meals(philo);
// 			set_last_meal(philo);
// 			print_status(EATING, philo);
// 			if (!all_philos_full(philo->data))
// 				ft_usleep(philo->data->time_to_eat, philo->data); // va une fois ici
// 			handle_mutex(&philo->data->full_lock, LOCK);
// 			if (philo->data->num_meals > 0
// 				&& philo->nb_meals_eaten == philo->data->num_meals)
// 				philo->full = TRUE;
// 			handle_mutex(&philo->data->full_lock, UNLOCK);
// 		}
// 	}
// 	handle_mutex(&philo->first_fork->fork, UNLOCK);
// 	handle_mutex(&philo->second_fork->fork, UNLOCK);
// }


// VERSION 2  marche !
//creer un flag si philo est en train de manger pour eviter qu'il meurt ? 
void	eat(t_philo *philo)
{
	take_forks(philo);
	increase_nb_meals(philo);
	set_last_meal(philo);
	if ((!get_bool(&philo->data->full_lock, &philo->full) && philo->data->num_meals > 0 
		&& get_long(&philo->data->full_lock, &philo->nb_meals_eaten) == philo->data->num_meals))
	{
			print_status(EATING, philo);
			handle_mutex(&philo->data->full_lock, LOCK);
			if (philo->data->num_meals > 0
				&& philo->nb_meals_eaten == philo->data->num_meals)
				philo->full = TRUE;
			handle_mutex(&philo->data->full_lock, UNLOCK);
			if (!all_philos_full(philo->data))
				ft_usleep(philo->data->time_to_eat, philo->data);
	}
	else
	{
		print_status(EATING, philo);
		ft_usleep(philo->data->time_to_eat, philo->data);
			
	}
	handle_mutex(&philo->first_fork->fork, UNLOCK);
	handle_mutex(&philo->second_fork->fork, UNLOCK);
}

void	think(t_philo *philo)
{
	if (!dinner_end(philo->data))
	{
			print_status(THINKING, philo);
		if (philo->data->philo_nbr % 2 != 0)
			ft_usleep(philo->data->time_to_eat * 0.5, philo->data);
	}
}

void	ft_sleep(t_philo *philo)
{
	if (!dinner_end(philo->data))
	{
		print_status(SLEEPING, philo);
		ft_usleep(philo->data->time_to_sleep, philo->data);
	}
}
