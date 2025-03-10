/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 16:35:47 by leonel            #+#    #+#             */
/*   Updated: 2025/03/10 17:59:23 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philo.h"

long long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	ft_initiate(char **argv, int argc, t_data *data)
{
	data->nb_philos = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->must_eat = ft_atoi(argv[5]);
	else
		data->must_eat = -1;
	data->dead = false;
	data->start_time = get_time();
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_philos);
	if (!data->forks)
	{
		printf("Error: malloc failed\n");
		exit(EXIT_FAILURE);
	}
	pthread_mutex_init(&data->writing, NULL);
	pthread_mutex_init(&data->meal_check, NULL);
	pthread_mutex_init(&data->is_dead, NULL);
	int i = 0;
	while (i < data->nb_philos)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}
	data->eaten = malloc(sizeof(bool) * data->nb_philos);
	if (!data->eaten)
	{
		printf("Error: malloc failed\n");
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (i < data->nb_philos)
	{
		data->eaten[i] = false;
		i++;
	}
	pthread_mutex_init(&data->has_eaten, NULL);
}
void	ft_display_data(t_data *data)
{
	printf("Start time: %lld\n", data->start_time - get_time());
	printf("Number of philosophers: %d\n", data->nb_philos);
	printf("Time to die: %d\n", data->time_to_die);
	printf("Time to eat: %d\n", data->time_to_eat);
	printf("Time to sleep: %d\n", data->time_to_sleep);
	if (data->must_eat != -1)
		printf("Number of times each philosopher must eat: %d\n", data->must_eat);
	else
		printf("Number of times each philosopher must eat: Infinite\n");
}

bool	check_eaten(t_data *data)
{
	int i;

	i = 0;
	while (i < data->nb_philos)
	{
		if (data->eaten[i] == false)
			return (false);
		i++;
	}
	return (true);
}

void	*routine(void *arg)
{
	t_philo *philo;

	philo = (t_philo *)arg;

	while (1)
	{
		
		philo_write(philo, "is thinking");
		
		pthread_mutex_lock(&philo->data->has_eaten);
		if (philo->id % 2 == 1)
		{
			pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
			pthread_mutex_lock(&philo->data->writing);
			printf("%lld Philosopher %d is taking left fork %d\n", get_time() - philo->data->start_time, philo->id, philo->left_fork);
			pthread_mutex_unlock(&philo->data->writing);

			pthread_mutex_lock(&philo->data->forks[philo->right_fork]);
			pthread_mutex_lock(&philo->data->writing);
			printf("%lld Philosopher %d is taking right fork %d\n", get_time() - philo->data->start_time, philo->id, philo->right_fork);
			pthread_mutex_unlock(&philo->data->writing);
		}
		else
		{
			pthread_mutex_lock(&philo->data->forks[philo->right_fork]);
			pthread_mutex_lock(&philo->data->writing);
			printf("%lld Philosopher %d is taking right fork %d\n", get_time() - philo->data->start_time, philo->id, philo->right_fork);
			pthread_mutex_unlock(&philo->data->writing);
			
			pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
			pthread_mutex_lock(&philo->data->writing);
			printf("%lld Philosopher %d is taking left fork %d\n", get_time() - philo->data->start_time, philo->id, philo->left_fork);
			pthread_mutex_unlock(&philo->data->writing);
		}
		pthread_mutex_unlock(&philo->data->has_eaten);
		
		if (philo->data->dead == true)
			return (pthread_mutex_unlock(&philo->data->forks[philo->left_fork]), pthread_mutex_unlock(&philo->data->forks[philo->right_fork]), NULL);
		if (is_starving(philo) == true)
			return (pthread_mutex_unlock(&philo->data->forks[philo->left_fork]), pthread_mutex_unlock(&philo->data->forks[philo->right_fork]), NULL);
		if (philo->data->dead == true)
			return (pthread_mutex_unlock(&philo->data->forks[philo->left_fork]), pthread_mutex_unlock(&philo->data->forks[philo->right_fork]), NULL);
		
		philo_write(philo, "is eating");
		philo->last_meal = get_time();
		philo->times_eaten++;
		usleep(philo->data->time_to_eat * 1000);
		pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
		pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
		if (philo->data->dead == true)
			break;
		if (is_starving(philo) == true)
			break;
		if (philo->data->dead == true)
			break;
		
		philo_write(philo, "is sleeping");
		usleep(philo->data->time_to_sleep * 1000);
			
		if (philo->data->dead == true)
			break;
		if (is_starving(philo) == true)
			break;
		if (philo->data->dead == true)
			break;
	}
	return (NULL);
}

void ft_create_philos(t_data *data)
{
	int i;

	i = 0;
	data->philos = malloc(sizeof(t_philo) * data->nb_philos);
	if (!data->philos)
	{
		printf("Error: malloc failed\n");
		exit(EXIT_FAILURE);
	}
	if (data->nb_philos == 1)
	{
		printf("%lld Philosopher 1 is thinking\n", get_time() - data->start_time);
		printf("%lld Philosopher 1 is taking left fork 0\n", get_time() - data->start_time);
		usleep(data->time_to_die * 1000);
		printf("%lld Philosopher 1 died\n", get_time() - data->start_time);
		return ;
	}
	while (i < data->nb_philos)
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
		pthread_create(&data->philos[i].thread, NULL, routine, &data->philos[i]);
		i++;
	}
	i = 0;
	while (i < data->nb_philos)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
}

int main(int argc, char **argv)
{
	t_data data;
	if (argc < 5 || argc > 6)
		return (EXIT_FAILURE);
	ft_initiate(argv, argc, &data);
	ft_display_data(&data);
	ft_create_philos(&data);
	return (EXIT_SUCCESS);
}
