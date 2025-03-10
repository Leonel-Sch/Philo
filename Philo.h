/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 16:37:39 by leonel            #+#    #+#             */
/*   Updated: 2025/03/10 17:35:55 by lscheupl         ###   ########.fr       */
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
# include <stdbool.h>

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
	bool				dead;
	bool 				*eaten;
	long long		start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	writing;
	pthread_mutex_t	meal_check;
	pthread_mutex_t	is_dead;
	pthread_mutex_t	has_eaten;
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

///* routine utils.c */
void    philo_write(t_philo *philo, char *str);
bool    is_starving(t_philo *philo);

#endif
