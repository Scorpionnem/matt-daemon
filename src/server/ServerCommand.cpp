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

std::string	Server::checkPart(Client &client, std::deque<std::string> data)
{
	if (data.size() == 1)
		return (client.send_msg(ERR_NEEDMOREPARAMS(client.getName(), data[0])));
	else if (data.size() > 3)
		return (client.send_msg(ERR_TOOMANYPARAMS(client.getName(), data[0])));

	std::deque<std::string>	channel = parsingMultiArgs(data[1]);
	for (std::deque<std::string>::iterator it = channel.begin(); it != channel.end(); it++)
	{
		std::string msg = "";
		if (data.size() == 2)
			msg = ":Leave";
		else if (data[2][0] != ':')
			msg = ":" + data[2];
		else
			msg += data[2];
		_channel->sendAllClient(client, LEAVE(client.getName(), client.getIp(), msg));
		_channel->deleteClient(client, _channel->getAllClient());
	}
	return ("");
}

std::string	Server::checkUser(Client& client, std::deque<std::string> data)
{
	if (data.size() < 5)
		return (client.send_msg(ERR_NEEDMOREPARAMS(client.getName(), data[0])));
	else if (data.size() > 5)
		return (client.send_msg(ERR_TOOMANYPARAMS(client.getName(), data[0])));

	for (std::deque<std::string>::iterator it = data.begin(); it != data.end(); it++)
	{
		if ((*it).find('@') != std::string::npos || (*it).find('#') != std::string::npos)
			return (client.send_msg(ERR_TOOMANYPARAMS(client.getName(), data[0])));
	}

	if (client.getName().size() != 0)
		client.setLogStatus(true);
	// if (client.getName().size() != 0)
	// {
		// client.send_msg(SELECTUSER(client.getUsername()));
	return (client.send_msg(AUTHENTIFICATED(client.getName())));
	// }
	// return (client.send_msg(SELECTUSER(client.getUsername())));
}

/*std::string	Server::checkNick(Client &client, std::deque<std::string> list_arg)
{

	if (list_arg.size() == 1 || list_arg[1] == "")
		return (client.send_msg(ERR_NONICKNAMEGIVEN));

	if (this->findClientByNick(list_arg[1]) != NULL)
		return (client.send_msg(ERR_NICKNAMEINUSE(list_arg[1])));

	if (list_arg[1].find('@') != std::string::npos || list_arg[1].find('#') != std::string::npos)
		return (client.send_msg(ERR_TOOMANYPARAMS(client.getName(), list_arg[1])));

	if (client.getName().size() == 0)
	{
		client.setName(list_arg[1]);
		// if (client.getUsername().size() != 0)
		// 	return (client.send_msg(AUTHENTIFICATED(client.getName())));
		return (client.send_msg(SELECTNICKNAME(client.getName())));	
	}
	this->sendToAllClient(client, list_arg[1]);
	client.setName(list_arg[1]);
	return ("");
}*/

void	Server::login(Client &client, std::deque<std::string> args)
{
	//add function to write in log the cmd
	_logger->log(LogType::CMD, std::to_string(client.getId()) + " used /login");
	if (args.size() != 3)
	{
		_logger->log(LogType::ERROR, std::to_string(client.getId()) + " failed /login");
		sendToClient(client, "/login : Invalid arguments");
		return ;
	}

	if (client.getLogin() == true)
	{
		_logger->log(LogType::ERROR, std::to_string(client.getId()) + " used /login : Already connected");
		sendToClient(client, "/login : You're already connected");
		return ; // add logger + msg already connected
	}

	if (this->findClientByNick(args[1]) != NULL)
	{
		_logger->log(LogType::ERROR, std::to_string(client.getId()) + " used /login : Can't used this name");
		sendToClient(client, "/login : Choosed another name");
		return ; //error
	}

	else if (_db.userExists(args[1]) == false)
		_db.addUser(args[1], args[2]);

	else if (_db.passMatch(args[1], args[2]) == false)
	{
		_logger->log(LogType::ERROR, std::to_string(client.getId()) + " used /login : Wrong password");
		sendToClient(client, "/login : Wrong password");
		return ;
	}

	client.setLogStatus(true);
	client.setName(args[1]);
	_logger->log(LogType::INFO, std::to_string(client.getId()) + " used /login : Connexion successfull");
	sendToClient(client, "/login : Connexion successfull");
}

void		Server::quit(Client &client, std::deque<std::string>)
{
	_stop = true;
	(void) client;
}

void		Server::leave(Client &client, std::deque<std::string>)
{
	(void) client;
}

void		Server::shell(Client &client, std::deque<std::string>)
{
	(void) client;
}

void		Server::list(Client &client, std::deque<std::string>)
{
	(void) client;
}

void		Server::help(Client &client, std::deque<std::string>)
{
	(void) client;
}

void		Server::msg(Client &client, std::deque<std::string> args)
{
	(void) client;
	(void) args;
}

void		Server::privMsg(Client &client, std::deque<std::string> args)
{
	(void) client;
	(void) args;
}


std::string	Server::checkPrivmsg(Client &client, std::deque<std::string> data)
{
	std::string	msg_to_send;

	std::deque<std::string>	receiver = data;

	receiver.pop_front();
	if (receiver.size() == 0)
		return (client.send_msg(ERR_NORECIPIENT(data[0])));
	else if (receiver.size() == 1)
		return (client.send_msg(ERR_NOTEXTTOSEND));
	else if (receiver.size() > 2)
		return (client.send_msg(ERR_TOOMANYPARAMS(client.getName(), data[0])));

	receiver = parsingMultiArgs(data[1]);
	msg_to_send = data[data.size() - 1];

	for (std::deque<std::string>::iterator it = receiver.begin(); it != receiver.end(); it++)
	{
		if ((*it)[0] != '#')
			this->sendToClient(*it, msg_to_send);
		else
			this->sendToChannel(client, *it, msg_to_send);
	}
	return ("");
}

/*void	Server::execMode(Client &client, std::deque<std::string> data, size_t &i, char token, char mode, Channel &channel)
{
	if (mode == 'i')
		channel.execModeI(client, token);
	else if (mode == 't')
		channel.execModeT(client, token);
	else if (mode == 'l')
		channel.execModeL(client, data, i, token);
	else if (data.size() <= i && token == '+')
		client.send_msg(ERR_NEEDMOREPARAMS(client.getName(), data[0]));
	else if (mode == 'k')
		channel.execModeK(client, data, i, token);
	else if (mode == 'o')
	{
		if (token == '-')
			channel.removeOp(client, data, i);
		else
			channel.addOp(client, data, i);
	}
	else
		client.send_msg(ERR_UNKNOWNMODE(client.getName(), mode));
}*/
