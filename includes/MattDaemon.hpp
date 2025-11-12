/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MattDaemon.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 14:36:38 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/12 15:30:59 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATTDAEMON_HPP
# define MATTDAEMON_HPP

# include "Tintin_reporter.hpp"
# include "Server.hpp"

# include <iostream>
# include <fstream>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/file.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <signal.h>
# include <string.h>

// Path to the lock file
# define LOCK_FILE "/var/lock/matt_daemon.lock"

/*
	Main class to handle the project (Owns logger, server and daemonizes itself)
*/
class	MattDaemon
{
	public:
		MattDaemon() {}
		~MattDaemon();

		MattDaemon(const MattDaemon &copy)
		{
			*this = copy;
		}
		MattDaemon	&operator=(const MattDaemon &copy)
		{
			if (this != &copy)
			{
				this->_isDaemon = copy._isDaemon;
				this->_lockFD = copy._lockFD;
				this->_running = copy._running;
				this->_logger = copy._logger;
				this->_server = copy._server;
			}
			return (*this);
		}

		void	start();
	private:
		Server			_server;
		Tintin_reporter	_logger;

		bool			_isDaemon = false;
		bool			_running = true;
		// FD to the lock file
		int				_lockFD = 0;

		bool	_init();
		void	_stop();

		//	Locks lock file
		void	_lock();
		//	Unlocks lock file
		void	_unlock();

		//	Checks if the program received signals
		bool	_checkSignals();
		static void	_sigHandler(int sig);

		//	Makes the program a daemon, returns wether the process needs to exit
		bool	_daemonize();
		// Setups all signal hooks to catch them
		void	_setupSignals();
		// Closes all std fd's (Thats what a daemon does)
		void	_closeFDs();
};

#endif
