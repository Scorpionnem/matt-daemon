/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 10:46:19 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/12 16:10:12 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

std::deque<std::string>	parsingMultiArgs(std::string data)
{
	std::deque<std::string> tab;

	size_t found = data.find(',');
	std::string	receiver;
	while (found != std::string::npos)
	{
		receiver = data.substr(0, found);
		data = data.substr(found + 1);
		found = data.find(',');
		if (receiver.size() != 0)
			tab.push_back(receiver);
	}
	if (data.size() != 0)
		tab.push_back(data);
	return (tab);
}

void	Server::stopServer()
{
	this->_stop = true;
}

void	Server::joinChannel(Client &client, Channel &channel) const
{
	channel.addClient(client);
}

void	Server::sendToAllClient(Client &client, std::string new_nickname)
{
	std::string	toSend = CHANGENICKNAMEFORALL(client.getNickname(), client.getUsername(), client.getIp(), new_nickname);
	for (std::vector<Client*>::iterator it = this->_client_list.begin(); it != this->_client_list.end(); it++)
	{
		send((*it)->getSocketFd(), toSend.c_str(), toSend.size(), 0);
	}
}

std::string Server::sendToChannel(Client &sender, std::string channel, std::string msgToSend)
{
	std::string	msg = MSGSEND(sender.getNickname(), sender.getUsername(), sender.getIp(), channel, msgToSend);

	this->_channel->sendAllClientMsg(sender, msg);
	return ("");
}

std::string Server::sendToClient(Client &sender, std::string receiver, std::string msgToSend)
{
	std::string	msg = MSGSEND(sender.getNickname(), sender.getUsername(), sender.getIp(), receiver, msgToSend);
	
	if (this->findClientByNick(receiver) == NULL)
		return (sender.send_msg(ERR_NOSUCHNICK(sender.getNickname(), receiver)));
	send(this->findClientByNick(receiver)->getSocketFd(), msg.c_str(), msg.size(), 0);
	return ("");
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

void	Server::runtime(Tintin_reporter &logger)
{
	bool	new_client = false;
	initialize_poll_fds(fds);

	int	nb_fd = poll(fds, _client_list.size() + 1, -1);

	if (nb_fd == -1 && errno == EINTR)
		return;

	if ((fds[0].revents & POLLIN) != 0)
	{
		new_client = add_client(logger);
	}

	if (new_client == true || this->_client_list.size() != 0)
		read_all_clients(logger, fds, new_client);
}

void	Server::initialize_poll_fds(struct pollfd fds[NB_MAX_CLIENTS + 1])
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

Client*	Server::findClientByNick(std::string recipient)
{
	for (std::vector<Client*>::iterator it = this->_client_list.begin(); it != this->_client_list.end(); it++)
	{
		if ((*it)->getNickname() == recipient)
			return ((*it));
	}
	return (NULL);
}

bool	Server::add_client(Tintin_reporter &logger)
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
			return (false);
		}
		else
		{
			char	ip[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, &addr, ip, INET_ADDRSTRLEN);
			Client	*new_client = new Client(clientSocket, ip);
			logger.log(LogType::LOG, "New client joined with fd: " + std::to_string(clientSocket));
			this->_client_list.push_back(new_client);
            this->_channel->addClient(*new_client);
			return (true);
		}
	}
	return (false);
}

void	Server::sendToAll(Client &client)
{
	for (std::vector<Client*>::iterator it = this->getListClient().begin(); it != this->getListClient().end(); it++)
	{
		std::string	msgLeave = USERDISCONNECTED(client.getNickname(), client.getUsername(), client.getIp(), (*it)->getNickname());
		if ((*it)->getNickname() != client.getNickname())
			send((*it)->getSocketFd(), msgLeave.c_str(), msgLeave.size(), 0);
	}
}

void	Server::read_all_clients(Tintin_reporter &logger, struct pollfd fds[NB_MAX_CLIENTS + 1], bool new_client)
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

			this->process_commands(logger, **it);
		}
		i++;

		if ((*it)->getDisconnected() == false)
			it++;
		else
		{
			logger.log(LogType::LOG, "User disconnected");
			this->sendToAll(**it);
			delete (*it);
			it = this->_client_list.erase(it);
		}
	}
}

bool	Server::process_commands(Tintin_reporter &logger, Client &client)
{
	std::string message;
	
	message = client.getMessage();
	if (!client.getDisconnected())
		logger.log(LogType::LOG, message);
	while (message.find("\n") != std::string::npos)
	{
		std::string command = message.substr(0, message.find("\n"));
		message = message.substr(message.find("\n") + 1);
		client.setMessage(message);
		commands_parsing(client, command);
	}
	return (true);
}

Command	Server::CommandLexer(const std::string input)
{
	static const std::array<std::pair<const char*, Command>, 15> table = {{
	{"/login", Command::LOGIN},
	{"/quit", Command::QUIT},
	{"quit", Command::QUIT},
	{"/shell", Command::SHELL},
	{"/leave", Command::LEAVE},
	{"/list", Command::LIST},
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

void	Server::ExecCommand(Command cmd, std::deque<std::string> args)
{
	switch (cmd)
	{
		case Command::LOGIN:
			return (login(args));
		case Command::QUIT:
			break;
		case Command::LEAVE:
			break;
		case Command::SHELL:
			break;
		case Command::LIST:
			break;
		case Command::HELP:
			break;
		case Command::UNKNOW:
			break;		
		default:
			break;
	}
}

void	Server::commands_parsing(Client &client, std::string input)
{
    (void) client;
	std::deque<std::string>		list_arg;

	if (input.size() == 0)
		return ;
	list_arg = splitCommand(input);
    for (std::deque<std::string>::iterator it = list_arg.begin(); it != list_arg.end(); it++)
    {
        std::cout << "args = " << *it << std::endl;
    }
	/*Command cmd = CommandLexer(list_arg[0]);
	if (client.getStatus() == 0 && cmd != Command::LOGIN)
		return ; // err not be connected 
	if (cmd == Command::QUIT && list_arg.size() != 1 && list_arg[0][0] != '/')
		cmd = Command::MSG;
	ExecCommand(cmd, list_arg);*/
	/*if (list_arg[0] == "USER")
		checkUser(client, list_arg);
	else if (list_arg[0] == "NICK")
		checkNick(client, list_arg);
	else if ((client.getNickname().size() == 0 || client.getUsername().size() == 0) && client.getStatus() == 0)
	{
		std::string	msg = NOTAUTHENTIFICATED;
		send(client.getSocketFd(), msg.c_str(), msg.size(), 0);
		return ;
	}
    else if (client.getNickname().size() != 0 && client.getUsername().size() != 0 && client.getStatus() == 0)
    {
        client.setStatus(1);
        this->joinChannel(client, *_channel);
    }
    if (client.getStatus() == 0)
    {
        return ;
    }

	if (list_arg[0] == "PRIVMSG")
		checkPrivmsg(client, list_arg);*/
	/*else if (list_arg[0] == "JOIN")
		checkJoin(client, list_arg);
	else if (list_arg[0] == "MODE")
		checkMode(client, list_arg);
	else if (list_arg[0] == "KICK")
		checkKick(client, list_arg);
	else if (list_arg[0] == "TOPIC")
		checkTopic(client, list_arg);
	else if (list_arg[0] == "INVITE")
		checkInvite(client, list_arg);
	else if (list_arg[0] == "PART")
		checkPart(client, list_arg);
	else if (list_arg[0] != "WHO" && client.getNickname().size() != 0 && client.getUsername().size() != 0)
	{
		std::string	msg = ERR_UNKNOWNCOMMAND(client.getNickname(), list_arg[0]);
		send(client.getSocketFd(), msg.c_str(), msg.size(), 0);
	}*/
}

/*Server::Server(int port, std::string password)
{
	this->_password = password;
	this->_server_socket = socket(AF_INET, SOCK_STREAM, 0);
	this->_serverAddress.sin_family = AF_INET;
	this->_serverAddress.sin_port = htons(port);
	this->_serverAddress.sin_addr.s_addr = INADDR_ANY;

	bind(this->_server_socket, (struct sockaddr*)&this->_serverAddress,
		sizeof(this->_serverAddress));
	listen(this->getServerSocket(), 5);
	runtime();
}*/

Server::Server()
{
}

void	Server::setup(Tintin_reporter &logger)
{
	logger.log(LogType::INFO, "Starting server");
	this->_server_socket = socket(AF_INET, SOCK_STREAM, 0);
	this->_serverAddress.sin_family = AF_INET;
	this->_serverAddress.sin_port = htons(7002);
	this->_serverAddress.sin_addr.s_addr = INADDR_ANY;

	bind(this->_server_socket, (struct sockaddr*)&this->_serverAddress,
		sizeof(this->_serverAddress));
	listen(this->getServerSocket(), 3);
	logger.log(LogType::INFO, "Server started");
	_channel = new Channel;
}

void	Server::stop(Tintin_reporter &logger)
{
	for (std::vector<Client*>::iterator it = this->_client_list.begin(); it != this->_client_list.end(); it++)
	{
		close((*it)->getSocketFd());
		delete (*it);
	}
	delete (_channel);
	close(_server_socket);
	logger.log(LogType::INFO, "Server closed.");
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
