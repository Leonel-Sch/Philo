/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 16:37:39 by leonel            #+#    #+#             */
/*   Updated: 2025/04/01 16:41:57 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philo
{
	int				id;
	int				left_fork;
	int				right_fork;
	int				times_eaten;
	long long		last_meal;
	pthread_t		thread;
	struct s_data	*data;
}					t_philo;

typedef struct s_data
{
	long long		nb_philos;
	long long		time_to_die;
	long long		time_to_eat;
	long long		time_to_sleep;
	long long		must_eat;
	bool			dead;
	bool			*eaten;
	long long		start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	writing;
	pthread_mutex_t	meal_check;
	pthread_mutex_t	is_dead;
	pthread_mutex_t	has_eaten;
	pthread_mutex_t	get_time;
	pthread_mutex_t	is_something;
	t_philo			*philos;
}					t_data;

/* utils.c */
long long			ft_atoll(const char *str);
long long			get_time(void);
int					verif_validity(t_data *data, int argc);
int					init_mutex(t_data *data);

///* routine.c */
void				*routine(void *arg);

///* routine utils.c */
void				philo_write(t_philo *philo, char *str);
bool				is_starving(t_philo *philo);
bool				check_death(t_philo *philo);
bool				check_eaten(t_data *data);

///** routine_utils_2.c */
void				*routine_monitor(void *dat);
void				*routine_one(void *arg);
void				only_one_philo(t_data *data, int *i);
void				init_each_philo(t_data *data, int i);
bool				routine_is_dead(t_data *data);

#endif
