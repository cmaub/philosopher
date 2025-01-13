/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 17:12:59 by cmaubert          #+#    #+#             */
/*   Updated: 2025/01/13 18:20:31 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// ./philo num_of_philo time_to_die time_to_eat time_to_sleep [num_eat]
// ./philo 5 800 200 200 [5]

#include "philo.h"

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYAN "\033[0;36m"
#define RESET "\033[0m"


void print_data(t_data *data) {
    printf("Number of Philosophers: %ld\n", data->philo_nbr);
    printf("Time to Die: %ld ms\n", data->time_to_die);
    printf("Time to Eat: %ld ms\n", data->time_to_eat);
    printf("Time to Sleep: %ld ms\n", data->time_to_sleep);
    printf("Number of Meals: %ld\n", data->num_meals);
    printf("Simulation Start Time: %ld\n", data->start_time);
    printf("Simulation Ended: %s\n", data->end ? "Yes" : "No");

    // Affichage des fourchettes (si pertinent)
    for (int i = 0; i < data->philo_nbr; i++) {
        printf("Fork %d Address: %p\n", i, (void *)&data->forks[i]);
    }

    printf("\nPhilosophers Details:\n");
    for (int i = 0; i < data->philo_nbr; i++) {
        t_philo *philo = &data->philos[i];
        printf("Philosopher ID: %d\n", philo->id);
        printf("Thread ID: %ld\n", (long)philo->thread_id);
        printf("Number of Meals Eaten: %ld\n", philo->nb_meals_eaten);
        printf("Is Full: %d\n", philo->full);
        printf("Last Meal Time: %ld\n", philo->last_meal_t);
        printf("First Fork Address: %p\n", (void *)philo->first_fork);
        printf("Second Fork Address: %p\n", (void *)philo->second_fork);
        printf("\n");
    }
}

void debug_status(t_philo_action action, t_philo *philo) 
{

    handle_mutex(&philo->data->print_lock, LOCK);
    long elapsed = gettime(MILLISECOND) - philo->data->start_time;

    // if (dinner_finished(philo->data) || all_philos_full(philo->data) == TRUE)
    //     return; // PB ici
     printf("[%ld ms] " CYAN "**%d**" RESET ": ", elapsed, philo->id);

    switch (action) {
        case TAKE_FIRST_FORK:
            printf(CYAN "took first fork (ID: %d)" RESET "\n", philo->first_fork->fork_id);
            break;
        case TAKE_SECOND_FORK:
            printf(CYAN "took second fork (ID: %d)" RESET "\n", philo->second_fork->fork_id);
            break;
        case EATING:
            printf(GREEN "is eating" RESET "\n");
            break;
        case SLEEPING:
            printf(BLUE "is sleeping" RESET "\n");
            break;
        case THINKING:
            printf(MAGENTA "is thinking" RESET "\n");
            break;
        case DIED:
            printf(RED "has died" RESET "\n");
            break;
        default:
            printf(YELLOW "unknown action" RESET "\n");
            break;
    }

    printf("  [DEBUG] Meals eaten: %ld, Last meal time: %ld\n", philo->nb_meals_eaten, philo->last_meal_t);
    // printf("  [DEBUG] Data -> Philosopher count: %ld, Time to die: %ld\n", philo->data->philo_nbr, philo->data->time_to_die);
    handle_mutex(&philo->data->print_lock, UNLOCK);
}

void	print_status(t_philo_action action, t_philo *philo)
{
	debug_status(action, philo);
	// long	elapsed;

    // handle_mutex(&philo->data->print_lock, LOCK);
	// elapsed = gettime(MILLISECOND) - philo->data->start_time;
	// // if (philo->full || philo->data->end)
	// // 	return ;
	// // if ((TAKE_FIRST_FORK == action|| TAKE_SECOND_FORK == action))
	// // {
	// // 	if (TAKE_FIRST_FORK == action)
	// // 		printf("%ld ms Philosopher %d has taken the first fork (number %d)\n", elapsed, philo->id, philo->first_fork->fork_id);
	// // 	if (TAKE_SECOND_FORK == action)
	// // 		printf("%ld ms Philosopher %d has taken the second fork (number %d)\n", elapsed, philo->id, philo->second_fork->fork_id);
	// // }
	// // else if (EATING == action)
	// // 	printf("%ld ms Philosopher %d is eating\n", elapsed, philo->id);
	// // else if (SLEEPING == action)
	// // 	printf("%ld ms Philosopher %d is sleeping\n", elapsed, philo->id);
	// // else if (THINKING == action)
	// // 	printf("%ld ms Philosopher %d is thinking\n", elapsed, philo->id);
	// // else if (DIED == action)
	// // 	printf("%ld ms Philosopher %d is dead\n", elapsed, philo->id);

    // // pour philo_visualizer
    // if (EATING == action)
	// 	printf("%ld %d is eating\n", elapsed, philo->id);
	// else if (SLEEPING == action)
	// 	printf("%ld %d is sleeping\n", elapsed, philo->id);
	// else if (THINKING == action)
	// 	printf("%ld %d is thinking\n", elapsed, philo->id);
	// else if (DIED == action)
	// 	printf("%ld %d is dead\n", elapsed, philo->id);

    // handle_mutex(&philo->data->print_lock, UNLOCK);
}
