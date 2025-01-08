/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 14:44:39 by cmaubert          #+#    #+#             */
/*   Updated: 2025/01/08 16:02:51 by cmaubert         ###   ########.fr       */
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
	// struct timeval current_time;
	// gettimeofday(&current_time, NULL);
	// printf("second = %ld, micro second = %d\n", current_time.tv_sec, current_time.tv_usec);
	if (ac == 5 || 6 == ac)
	{
		parse(&data, av);
		data_initializer(&data);
		dinner(&data);
		// clean(&data);
	}
	else
	{
		erro_exit("Wrong input, must have 4 or 5 paramaters");
	}
	return (0);
}

// int main() {
// 	pthread_t thread_id;

//  	// Create a new thread
// 	if (pthread_create(&thread_id, NULL, print_in_background, NULL) != 0)
// 	{
// 		perror("Failed to create thread");
// 		return 1;
// 	}

//     // 🚨 toggle these 2 🚨
//     // pthread_detach(thread_id);
// 	pthread_join(thread_id, NULL);

//     // Continue with the main thread's work
// 	printf("This is printed from the main thread.\n");
// 	usleep(100000);

//     return 0;
// }
