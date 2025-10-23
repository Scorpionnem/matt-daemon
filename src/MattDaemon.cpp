/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MattDaemon.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 14:37:51 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/23 15:38:41 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MattDaemon.hpp"
#include "Tintin_reporter.hpp"

int	g_signal = -1;

static void	sigHandler(int sig)
{
	g_signal = sig;
}

bool	MattDaemon::_checkSignals()
{
	if (g_signal != -1)
	{
		_logger.log(LogType::INFO, "Received signal: " + std::to_string(g_signal));
		return (true);
	}
	return (false);
}

void	MattDaemon::_setupSignals()
{
	signal(SIGINT, sigHandler);
	signal(SIGTERM, sigHandler);
}

void	MattDaemon::_daemonize()
{
	_logger.log(LogType::INFO, "Entering daemon mode.");

	pid_t	pid;

	pid = fork();
	if (pid > 0)
		exit(0);
	if (pid < 0)
		exit(1);

	setsid();

	pid = fork();
	if (pid > 0)
		exit(0);
	if (pid < 0)
		exit(1);

	if (chdir("/") == -1)
	{
		_logger.log(LogType::INFO, "Failed to mount to '/'" + std::to_string(getpid()));
		exit(1);
	}

	_setupSignals();
	_closeFDs();

	_logger.log(LogType::INFO, "Entered daemon mode, PID: " + std::to_string(getpid()));
}

void	MattDaemon::_closeFDs()
{
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}

void	MattDaemon::_lock()
{
	_lockFD = open(LOCK_FILE, O_RDWR | O_CREAT, 0640);
	if (_lockFD < 0)
		throw std::runtime_error("Failed to open lock file");
	if (flock(_lockFD, LOCK_EX | LOCK_NB) < 0)
		throw std::runtime_error("Failed to lock " LOCK_FILE);
}

void	MattDaemon::_unlock()
{
	close(_lockFD);
	remove(LOCK_FILE);
	_logger.log(LogType::INFO, std::string("Unlocked ") + LOCK_FILE);
}
