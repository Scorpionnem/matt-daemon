/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MattDaemon.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 14:36:38 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/22 14:43:20 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATTDAEMON_HPP
# define MATTDAEMON_HPP

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

		void	start();
		void	stop();
		bool	receivedSignal();
	private:
		void	_setupSignals();
		void	_closeFDs();
		void	_daemonize();
		void	_lockFile();
		void	_unlockFile();
		
		int	_lockFD;
};

#endif
