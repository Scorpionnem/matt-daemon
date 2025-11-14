/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 10:46:21 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/13 08:26:31 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Define.hpp"

void	Server::LogMsgClient(Client &client, std::string msg_cl, LogType type, std::string log_msg)
{
	sendToClient(client, msg_cl);
	_logger->log(type, log_msg);
}

void	Server::login(Client &client, std::deque<std::string> args)
{
	_logger->log(LogType::CMD, LOGIN_CMD(client.getId()));

	if (args.size() != 3)
		return (LogMsgClient(client, LOGIN_CL_INV_ARG, LogType::ERROR, LOGIN_LOG_FAILED(client.getId())));
		
	if (client.getLogin() == true || this->findClientByName(args[1]) != NULL)
		return (LogMsgClient(client, LOGIN_CL_ALRD_CO, LogType::ERROR, LOGIN_LOG_ALRD_CO(client.getId())));
	
	else if (_db.userExists(args[1]) == false)
		_db.addUser(args[1], args[2]);

	else if (_db.passMatch(args[1], args[2]) == false)
		return (LogMsgClient(client, LOGIN_CL_ERR_DB, LogType::ERROR, LOGIN_LOG_ERR_DB(client.getId())));


	client.setLogStatus(true);
	client.setName(args[1]);

	LogMsgClient(client, LOGIN_CL_SUCCESS, LogType::INFO, LOGIN_LOG_SUCCESS(client.getId()));
}

void	Server::sendAll(std::string msg_cl)
{
	for (std::vector<Client *>::iterator it = this->getListClient().begin(); it != this->getListClient().end(); it++)
	{
		send((*it)->getSocketFd(), msg_cl.c_str(), msg_cl.size(), 0);
	}
}

void		Server::quit(Client &client, std::deque<std::string>)
{
	this->sendAll("Server closing...\n\r");
	_logger->log(LogType::INFO, std::to_string(client.getId()) + " /quit : closing server");
	_stop = true;
}

void		Server::leave(Client &client, std::deque<std::string>)
{
	this->_channel->removeClient(client);
	client.setDisconnected(true);
}

void		Server::list(Client &client, std::deque<std::string> args)
{
	_logger->log(LogType::CMD, LIST_CMD(client.getId()));

	send(client.getSocketFd(), "Client(s) connected : \n\r", 25, 0);
	for (std::vector<Client *>::iterator it = getListClient().begin(); it != getListClient().end(); it++)
	{
		std::string cli = LIST_CL((*it)->getName());
		send(client.getSocketFd(), cli.c_str(), cli.size(), 0);
	}

	if (args.size() > 1)
	{
		if (args[1] == "-a")
		{
			send(client.getSocketFd(), "Client(s) in Database : \n\r", 27, 0);
			for (std::map<std::string, std::string>::iterator it = _db.getDB().begin(); it != _db.getDB().end(); it++)
			{
				std::string cli = LIST_CL(it->first);
				if (findClientByName(it->first) == NULL)
					send(client.getSocketFd(), cli.c_str(), cli.size(), 0);
			}
		}
		else
			LogMsgClient(client, LIST_CL_BAD_FLAG, LogType::ERROR, LIST_LOG_BAD_FLAG(client.getId()));
	}
}

void		Server::help(Client &client, std::deque<std::string> args)
{
	_logger->log(LogType::CMD, HELP_CMD(client.getId()));

	if (args.size() > 1)
		return (LogMsgClient(client, HELP_CL_BAD_FLAG, LogType::ERROR, HELP_LOG_BAD_FLAG(client.getId())));
	
	std::string msg = HELP_CL_CMD;
	send(client.getSocketFd(), msg.c_str(), msg.size(), 0);
}

void		Server::msg(Client &client, std::deque<std::string> args)
{
	std::string all_args;
	
	for (std::deque<std::string>::iterator it = args.begin(); it != args.end(); it++)
	{
		all_args += *it;
		if (it != args.end())
			all_args += " ";
	}
	sendAll(client, all_args);
}

void		Server::privMsg(Client &client, std::deque<std::string> args)
{
	_logger->log(LogType::CMD, PRIVMSG_CMD(client.getId()));

	if (args.size() != 3)
		return (LogMsgClient(client, PRIVMSG_CL_ERR_ARG, LogType::ERROR, PRIVMSG_LOG_ERR_ARG(client.getId())));

	Client *target = findClientByName(args[1]);
	if (target == NULL)
		return (LogMsgClient(client, PRIVMSG_CL_ERR_USER_NOT_FOUND, LogType::ERROR, PRIVMSG_LOG_ERR_USER_NOT_FOUND(client.getId())));


	std::string msg = MSG_CL(client.getName(), args[2]);
	send(target->getSocketFd(), msg.c_str(), msg.size(), 0);
}
