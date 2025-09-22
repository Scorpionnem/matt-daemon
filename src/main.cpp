/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 18:06:35 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/22 15:01:42 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Tintin_reporter.hpp"
#include "MattDaemon.hpp"

# define PROD 0

int	main(void)
{
	#if PROD
		if (getuid() != 0)
		{
			std::cerr << "You are not root." << std::endl;
			return (1);
		}
	#endif

	try {
		Tintin_reporter::log(LogType::INFO, "Starting.");
		
		MattDaemon	mattDaemon;
	
		mattDaemon.start();
	
		int i = 0;

		while (i++ < 10)
		{
			if (mattDaemon.receivedSignal())
				break ;

			Tintin_reporter::log(LogType::LOG, "Logging");
			sleep(1);
		}

		mattDaemon.stop();
	
		Tintin_reporter::log(LogType::INFO, "Quitting.");
	} catch (const std::exception &e) {
		return (1);
	}
}
