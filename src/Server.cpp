/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 16:20:25 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/22 16:25:56 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::start()
{
	_socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socketFd == -1)
		throw std::runtime_error("server error");
	_socket.sin_family = AF_INET;
	_socket.sin_addr.s_addr = INADDR_ANY;
	_socket.sin_port = htons(SERVER_PORT);
	int yes = 1;
	if (fcntl(_socketFd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("server error");
	if (setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		throw std::runtime_error("server error");
	if (bind(_socketFd, (struct sockaddr *)&_socket, sizeof(_socket)) < 0)
		throw std::runtime_error("server error");
	if (listen(_socketFd, MAX_CLIENTS) < 0)
		throw std::runtime_error("server error");
	_pollFd = epoll_create1(0);
	if (_pollFd < 0)
		throw std::runtime_error("server error");
	_poll.events = EPOLLIN;
	_poll.data.fd = _socketFd;
	if (epoll_ctl(_pollFd, EPOLL_CTL_ADD, _socketFd, &_poll) < 0)
		throw std::runtime_error("server error");
}

void	Server::stop()
{
	for (auto it = _clients.begin(); it != _clients.end(); ++it)
	{
		_sendMessage(it->first, "MattDaemon: Closing.\n");
		close(it->first);
	}
	if (_socketFd > 0)
		close(_socketFd);
	if (_pollFd > 0)
		close(_pollFd);
}

bool	Server::receive()
{
	socklen_t	socketSize;
	int nbFds = epoll_wait(_pollFd, _multPoll, MAX_EVENTS, 0);
	if (nbFds < 0)
		exit(1);
	for (int i = 0; i < nbFds; ++i)
	{
		if (_multPoll[i].data.fd == _socketFd)
		{
			socketSize = sizeof(_socket);
			int	clientFd = accept(_socketFd, (struct sockaddr *)&_socket, &socketSize);
			if (clientFd < 0)
				continue;
			fcntl(clientFd, F_SETFL, O_NONBLOCK);
			_poll.events = EPOLLIN | EPOLLET;
			_poll.data.fd = clientFd;
			_clients.insert({clientFd, ""});
			Tintin_reporter::log(LogType::INFO, "New client connected.");
			_sendMessage(clientFd, "MattDaemon: Successfully connected.\n");
			if (epoll_ctl(_pollFd, EPOLL_CTL_ADD, clientFd, &_poll) < 0)
			{
				close(clientFd);
				_clients.erase(clientFd);
			}
		}
		else
		{
			int	clientFd = _multPoll[i].data.fd;
			if (!_readMessage(clientFd))
			{
				_disconnectClient(clientFd);
				return (true);
			}
			else if (!_parseMessage(_clients[clientFd]))
				return (false);
			_clients[clientFd] = "";
			_sendMessage(clientFd, "MattDaemon: Successfully received message.\n");
		}
	}
	return (true);
}

bool	Server::_readMessage(int clientFd)
{
	int		nbRecv;
	char	buffer[BUFFER_SIZE] = {0};

	do
	{
		memset(buffer, 0, sizeof(buffer));
		nbRecv = recv(clientFd, buffer, sizeof(buffer), MSG_DONTWAIT);
		if (nbRecv == 0)
			return (false);
		_clients[clientFd].append(buffer, nbRecv);
	} while (nbRecv == BUFFER_SIZE);

	if (_clients[clientFd].empty())
		return (true);

	if (_clients[clientFd].back() == '\n')
		_clients[clientFd].erase(_clients[clientFd].size() - 1);
	return (true);
}

bool	Server::_parseMessage(const std::string &message)
{
	Tintin_reporter::log(LogType::LOG, message);
	if (message == "quit")
	{
		Tintin_reporter::log(LogType::INFO, "Received order to close.");
		return (false);
	}
	return (true);
}

void	Server::_sendMessage(int clientFd, const std::string &message)
{
	const char	*data = message.c_str();
	size_t		totalSent = 0;
	size_t		toSend = message.size();

	while (totalSent < toSend)
	{
		ssize_t sent = send(clientFd, data + totalSent, toSend - totalSent, 0);
		if (sent <= 0)
		{
			_disconnectClient(clientFd);
			return ;
		}
		totalSent += sent;
	}
}

void	Server::_disconnectClient(int fd)
{
	epoll_ctl(_pollFd, EPOLL_CTL_DEL, fd, NULL);
	close(fd);
	_clients.erase(fd);
	Tintin_reporter::log(LogType::INFO, "Client disconnected.");
}
