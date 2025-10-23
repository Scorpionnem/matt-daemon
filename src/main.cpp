/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 18:06:35 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/23 14:22:54 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Tintin_reporter.hpp"
#include "MattDaemon.hpp"
#include "Server.hpp"

#define PROD 0

static void	startServer(MattDaemon &mattDaemon, Server &server)
{
	try {
		server.start();
	} catch (const std::exception &e) {
		Tintin_reporter::log(LogType::ERROR, "Failed to start server.");
		mattDaemon.stop();
		throw ;
	}
}

static void	loop(MattDaemon &mattDaemon, Server &server)
{
	while (1)
	{
		if (mattDaemon.receivedSignal())
			break ;

		if (!server.receive())
			break ;
	}
}

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
		Tintin_reporter::getInstance();
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return (1);
	}

	try {
		MattDaemon	mattDaemon;
		Server		server;
	
		mattDaemon.start();
		
		startServer(mattDaemon, server);

		loop(mattDaemon, server);

		server.stop();
		mattDaemon.stop();
		Tintin_reporter::log(LogType::INFO, "Quitting.");
	} catch (const std::exception &e) {
		return (1);
	}
}
