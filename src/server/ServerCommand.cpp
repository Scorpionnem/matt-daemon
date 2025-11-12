/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 10:46:21 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/12 10:46:51 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Define.hpp"

std::string	Server::checkPart(Client &client, std::deque<std::string> data)
{
	if (data.size() == 1)
		return (client.send_msg(ERR_NEEDMOREPARAMS(client.getNickname(), data[0])));
	else if (data.size() > 3)
		return (client.send_msg(ERR_TOOMANYPARAMS(client.getNickname(), data[0])));

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
		_channel->sendAllClient(client, LEAVE(client.getNickname(), client.getUsername(), client.getIp(), msg));
		_channel->deleteClient(client, _channel->getAllClient());
	}
	return ("");
}

std::string	Server::checkUser(Client& client, std::deque<std::string> data)
{
	if (client.getRealName() != "")
		return (client.send_msg(ERR_ALREADYREGISTRED));

	if (data.size() < 5)
		return (client.send_msg(ERR_NEEDMOREPARAMS(client.getNickname(), data[0])));
	else if (data.size() > 5)
		return (client.send_msg(ERR_TOOMANYPARAMS(client.getNickname(), data[0])));

	if (client.getUsername().size() != 0)
			return (client.send_msg(ERR_ALREADYREGISTRED));

	for (std::deque<std::string>::iterator it = data.begin(); it != data.end(); it++)
	{
		if ((*it).find('@') != std::string::npos || (*it).find('#') != std::string::npos)
			return (client.send_msg(ERR_TOOMANYPARAMS(client.getNickname(), data[0])));
	}

	client.setUsername(data[1]);
	client.setRealName(data[4]);
	if (client.getNickname().size() != 0)
		client.setStatus(true);
	if (client.getUsername().size() != 0 && client.getNickname().size() != 0)
	{
		client.send_msg(SELECTUSER(client.getUsername()));
		return (client.send_msg(AUTHENTIFICATED(client.getNickname())));
	}
	return (client.send_msg(SELECTUSER(client.getUsername())));
}

std::string	Server::checkNick(Client &client, std::deque<std::string> list_arg)
{

	if (list_arg.size() == 1 || list_arg[1] == "")
		return (client.send_msg(ERR_NONICKNAMEGIVEN));

	if (this->findClientByNick(list_arg[1]) != NULL)
		return (client.send_msg(ERR_NICKNAMEINUSE(list_arg[1])));

	if (list_arg[1].find('@') != std::string::npos || list_arg[1].find('#') != std::string::npos)
		return (client.send_msg(ERR_TOOMANYPARAMS(client.getNickname(), list_arg[1])));

	if (client.getNickname().size() == 0)
	{
		client.setNickname(list_arg[1]);
		if (client.getUsername().size() != 0)
			return (client.send_msg(AUTHENTIFICATED(client.getNickname())));
		return (client.send_msg(SELECTNICKNAME(client.getNickname())));	
	}
	this->sendToAllClient(client, list_arg[1]);
	client.setNickname(list_arg[1]);
	return ("");
}

void	Server::login(std::deque<std::string> args)
{
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
		return (client.send_msg(ERR_TOOMANYPARAMS(client.getNickname(), data[0])));

	receiver = parsingMultiArgs(data[1]);
	msg_to_send = data[data.size() - 1];

	for (std::deque<std::string>::iterator it = receiver.begin(); it != receiver.end(); it++)
	{
		if ((*it)[0] != '#')
			this->sendToClient(client, *it, msg_to_send);
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
		client.send_msg(ERR_NEEDMOREPARAMS(client.getNickname(), data[0]));
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
		client.send_msg(ERR_UNKNOWNMODE(client.getNickname(), mode));
}*/
