/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   00_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 14:44:39 by cmaubert          #+#    #+#             */
/*   Updated: 2025/01/20 17:39:07 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// ./philo num_of_philo time_to_die time_to_eat time_to_sleep [num_eat]
// ./philo 5 800 200 200 [5]
// max 10ms pour declarer la mort
// nb impair de philo = time_to_die au moin 3 fois sup au time_to_eat
// nb pair de philo = time to die au moins 2 fois sup au temps time_to_eat
// ./philo 4 500 200 2147483647 -> pb
// ./philo 10 310 150 100 10 -> pb - des fois mort des fois pas
// pb ordre des messages
// valgrind --tool=helgrind ./philo 155 610 200 200 5

int	main(int ac, char **av)
{
	t_data	data;

	if (ac != 5 && ac != 6)
	{
		printf("Wrong input, must have 4 or 5 paramaters\n");
		return (FALSE);
	}
	if (!parse(&data, av))
		return (FALSE);
	if (!data_initializer(&data))
	{
		free_struct("Error with the initialization of the data", &data);
		return (FALSE);
	}
	dinner(&data);
	free_destroy_mutex(&data);
	return (0);
}
