/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ben_AFK.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 14:31:06 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/12 15:32:38 by mbatty           ###   ########.fr       */
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
#include <sys/ioctl.h>

class	Ben_AFK
{
	public:
		Ben_AFK()
		{
			_sockFD = -1;
			_running = false;
		}		
		~Ben_AFK()
		{
			if (_sockFD != -1)
				close(_sockFD);
		}

		Ben_AFK(const Ben_AFK &copy)
		{
			*this = copy;
		}
		Ben_AFK	&operator=(const Ben_AFK &copy)
		{
			if (this != &copy)
			{
				this->_readfds = copy._readfds;
				this->_running = copy._running;
				this->_sockFD = copy._sockFD;
			}
			return (*this);
		}

		void	init(const char *ip, int port);
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
