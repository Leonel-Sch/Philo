/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 16:09:11 by lscheupl          #+#    #+#             */
/*   Updated: 2025/04/01 16:34:53 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philo.h"

void	philo_write(t_philo *philo, char *str)
{
	pthread_mutex_lock(&philo->data->writing);
	printf("%lld %d %s\n", get_time() - philo->data->start_time, philo->id,
		str);
	pthread_mutex_unlock(&philo->data->writing);
}

bool	is_starving(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->get_time);
	if (get_time() - philo->last_meal >= philo->data->time_to_die)
	{
		philo_write(philo, "died");
		philo->data->dead = true;
		return (pthread_mutex_unlock(&philo->data->get_time),
			pthread_mutex_unlock(&philo->data->is_dead), true);
	}
	return (pthread_mutex_unlock(&philo->data->get_time),
		pthread_mutex_unlock(&philo->data->is_dead), false);
}

bool	check_death(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->is_dead);
	if (philo->data->dead == true)
		return (pthread_mutex_unlock(&philo->data->is_dead), true);
	return (pthread_mutex_unlock(&philo->data->is_dead), false);
}

bool	check_eaten(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		if (data->eaten[i] == false)
			return (false);
		i++;
	}
	return (true);
}
