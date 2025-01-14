/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 16:40:34 by cmaubert          #+#    #+#             */
/*   Updated: 2025/01/14 16:23:25 by cmaubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// ./philo num_of_philo time_to_die time_to_eat time_to_sleep [num_eat]
// ./philo 5 800 200 200 [5]

// typedef struct s_data {
// 	long	num_philo;
// 	long	time_to_die;
// 	long	time_to_eat;
// 	long	time_to_sleep;
// 	long num_meals; // Nombre de repas que chaque philosophe doit manger
// 	long start_time;
// 	int end; // Flag pour indiquer si la simulation est terminée (philo meurt ou tous sont pleins)
// 	// t_mtx *forks;
// 	t_fork	*forks; // tableau de fourchettes
// 	t_philo	*philos; // tableau de philosophes
// 	// t_mtx print_lock; // Mutex pour protéger les sorties sur la console
// } t_data;

static int	is_space(char c)
{
	if (c && (c == ' ' || c == '\t'
			|| c == '\n' || c == '\r'
			|| c == '\v' || c == '\f'))
		return (1);
	return (0);
}

static int	ft_isdigit(int c)
{
	if (!(c >= '0' && c <= '9'))
		return (0);
	else
		return (c);
}

static char	*is_valid_input(char *str)
{
	char	*number;
	int		len;

	while (is_space(*str))
		str++;
	if (*str == '+')
		str++;
	else if (*str == '-')
		erro_exit("Accept only positive values");
	if (!ft_isdigit(*str))
		erro_exit("Accept only digits");
	number = str;
	len = 0;
	while (ft_isdigit(*str++))
		len++;
	if (len > 10)
		erro_exit("The value is superior at INT_MAX");
	return (number);
}

static long	ft_atol(char *str)
{
	long	num;
	int		i;

	num = 0;
	i = 0;
	str = is_valid_input(str);
	while (str[i] >= '0' && str[i] <= '9')
	{
		num = (num * 10) + (str[i] - '0');
		i++;
	}
	if (num > (INT_MAX))
		erro_exit("The value is superior at INT_MAX");
	return (num);
}

// ./philo num_of_philo time_to_die time_to_eat time_to_sleep [num_eat]
// ./philo 5 800 200 200 [5]
	// 1 ms = milliseconde = 10puissance-3 = 1/1000 = 1000 microsecondes
	// 1 us	= microseconde = 10puissance-6 = 1/1000000 = 0,001 milliseconde
	// usleep = microseconde
void	parse(t_data *data, char **av)
{
	// ajouter verification si bien chiffres
	data->philo_nbr = ft_atol(av[1]);
	if (data->philo_nbr > 200 || data->philo_nbr <= 0 )
	{
		erro_exit("philo number must be less than 200 and more than 0");
	}
	data->time_to_die = ft_atol(av[2]);
	data->time_to_eat = ft_atol(av[3]);
	data->time_to_sleep = ft_atol(av[4]);
	if (data->time_to_die < 60
		|| data->time_to_eat < 60
			|| data->time_to_sleep < 60)
			erro_exit("Timestamps must be major than 60ms");
	if (av[5])
	{
		data->num_meals = ft_atol(av[5]);
		if (data->num_meals <= 0)
			erro_exit("number of meals must be more tan 0");
	}
	else
		data->num_meals = -1;
}

//./philo 5 450 200 200 -> pb 2 philo mangent en mm temps alors quil ne devraient pas
// time_to_die doit etre 3 fois sup au time_to_eat quand nb impair de philo
// qd nb pair time to die doit etre au moins 2 fois sup au temps time_to_eat