/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 18:06:35 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/21 20:07:53 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Tintin_reporter.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>

void	loop()
{
	for (int i = 0; i < 10; i++)
	{
		Tintin_reporter::log(LogType::INFO, "logging things");
		sleep(1);
	}
}

void	daemonize()
{
	pid_t	pid;

	pid = fork();
	if (pid > 0)
		exit(1);
	
	setsid();

	pid = fork();
	if (pid > 0)
		exit(1);

	chdir("/");

	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	#define LOCK_FILE "/var/lock/matt_daemon.lock"
	int lock_fd = open(LOCK_FILE, O_RDWR | O_CREAT, 0640);
	if (lock_fd < 0)
	{
		Tintin_reporter::log(LogType::INFO, "failed to open lock file");
		exit(EXIT_FAILURE);
	}
	if (flock(lock_fd, LOCK_EX | LOCK_NB) < 0)
	{
		Tintin_reporter::log(LogType::INFO, "lock already locked");
		exit(EXIT_FAILURE);
	}

	loop();

	close(lock_fd);
}

int	main(void)
{
	try {
		Tintin_reporter::log(LogType::INFO, "Starting...");
	} catch (const std::exception& e) {
		std::cerr << "Logger error: " << e.what() << "\n";
		return (1);
	}

	daemonize();
	Tintin_reporter::log(LogType::INFO, "leaving.");
}
