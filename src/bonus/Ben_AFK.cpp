/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ben_AFK.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 14:30:44 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/05 10:08:35 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Ben_AFK.hpp"

bool	Ben_AFK::sig = false;

void	Ben_AFK::init(const char *ip, int port)
{
	sockaddr_in server_addr{};

	_sockFD = socket(AF_INET, SOCK_STREAM, 0);
	if (_sockFD < 0)
		throw std::runtime_error(strerror(errno));
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	inet_pton(AF_INET, ip, &server_addr.sin_addr);

	if (connect(_sockFD, (sockaddr*)&server_addr, sizeof(server_addr)) < 0)
		throw std::runtime_error(strerror(errno));

	_running = true;
	signal(SIGINT, _sigHandler);
	_loop();
}

void	Ben_AFK::_loop()
{
	while (_running)
	{
		std::cout << ":> " << std::flush;

		FD_ZERO(&_readfds);
		FD_SET(STDIN_FILENO, &_readfds);
		FD_SET(_sockFD, &_readfds);

		int maxfd = std::max(STDIN_FILENO, _sockFD) + 1;
		int activity = select(maxfd, &_readfds, nullptr, nullptr, nullptr);

		if (activity < 0)
		{
			if (!Ben_AFK::sig)
				perror("select");
			break ;
		}

		if (FD_ISSET(STDIN_FILENO, &_readfds))
		{
			std::string	input;
			if (!std::getline(std::cin, input))
				break ;
			_userInput(input);
		}

		if (FD_ISSET(_sockFD, &_readfds))
		{
			ssize_t bytes = recv(_sockFD, _buffer, sizeof(_buffer) - 1, 0);
			if (bytes <= 0)
				break ;
			_buffer[bytes] = '\0';
			_serverInput(_buffer);
		}
	}
	std::cout << std::endl << "Connection closed." << std::endl;
}

void	Ben_AFK::_serverInput(std::string input)
{
	std::cout << "\a\r" << input;
}

void	Ben_AFK::_userInput(std::string input)
{
	input += "\n\r";
	send(_sockFD, input.c_str(), input.size(), 0);
}

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << "Usage: " << "Ben_AFK <IP> <PORT>\n";
		return (1);
	}

	Ben_AFK	client;

	try {
		client.init(av[1], std::stoi(av[2]));
	} catch (const std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
}
