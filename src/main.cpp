/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 18:06:35 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/14 08:51:47 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define PROD 1

#include "Tintin_reporter.hpp"
#include "MattDaemon.hpp"

int	main(void)
{
	#if PROD == 1
		if (getuid() != 0)
		{
			std::cerr << "Program can only be run as root!" << std::endl;
			return (1);
		}
	#endif

	MattDaemon	mattDaemon;

	mattDaemon.start();
}
