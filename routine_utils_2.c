/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 16:37:16 by lscheupl          #+#    #+#             */
/*   Updated: 2025/04/01 16:39:34 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philo.h"

bool	routine_is_dead(t_data *data)
{
	pthread_mutex_lock(&data->is_dead);
	if (data->dead == true)
		return (pthread_mutex_unlock(&data->is_dead), true);
	return (pthread_mutex_unlock(&data->is_dead), false);
}

void	*routine_monitor(void *dat)
{
	int		i;
	t_data	*data;

	i = 0;
	data = (t_data *)dat;
	while (1)
	{
		usleep(5000);
		if (routine_is_dead(data) == true)
			return (NULL);
		while (i <= (data->nb_philos - 1))
		{
			pthread_mutex_lock(&data->is_dead);
			if (is_starving(&data->philos[i]) == true)
				return (NULL);
			i++;
			if (routine_is_dead(data) == true)
				return (NULL);
		}
		i = 0;
		if (routine_is_dead(data) == true)
			return (NULL);
	}
	return (NULL);
}

void	*routine_one(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		philo_write(philo, "is thinking");
		pthread_mutex_lock(&philo->data->forks[philo->right_fork]);
		philo_write(philo, "has taken a fork");
		if (check_death(philo) == true)
			return (pthread_mutex_unlock(&philo->data->forks[0]), NULL);
		else
		{
			while (1)
			{
				usleep(10);
				if (check_death(philo) == true)
					return (pthread_mutex_unlock(&philo->data->forks[0]), NULL);
			}
		}
	}
}

void	only_one_philo(t_data *data, int *i)
{
	data->philos[0].id = 1;
	data->philos[0].times_eaten = 0;
	data->philos[0].last_meal = get_time();
	data->philos[0].data = data;
	data->philos[0].left_fork = -1;
	data->philos[0].right_fork = 0;
	pthread_create(&data->philos[0].thread, NULL, routine_one,
		&data->philos[0]);
	*i = 1;
}

void	init_each_philo(t_data *data, int i)
{
	data->philos[i].id = i + 1;
	data->philos[i].times_eaten = 0;
	data->philos[i].last_meal = get_time();
	data->philos[i].data = data;
	if (i == 0)
		data->philos[i].left_fork = data->nb_philos - 1;
	else
		data->philos[i].left_fork = i - 1;
	data->philos[i].right_fork = i;
}
