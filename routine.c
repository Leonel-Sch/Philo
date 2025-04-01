/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 16:30:18 by lscheupl          #+#    #+#             */
/*   Updated: 2025/04/01 16:45:10 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philo.h"

void    handle_think_time(t_philo *philo)
{
    long long    think_time;

    think_time = 0;
    pthread_mutex_lock(&philo->data->meal_check);
	if ((philo->times_eaten > 0))
	{
		pthread_mutex_unlock(&philo->data->meal_check);
		pthread_mutex_lock(&philo->data->has_eaten);
		pthread_mutex_lock(&philo->data->get_time);
		think_time = get_time() - philo->last_meal;
		pthread_mutex_unlock(&philo->data->get_time);
		pthread_mutex_unlock(&philo->data->has_eaten);
		if (think_time + philo->data->time_to_eat < philo->data->time_to_die
			- philo->data->time_to_sleep)
		{
			usleep((philo->data->time_to_die - philo->data->time_to_sleep
					- think_time) * 500);
		}
	}
	else
	{
		pthread_mutex_unlock(&philo->data->meal_check);
	}
}

void	*routine(void *arg)
{
	t_philo		*philo;
	int			i;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(philo->data->time_to_eat * 500);
	while (1)
	{
		if (check_death(philo) == true)
			break ;
		philo_write(philo, "is thinking");
		
        handle_think_time(philo);

		if (check_death(philo) == true)
			return (NULL);
		if (philo->id % 2 == 1)
		{
			pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
			if (check_death(philo) == true)
				return (pthread_mutex_unlock(&philo->data->forks[philo->left_fork]),
					NULL);
			philo_write(philo, "has taken a fork");
			if (check_death(philo) == true)
				return (pthread_mutex_unlock(&philo->data->forks[philo->left_fork]),
					NULL);
			pthread_mutex_lock(&philo->data->forks[philo->right_fork]);
			if (check_death(philo) == true)
				return (pthread_mutex_unlock(&philo->data->forks[philo->left_fork]),
					pthread_mutex_unlock(&philo->data->forks[philo->right_fork]),
					NULL);
			philo_write(philo, "has taken a fork");
		}
		else
		{
			pthread_mutex_lock(&philo->data->forks[philo->right_fork]);
			if (check_death(philo) == true)
				return (pthread_mutex_unlock(&philo->data->forks[philo->right_fork]),
					NULL);
			philo_write(philo, "has taken a fork");
			pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
			if (check_death(philo) == true)
				return (pthread_mutex_unlock(&philo->data->forks[philo->right_fork]),
					pthread_mutex_unlock(&philo->data->forks[philo->left_fork]),
					NULL);
			philo_write(philo, "has taken a fork");
			if (check_death(philo) == true)
				return (pthread_mutex_unlock(&philo->data->forks[philo->right_fork]),
					pthread_mutex_unlock(&philo->data->forks[philo->left_fork]),
					NULL);
		}
		if (check_death(philo) == true)
			return (pthread_mutex_unlock(&philo->data->forks[philo->left_fork]),
				pthread_mutex_unlock(&philo->data->forks[philo->right_fork]),
				NULL);
		pthread_mutex_lock(&philo->data->has_eaten);
		pthread_mutex_lock(&philo->data->get_time);
		philo->last_meal = get_time();
		pthread_mutex_unlock(&philo->data->get_time);
		philo_write(philo, "is eating");
		pthread_mutex_lock(&philo->data->meal_check);
		philo->times_eaten++;
		pthread_mutex_unlock(&philo->data->meal_check);
		pthread_mutex_unlock(&philo->data->has_eaten);
		pthread_mutex_lock(&philo->data->meal_check);
		if (philo->data->must_eat != -1
			&& philo->times_eaten == philo->data->must_eat)
		{
			philo->data->eaten[philo->id - 1] = true;
			if (check_eaten(philo->data) == true)
			{
				pthread_mutex_lock(&philo->data->is_dead);
				philo->data->dead = true;
				pthread_mutex_unlock(&philo->data->is_dead);
			}
		}
		pthread_mutex_unlock(&philo->data->meal_check);
		usleep(philo->data->time_to_eat * 1000);
		pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
		pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
		if (check_death(philo) == true)
			break ;
		philo_write(philo, "is sleeping");
		i = 10;
		while (i > 0)
		{
			usleep(philo->data->time_to_sleep * 100);
			if (check_death(philo) == true)
				return (NULL);
			i--;
		}
	}
	return (NULL);
}