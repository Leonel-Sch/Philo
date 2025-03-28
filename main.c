/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leonel <leonel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 16:35:47 by leonel            #+#    #+#             */
/*   Updated: 2025/03/28 17:27:34 by leonel           ###   ########.fr       */
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
	pthread_mutex_init(&data->get_time, NULL);
}
void	ft_display_data(t_data *data)
{
	printf("Start time: %lld\n", data->start_time - get_time());
	printf("Number of philosophers: %d\n", data->nb_philos);
	printf("Time to die: %lld\n", data->time_to_die);
	printf("Time to eat: %lld\n", data->time_to_eat);
	printf("Time to sleep: %lld\n", data->time_to_sleep);
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

bool	check_death(t_philo *philo)
{
	// dprintf(2, "check_death philo : %d\n", philo->id);
	pthread_mutex_lock(&philo->data->is_dead);
	// dprintf(2, "prout\n");
	if (philo->data->dead == true)
		return (pthread_mutex_unlock(&philo->data->is_dead), true);
	return (pthread_mutex_unlock(&philo->data->is_dead), false);
}
void *routine_monitor(void *dat)
{
	int i;

	i = 0;
	t_data *data;
	data = (t_data *)dat;
	while (1)
	{
		usleep(1000);
		pthread_mutex_lock(&data->is_dead);
		if (data->dead == true)
			return (pthread_mutex_unlock(&data->is_dead), NULL);
		pthread_mutex_unlock(&data->is_dead);
		while (i <= (data->nb_philos - 1))
		{
			pthread_mutex_lock(&data->is_dead);
			if (is_starving(&data->philos[i]) == true)
				return (NULL);
			i++;
			pthread_mutex_lock(&data->is_dead);
			if (data->dead == true)
				return (pthread_mutex_unlock(&data->is_dead), NULL);
			pthread_mutex_unlock(&data->is_dead);
		}
		i = 0;
		pthread_mutex_lock(&data->is_dead);
		if (data->dead == true)
			return (pthread_mutex_unlock(&data->is_dead), NULL);
		pthread_mutex_unlock(&data->is_dead);
	}
	return (NULL);
}

// void monitor(t_data *data)
// {
// 	printf("proutmachin\n");
// 	pthread_t monitor;
// 	pthread_create(&monitor, NULL, routine_monitor, data);
// 	pthread_join(monitor, NULL);
// }

void	*routine(void *arg)
{
	t_philo *philo;
	int i;
	// long long think_time;

	philo = (t_philo *)arg;

	while (1)
	{
		if (check_death(philo) == true)
			break;
		
		philo_write(philo, "is thinking");
		
		// pthread_mutex_lock(&philo->data->has_eaten);
		// think_time = philo->data->time_to_die - (get_time() - philo->last_meal) - (philo->data->time_to_eat / 2);
		// dprintf(2, "%lld\n", think_time);
		// pthread_mutex_unlock(&philo->data->has_eaten);
		// if (think_time <= -100)
		// {
		// 	while (1)
		// 	{
		// 		usleep(10);
		// 		if (check_death(philo) == true)
		// 			return (NULL);
		// 	}
		// }
		// else
		// {
		// 	while(think_time > 50)
		// 	{
		// 		think_time = think_time / 7;
		// 	}
		//usleep(think_time);
		// }
		// pthread_mutex_lock(&philo->data->get_time);
		// usleep(get_time() % 16384);
		// pthread_mutex_unlock(&philo->data->get_time);
		
		if (check_death(philo) == true)
			return (NULL);

		if (philo->id % 2 == 1)
		{
			pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
		
			if (check_death(philo) == true)
				return (pthread_mutex_unlock(&philo->data->forks[philo->left_fork]), NULL);
			
			pthread_mutex_lock(&philo->data->writing);
			printf("%lld Philosopher %d is taking left fork %d\n", philo->data->start_time, philo->id, philo->left_fork);
			pthread_mutex_unlock(&philo->data->writing);
			
			if (check_death(philo) == true)
				return (pthread_mutex_unlock(&philo->data->forks[philo->left_fork]), NULL);

			pthread_mutex_lock(&philo->data->forks[philo->right_fork]);
		
		
			if (check_death(philo) == true)
				return (pthread_mutex_unlock(&philo->data->forks[philo->left_fork]), pthread_mutex_unlock(&philo->data->forks[philo->right_fork]), NULL);

			pthread_mutex_lock(&philo->data->writing);
			printf("%lld Philosopher %d is taking right fork %d\n", philo->data->start_time, philo->id, philo->right_fork);
			pthread_mutex_unlock(&philo->data->writing);
		}
		else
		{
			pthread_mutex_lock(&philo->data->forks[philo->right_fork]);
		
		
			if (check_death(philo) == true)
				return (pthread_mutex_unlock(&philo->data->forks[philo->right_fork]), NULL);

			pthread_mutex_lock(&philo->data->writing);
			printf("%lld Philosopher %d is taking right fork %d\n", philo->data->start_time, philo->id, philo->right_fork);
			pthread_mutex_unlock(&philo->data->writing);
			
			pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
		
			if (check_death(philo) == true)
				return (pthread_mutex_unlock(&philo->data->forks[philo->right_fork]), pthread_mutex_unlock(&philo->data->forks[philo->left_fork]), NULL);
		
			pthread_mutex_lock(&philo->data->writing);
			printf("%lld Philosopher %d is taking left fork %d\n", philo->data->start_time, philo->id, philo->left_fork);
			pthread_mutex_unlock(&philo->data->writing);

			if (check_death(philo) == true)
				return (pthread_mutex_unlock(&philo->data->forks[philo->right_fork]), pthread_mutex_unlock(&philo->data->forks[philo->left_fork]), NULL);
		}
		
		if (check_death(philo) == true)
			return (pthread_mutex_unlock(&philo->data->forks[philo->left_fork]), pthread_mutex_unlock(&philo->data->forks[philo->right_fork]), NULL);

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
		
		if (philo->data->must_eat != -1 && philo->times_eaten == philo->data->must_eat)
		{
			philo->data->eaten[philo->id - 1] = true;
			if (check_eaten(philo->data) == true)
			{
				pthread_mutex_lock(&philo->data->is_dead);
				philo->data->dead = true;
				pthread_mutex_unlock(&philo->data->is_dead);
				// dprintf(2, "eaten\n");
			}
		}
		
		pthread_mutex_unlock(&philo->data->meal_check);
		
		
		usleep(philo->data->time_to_eat * 1000);
		pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
		pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
		
		if (check_death(philo) == true)
			break;
		
		philo_write(philo, "is sleeping");
		
		i = 10;
		while (i > 0)
		{
			usleep(philo->data->time_to_sleep * 100);
			if (check_death(philo) == true)
				return (NULL);
			i--;
		}
		// dprintf(2, "up\n");
	}
	return (NULL);
}

void	*routine_one(void *arg)
{
	t_philo *philo;

	philo = (t_philo *)arg;
	while(1)
	{
		printf("%lld Philosopher 1 is thinking\n", get_time() - philo->data->start_time);
		printf("%lld Philosopher 1 is taking left fork 0\n", get_time() - philo->data->start_time);
		usleep(philo->data->time_to_die * 1000);
		printf("%lld Philosopher 1 died\n", get_time() - philo->data->start_time);
	}
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
		pthread_create(&data->philos[i].thread, NULL, routine_one, &data->philos[i]);
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
	pthread_t monitor;
	pthread_create(&monitor, NULL, routine_monitor, data);
	// monitor(data);
	i = 0;
	while (i < data->nb_philos)
		pthread_join(data->philos[i++].thread, NULL);
	pthread_join(monitor, NULL);
}

void 	ft_destroy(t_data *data)
{
	int i;

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

int main(int argc, char **argv)
{
	t_data data;
	// int i;
	
	// i = 0;
	if (argc < 5 || argc > 6)
		return (EXIT_FAILURE);
	ft_initiate(argv, argc, &data);
	ft_display_data(&data);
	ft_create_philos(&data);
	ft_destroy(&data);
	return (EXIT_SUCCESS);
}
