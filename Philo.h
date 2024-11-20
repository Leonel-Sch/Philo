/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 16:37:39 by leonel            #+#    #+#             */
/*   Updated: 2024/11/20 16:21:52 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
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
	int				nb_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat;
	int				dead;
	long long		start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	writing;
	pthread_mutex_t	meal_check;
	t_philo			*philos;
}					t_data;

/* utils.c */
int					ft_atoi(const char *str);
long long			get_time(void);
//void				ft_usleep(long long time);
//void				print_message(t_data *data, int id, char *msg);

///* init.c */
//int					init_data(t_data *data, int argc, char **argv);
//int					init_mutex(t_data *data);
//int					init_philos(t_data *data);

///* routine.c */
//void				*philo_routine(void *arg);
//void				eat(t_philo *philo);
//void				check_death(t_data *data, t_philo *philos);

#endif
