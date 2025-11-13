/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 18:06:35 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/13 16:15:31 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Tintin_reporter.hpp"
#include "MattDaemon.hpp"

int	main(void)
{
	if (getuid() != 0)
	{
		std::cerr << "Program can only be run as root!" << std::endl;
		return (1);
	}

	MattDaemon	mattDaemon;

	mattDaemon.start();
}
