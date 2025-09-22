/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MattDaemon.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 14:37:51 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/22 15:00:08 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MattDaemon.hpp"
#include "Tintin_reporter.hpp"

int	g_signal = -1;

static void	sigHandler(int sig)
{
	g_signal = sig;
}

void	MattDaemon::_setupSignals()
{
	signal(SIGINT, sigHandler);
	signal(SIGTERM, sigHandler);
}

bool	MattDaemon::receivedSignal()
{
	if (g_signal < 0)
		return (false);
	Tintin_reporter::log(LogType::INFO, "Received signal: " + std::to_string(g_signal));
	return (true);
}

void	MattDaemon::start()
{
	_daemonize();
	_lockFile();
}

void	MattDaemon::stop()
{
	_unlockFile();
}

void	MattDaemon::_daemonize()
{
	Tintin_reporter::log(LogType::INFO, "Entering daemon mode.");

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
		Tintin_reporter::log(LogType::INFO, "Failed to mount to '/'" + std::to_string(getpid()));
		exit(1);
	}

	_setupSignals();
	_closeFDs();

	Tintin_reporter::log(LogType::INFO, "Entered daemon mode, PID: " + std::to_string(getpid()));
}

void	MattDaemon::_closeFDs()
{
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}

void	MattDaemon::_lockFile()
{
	_lockFD = open(LOCK_FILE, O_RDWR | O_CREAT, 0640);
	if (_lockFD < 0)
	{
		Tintin_reporter::log(LogType::ERROR, "Failed to open lock file");
		exit(1);
	}
	if (flock(_lockFD, LOCK_EX | LOCK_NB) < 0)
	{
		Tintin_reporter::log(LogType::ERROR, "A daemon is already running");
		exit(1);
	}

	Tintin_reporter::log(LogType::INFO, std::string("Locked ") + LOCK_FILE);
}

void	MattDaemon::_unlockFile()
{
	close(_lockFD);
	remove(LOCK_FILE);
	Tintin_reporter::log(LogType::INFO, std::string("Unlocked ") + LOCK_FILE);
}
