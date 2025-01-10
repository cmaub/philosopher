/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 14:44:39 by cmaubert          #+#    #+#             */
/*   Updated: 2025/01/10 15:46:08 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// foncition pour intialiser tableau
// fonction pour allouer dynamiquement
// fonction pour la gestion du temps

// routine
// fonction qui print

// ./philo num_of_philo time_to_die time_to_eat time_to_sleep [num_eat]
// ./philo 5 800 200 200 [5]


int	main (int ac, char **av)
{
	t_data	data;
	if (ac == 5 || 6 == ac)
	{
		parse(&data, av);
		data_initializer(&data);
		dinner(&data);
		free_destroy_mutex(&data);
	}
	else
	{
		erro_exit("Wrong input, must have 4 or 5 paramaters");
	}
	return (0);
}

