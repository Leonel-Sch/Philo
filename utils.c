/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 17:02:51 by leonel            #+#    #+#             */
/*   Updated: 2025/04/01 16:36:28 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philo.h"

long long	ft_atoll(const char *str)
{
	int			i;
	long long	sign;
	long long	res;

	i = 0;
	sign = 1;
	res = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\v'
		|| str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + str[i] - '0';
		i++;
	}
	return (res * sign);
}

int	verif_validity(t_data *data, int argc)
{
	if (data->nb_philos < 1 || data->nb_philos > 200)
		return (printf("Error: number of philosophers\n"), 1);
	if (data->time_to_die <= 0)
		return (printf("Error: time to die must be more than zero\n"), 1);
	if (data->time_to_die > 2147483647)
		return (printf("Error: time to die must be under INT_MAX\n"), 1);
	if (data->time_to_eat <= 0)
		return (printf("Error: time to eat must be more than zero\n"), 1);
	if (data->time_to_eat > 2147483647)
		return (printf("Error: time to eat must be under INT_MAX\n"), 1);
	if (data->time_to_sleep <= 0)
		return (printf("Error: time to sleep must be more than zero\n"), 1);
	if (data->time_to_sleep > 2147483647)
		return (printf("Error: time to sleep must be under INT_MAX\n"), 1);
	if (argc == 6)
	{
		if (data->must_eat <= 0)
			return (printf("Error: must eat must be more than zero\n"), 1);
		if (data->must_eat > 2147483647)
			return (printf("Error: must eat must be under INT_MAX\n"), 1);
	}
	return (EXIT_SUCCESS);
}

int	init_mutex(t_data *data)
{
	int	i;

	i = 0;
	pthread_mutex_init(&data->writing, NULL);
	pthread_mutex_init(&data->meal_check, NULL);
	pthread_mutex_init(&data->is_dead, NULL);
	pthread_mutex_init(&data->has_eaten, NULL);
	pthread_mutex_init(&data->get_time, NULL);
	while (i < data->nb_philos)
	{
		pthread_mutex_init(&data->forks[i++], NULL);
	}
	return (EXIT_SUCCESS);
}

long long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}
