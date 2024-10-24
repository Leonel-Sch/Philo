/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leonel <leonel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 16:35:47 by leonel            #+#    #+#             */
/*   Updated: 2024/10/24 11:42:58 by leonel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philo.h"

void	ft_initiate(char **argv, int argc, t_info *info)
{
	info->t_to_death = ft_atoi(argv[2]);
	info->t_to_eat = ft_atoi(argv[3]);
	info->t_to_sleep = ft_atoi(argv[4]);
	info->philos = ft_atoi(argv[1]);
	if (argc == 6)
		info->n_of_meal = ft_atoi(argv[5]);
	else
		info->n_of_meal = -1;
	info->fork = info->philos;
}

int main(int argc, char argv)
{
	t_info info;
	if (argc < 5 || argc > 6)
		return (EXIT_FAILURE);
	ft_initiate(argv, argc, &info);
	return (EXIT_SUCCESS);
}

void 