/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 10:46:19 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/14 08:22:38 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::stopServer()
{
	this->_stop = true;
}

void Server::sendToClient(Client &client, std::string msgToSend)
{
	send(client.getSocketFd(), msgToSend.c_str(), msgToSend.size(), 0);
}


std::deque<std::string>	splitCommand(std::string input)
{
	std::deque<std::string>		tab;
	std::string					arg;
	bool						end_command = false;


	if (input[input.size() - 1] == '\r')
		input = input.substr(0, input.size() - 1);
	for (std::string::iterator it = input.begin(); it != input.end(); it++)
	{
		if (*it == ' ' && end_command == false)
		{
			if (arg.size() != 0)
			{
				tab.push_back(arg);
				arg = "";
			}
		}
		else if (*it == ':')
			end_command = true;

		if (*it != ' ' || end_command == true)
			arg += *it;
	}
	if (arg.size() != 0)
		tab.push_back(arg);
	return (tab);
}

void	Server::runtime()
{
	bool	new_client = false;
	initializePollFds(fds);

	_pollFD = poll(fds, _client_list.size() + 1, -1);

	if (_pollFD == -1 && errno == EINTR)
		return;

	if ((fds[0].revents & POLLIN) != 0)
	{
		new_client = addClient();
	}

	if (new_client == true || this->_client_list.size() != 0)
		readAllClients(fds, new_client);
}

void	Server::initializePollFds(struct pollfd fds[NB_MAX_CLIENTS + 1])
{
	fds[0].fd = this->getServerSocket();
	fds[0].events = POLLIN;
	fds[0].revents = 0;
	int	i = 1;
	for (std::vector<Client*>::iterator it = _client_list.begin(); it != _client_list.end(); it++)
	{
		fds[i].fd = (*it)->getSocketFd();
		fds[i].events = POLLIN;
		fds[i].revents = 0;
		i++;
	}
}

Client*	Server::findClientByName(std::string recipient)
{
	for (std::vector<Client*>::iterator it = this->_client_list.begin(); it != this->_client_list.end(); it++)
	{
		if ((*it)->getName() == recipient)
			return ((*it));
	}
	return (NULL);
}

bool	Server::addClient()
{
	sockaddr_in		addr;
	unsigned int	len = 0;

	std::memset(&addr, 0, sizeof(sockaddr_in));
	int clientSocket = accept(this->getServerSocket(), (struct sockaddr*)&addr, &len);
	if (clientSocket != -1)
	{
		if (this->_client_list.size() >= NB_MAX_CLIENTS)
		{
			close(clientSocket);
			_logger->log(LogType::ERROR, "User can't join : Too many clients");
			return (false);
		}
		else
		{
			char	ip[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, &addr, ip, INET_ADDRSTRLEN);
			Client	*new_client = new Client(clientSocket, ip, _id++);
			_logger->log(LogType::LOG, "New client joined with fd: " + std::to_string(clientSocket));
			this->_client_list.push_back(new_client);
            this->_channel->addClient(*new_client);
			return (true);
		}
	}
	return (false);
}

void	Server::sendAll(Client &client, std::string msg_cl)
{
	for (std::vector<Client*>::iterator it = this->getListClient().begin(); it != this->getListClient().end(); it++)
	{
		std::string	msg = MSG_CL(client.getName(), msg_cl);
		if ((*it)->getName() != client.getName())
			send((*it)->getSocketFd(), msg.c_str(), msg.size(), 0);
	}
}

void	Server::readAllClients(struct pollfd fds[NB_MAX_CLIENTS + 1], bool new_client)
{
	int		i = 1;
	ssize_t size;
	char	buffer[1024];

	for (std::vector<Client*>::iterator it = this->_client_list.begin(); it != this->_client_list.end() - new_client;)
	{
		std::string	message = (*it)->getMessage();
	
		if ((fds[i].revents & POLLIN) != 0)
		{
			do
			{
				size = recv(fds[i].fd, buffer, sizeof(buffer) - 1, 0);
				if (size == -1)
				{
					(*it)->setDisconnected(true);
					break;
				}
				if (size == 0)
					(*it)->setDisconnected(true);
				buffer[size] = 0;
				message = message + buffer;
				(*it)->setMessage(message);
			} while (size == 1024);

			this->processCommands(**it);
		}
		i++;

		if ((*it)->getDisconnected() == false)
			it++;
		else
		{
			_logger->log(LogType::LOG, "User : " + std::to_string((*it)->getId()) + " disconnected");
			this->sendAll(std::to_string((*it)->getId()) + " disconnected\n\r");
			delete (*it);
			it = this->_client_list.erase(it);
		}
	}
}

bool	Server::processCommands(Client &client)
{
	std::string message;
	
	message = client.getMessage();
	if (!client.getDisconnected())
		_logger->log(LogType::LOG, message);
	while (message.find("\n") != std::string::npos)
	{
		std::string command = message.substr(0, message.find("\n"));
		message = message.substr(message.find("\n") + 1);
		client.setMessage(message);
		commandsParsing(client, command);
	}
	return (true);
}

Command	Server::commandLexer(const std::string input)
{
	static const std::array<std::pair<const char*, Command>, 7> table = {{
	{"/login", Command::LOGIN},
	{"/quit", Command::QUIT},
	{"quit", Command::QUIT},
	{"/leave", Command::LEAVE},
	{"/list", Command::LIST},
	{"/msg", Command::PRIVMSG},
	{"/help", Command::HELP}}};

    for (auto &pair : table)
    {
        const std::string &name = pair.first;
        Command command = pair.second;
        if (input == name)
            return (command);
    }
	if (input[0] == '/')
    	return (Command::UNKNOW);
    return (Command::MSG);
}

void	Server::execCommand(Command cmd, std::deque<std::string> args, Client &client)
{
	Command cmds[7] = {Command::LOGIN, Command::QUIT,
		Command::LEAVE, Command::LIST, Command::HELP,
		Command::PRIVMSG, Command::MSG};
    void (Server::*functptr[])(Client &, std::deque<std::string>) = {
        &Server::login,
        &Server::quit,
        &Server::leave,
        &Server::list,
        &Server::help,
        &Server::privMsg,
        &Server::msg
    };
	if (cmd != Command::HELP && cmd != Command::QUIT && cmd != Command::UNKNOW && cmd != Command::LOGIN && client.getLogin() == 0)
		return (LogMsgClient(client, CL_NOT_LOGIN, LogType::ERROR, LOG_NOT_LOGIN(client.getId())));

	for (int j = 0; j <= 6; j++)
	{
		if (cmd == cmds[j])
		{
			(this->*functptr[j])(client, args);
			return ;
		}
	}
	LogMsgClient(client, CL_CMD_UNKNOW, LogType::ERROR, LOG_CMD_UNKNOW(client.getId()));
}

void	Server::commandsParsing(Client &client, std::string input)
{
	std::deque<std::string>		list_arg;

	if (input.size() == 0)
		return ;
	list_arg = splitCommand(input);
	Command cmd = commandLexer(list_arg[0]);
	if (cmd == Command::QUIT && list_arg.size() != 1 && list_arg[0][0] != '/')
		cmd = Command::MSG;
	execCommand(cmd, list_arg, client);
}

Server::Server()
{}

void	Server::setup(Tintin_reporter &logger)
{
	_logger = &logger;
	_logger->log(LogType::INFO, "Starting server");
	_db.loadDB();
	this->_server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_server_socket == -1)
		throw std::runtime_error(strerror(errno));
	this->_serverAddress.sin_family = AF_INET;
	this->_serverAddress.sin_port = htons(SERVER_PORT);
	this->_serverAddress.sin_addr.s_addr = INADDR_ANY;

	int yes = 1;
	if (setsockopt(_server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
	{
		close(_server_socket);
		throw std::runtime_error(strerror(errno));
	}

	if (bind(this->_server_socket, (struct sockaddr*)&this->_serverAddress, sizeof(this->_serverAddress)) == -1)
	{
		close(_server_socket);
		throw std::runtime_error(strerror(errno));
	}
	if (listen(this->getServerSocket(), 3) == -1)
	{
		close(_server_socket);
		throw std::runtime_error(strerror(errno));
	}
	_logger->log(LogType::INFO, "Server started");
	_channel = new Channel;
}

void	Server::stop()
{
	_db.exportDB();
	for (std::vector<Client*>::iterator it = this->_client_list.begin(); it != this->_client_list.end(); it++)
	{
		close((*it)->getSocketFd());
		delete (*it);
	}
	if (_channel)
		delete (_channel);
	if (_server_socket != -1)
		close(_server_socket);
	if (_pollFD != -1)
		close(_pollFD);
	_logger->log(LogType::INFO, "Server closed.");
}

Server::~Server()
{
}

std::vector<Client*>&	Server::getListClient(void)
{
	return (this->_client_list);
}

int	Server::getServerSocket()
{
	return (this->_server_socket);
}
