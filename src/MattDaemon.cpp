/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MattDaemon.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 14:37:51 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/23 17:57:56 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MattDaemon.hpp"


// MAIN LOOP
void	MattDaemon::start()
{
	if (!_init())
		return ;
	while (_running)
	{
		if (_checkSignals())
			break ;

		// Server update (Server owns the remote shell and is given a reference to the logger)
	}
	_stop();
}


// INITIALIZATION / END
bool	MattDaemon::_init()
{
	try {
		_lock();
		_logger.init();
		_logger.log(LogType::INFO, "Locked " LOCK_FILE);
	} catch (const std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return (false);
	}

	try {
		_logger.log(LogType::INFO, "Starting.");
		_daemonize();
	} catch (const std::exception &e) {
		_logger.log(LogType::ERROR, std::string(e.what()));
		return (false);
	}
	return (true);
}

void	MattDaemon::_stop()
{
	_logger.log(LogType::INFO, "Stopping...");
	_unlock();
}


// SIGNALS
int	g_signal = -1;

void	MattDaemon::_sigHandler(int sig)
{
	g_signal = sig;
}

bool	MattDaemon::_checkSignals()
{
	if (g_signal != -1)
	{
		_logger.log(LogType::INFO, "Received signal: " + std::to_string(g_signal) + " (" + strsignal(g_signal) + ")"); // strsignal leaks internally, not fixable
		return (true);
	}
	return (false);
}


// LOCK FILE HANDLING
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


// DAEMONIZATION
void	MattDaemon::_setupSignals()
{
	signal(SIGABRT, _sigHandler);
	signal(SIGINT, _sigHandler);
	signal(SIGQUIT, _sigHandler);
	signal(SIGSTOP, _sigHandler);
	signal(SIGKILL, _sigHandler);
	signal(SIGTSTP, _sigHandler);
	signal(SIGCONT, _sigHandler);
	signal(SIGTERM, _sigHandler);
	_logger.log(LogType::INFO, "Now catching signals");
}

void	MattDaemon::_daemonize()
{
	_logger.log(LogType::INFO, "Entering daemon mode.");

	pid_t	pid;

	pid = fork();
	if (pid > 0)
		exit(0);
	if (pid < 0)
		throw std::runtime_error("Failed to fork");

	setsid();

	pid = fork();
	if (pid > 0)
		exit(0);
	if (pid < 0)
		throw std::runtime_error("Failed to fork");

	if (chdir("/") == -1)
		throw std::runtime_error("Failed to mount to '/'");

	_setupSignals();
	_closeFDs();

	_logger.log(LogType::INFO, "Entered daemon mode, PID: " + std::to_string(getpid()));
}

void	MattDaemon::_closeFDs()
{
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	_logger.log(LogType::INFO, "Closed std file descriptors");
}
