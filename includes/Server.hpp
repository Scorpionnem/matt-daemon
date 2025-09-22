/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 16:15:31 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/22 16:25:35 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <sys/epoll.h>
# include <arpa/inet.h>
# include <fcntl.h>
# include <map>
# include <cstdlib>
# include <cstring>
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

# include "Tintin_reporter.hpp"

# define MAX_EVENTS 10
# define SERVER_PORT 6942
# define MAX_CLIENTS 3
# define BUFFER_SIZE 1024

class	Server
{
	public:
		Server() {}
		~Server() {}
		void	start();
		void	stop();
		bool	receive();
	private:
		void	_sendMessage(int clientFd, const std::string &message);
		void	_disconnectClient(int fd);
		bool	_parseMessage(const std::string &message);
		bool	_readMessage(int clientFd);
		std::map<int, std::string>	_clients;
		int							_socketFd = 0;
		int							_pollFd = 0;
		struct epoll_event			_poll;
		struct epoll_event			_multPoll[MAX_EVENTS];
		struct sockaddr_in			_socket;
};

#endif
