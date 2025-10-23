/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MattDaemon.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 14:36:38 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/23 15:35:36 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATTDAEMON_HPP
# define MATTDAEMON_HPP

# include "Tintin_reporter.hpp"

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

# define LOCK_FILE "/var/lock/matt_daemon.lock"

class	MattDaemon
{
	public:
		MattDaemon() {}
		~MattDaemon() {}

		void	start()
		{
			if (!_init())
				return ;
			while (_running)
			{
				if (_checkSignals())
					break ;
			}
			_stop();
		}
	private:
		Tintin_reporter	_logger;
		
		bool			_running = true;
		int				_lockFD = 0;

		bool	_checkSignals();

		bool	_init()
		{
			try {
				_logger.init();
				_lock();
			} catch (const std::exception &e) {
				std::cerr << "Error: " << e.what() << std::endl;
				return (false);
			}

			try {
				_logger.log(LogType::INFO, "Starting.");
				_daemonize();
			} catch (const std::exception &e) {
				_logger.log(LogType::ERROR, "Error: " + std::string(e.what()));
				return (false);
			}
			return (true);
		}

		void	_stop()
		{
			_logger.log(LogType::INFO, "Stopping...");
			_unlock();
		}

		void	_lock();
		void	_unlock();

		void	_daemonize();
		void	_setupSignals();
		void	_closeFDs();
};

#endif
