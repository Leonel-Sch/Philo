/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leonel <leonel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 16:09:11 by lscheupl          #+#    #+#             */
/*   Updated: 2025/03/28 17:15:39 by leonel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philo.h"

void    philo_write(t_philo *philo, char *str)
{
    pthread_mutex_lock(&philo->data->writing);
    
    printf("%lld Philosopher %d %s\n", get_time() - philo->data->start_time, philo->id, str);
    
    pthread_mutex_unlock(&philo->data->writing);
}

bool    is_starving(t_philo *philo)
{
    pthread_mutex_lock(&philo->data->get_time);
    if (get_time() - philo->last_meal >= philo->data->time_to_die)
	{
        philo_write(philo, "died");
		philo->data->dead = true;
		return (pthread_mutex_unlock(&philo->data->get_time), pthread_mutex_unlock(&philo->data->is_dead), true);
	}
    return (pthread_mutex_unlock(&philo->data->get_time), pthread_mutex_unlock(&philo->data->is_dead), false);
}