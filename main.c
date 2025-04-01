/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 16:35:47 by leonel            #+#    #+#             */
/*   Updated: 2025/04/01 16:39:28 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philo.h"

int	ft_initiate(char **argv, int argc, t_data *data)
{
	int	i;

	i = 0;
	data->nb_philos = ft_atoll(argv[1]);
	data->time_to_die = ft_atoll(argv[2]);
	data->time_to_eat = ft_atoll(argv[3]);
	data->time_to_sleep = ft_atoll(argv[4]);
	if (argc == 6)
		data->must_eat = ft_atoll(argv[5]);
	else
		data->must_eat = -1;
	if (verif_validity(data, argc) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	data->dead = false;
	data->start_time = get_time();
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_philos);
	if (!data->forks)
		return (printf("Error: malloc failed\n"), EXIT_FAILURE);
	data->eaten = malloc(sizeof(bool) * data->nb_philos);
	if (!data->eaten)
		return (printf("Error: malloc failed\n"), free(data->forks), 1);
	while (i < data->nb_philos)
		data->eaten[i++] = false;
	init_mutex(data);
	return (EXIT_SUCCESS);
}

int	ft_create_philos(t_data *data)
{
	int			i;
	pthread_t	monitor;

	i = 0;
	data->philos = malloc(sizeof(t_philo) * data->nb_philos);
	if (!data->philos)
		return (printf("Error: malloc failed\n"), EXIT_FAILURE);
	if (data->nb_philos == 1)
		only_one_philo(data, &i);
	while (i < data->nb_philos)
	{
		init_each_philo(data, i);
		pthread_create(&data->philos[i].thread, NULL, routine,
			&data->philos[i]);
		i++;
	}
	pthread_create(&monitor, NULL, routine_monitor, data);
	i = 0;
	while (i < data->nb_philos)
		pthread_join(data->philos[i++].thread, NULL);
	pthread_join(monitor, NULL);
	return (EXIT_SUCCESS);
}

void	ft_destroy(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data->writing);
	pthread_mutex_destroy(&data->meal_check);
	pthread_mutex_destroy(&data->is_dead);
	pthread_mutex_destroy(&data->has_eaten);
	pthread_mutex_destroy(&data->get_time);
	free(data->forks);
	free(data->philos);
	free(data->eaten);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc < 5 || argc > 6)
		return (EXIT_FAILURE);
	if (ft_initiate(argv, argc, &data) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	ft_create_philos(&data);
	ft_destroy(&data);
	return (EXIT_SUCCESS);
}
