/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ben_AFK.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 14:31:06 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/02 13:39:20 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BEN_AFK_HPP
# define BEN_AFK_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <exception>

class	Ben_AFK
{
	public:
		Ben_AFK()
		{
			_sockFD = -1;
			_running = false;
		}
		void	init(const char *ip, int port);

		~Ben_AFK()
		{
			if (_sockFD != -1)
				close(_sockFD);
		}

		bool	running() {return (_running);}
		static bool	sig;
	private:
		static void	_sigHandler(int s)
		{
			(void)s;
			Ben_AFK::sig = true;
		}

		void	_userInput(std::string input);
		void	_serverInput(std::string input);
		void	_loop();

		fd_set	_readfds;
		char 	_buffer[4096];
		bool	_running;
		int		_sockFD;
};

#endif
