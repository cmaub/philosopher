/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 14:44:39 by cmaubert          #+#    #+#             */
/*   Updated: 2025/01/15 16:46:47 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// ./philo num_of_philo time_to_die time_to_eat time_to_sleep [num_eat]
// ./philo 5 800 200 200 [5]
//	max 10ms pour declarer la mort
// nb impair de philo = time_to_die doit etre 3 fois sup au time_to_eat
// nb pair de philo = time to die doit etre au moins 2 fois sup au temps time_to_eat
// 4 410 200 200 -> pas de mort
// 3 410 200 200 -> Mort (fonctionne avec n'importe quel nombres impaire de philo, un mort a chaque fois)
// 4 410 200 100 -> pas de mort
// 3 410 200 100 -> mort 
// 5 800 200 200
// 2 400 200 200 est ce normal qu un phil meurt?

int	main (int ac, char **av)
{
	t_data	data;
	if (ac == 5 || 6 == ac)
	{
		parse(&data, av);
		data_initializer(&data);
		dinner(&data);
		free_destroy_mutex(&data); // revoir les protections
	}
	else
	{
		str_exit("Wrong input, must have 4 or 5 paramaters", NULL);
	}
	return (0);
}
