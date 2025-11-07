/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MattDaemon.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 14:37:51 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/07 16:36:18 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MattDaemon.hpp"

// MAIN LOOP
void	MattDaemon::start()
{
	if (!_init())
		return ;
	/*
		This is the main loop of the program, the server should have no such loop.
	*/
	while (_running)
	{
		if (_checkSignals())
			break ;

		/*
			Here the runtime function of the server is called, it takes the logger by reference so it can log things by itself
			The runtime function should accept clients, let them disconnect and send/receive informations.
			If it needs to tell the daemon that it needs to stop we can just throw an exception for that.

			The server should be the one handling the remote shell if we do it
			It also handles everything about clients, the daemon only knows that it has a server running in it and wether it needs to stop it or not
		*/

		// try {
		// 	_server.runtime(_logger);
		// } catch (const Server::StopExcept &e) {
			// _logger.log(LogType::INFO, "Received call to stop server: " + std::string(e.what()));
		// 	break ;
		// } catch (const std::exception &e) {
				// _logger.log(LogType::ERROR, std::string(e.what()));
		// }
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

	/*
		Here the server is started, it will open socket, set the poll and all
		All protected by a catch (Better if everything is logged in the log file.)
		The logger is given by reference to the start function so it can log
	*/

	// try {
	// 	_server.start(_logger);
	// } catch (const std::exception &e) {
	// 	_logger.log(LogType::ERROR, std::string(e.what()));
	// 	return (false);
	// }
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
