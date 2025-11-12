/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 10:46:17 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/12 10:46:17 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

std::string	Client::send_msg(std::string msg_error)
{
	send(this->getSocketFd(), msg_error.c_str(), msg_error.size(), 0);
	return (msg_error);
}

Client::Client()
{
	this->_ip = "";
	this->setUsername("");
	this->setRealName("");
	this->setNickname("");
	this->setStatus(0);
	this->setSocketFd(-1);
	this->setDisconnected(false);
}

Client::Client(int socket_fd, std::string ip)
{
	this->_ip = ip;
	this->setUsername("");
	this->setRealName("");
	this->setNickname("");
	this->setStatus(0);
	this->setSocketFd(socket_fd);
	this->setDisconnected(false);
}

Client::~Client()
{}

Client::Client(const Client &obj)
{
	*this = obj;
}

Client	&Client::operator=(const Client &obj)
{
	_ip = obj._ip;
	_username = obj._username;
	_real_name = obj._real_name;
	_nickname = obj._nickname;
	_isLogged = obj._isLogged;
	_socket_fd = obj._socket_fd;
	_message_buffer = obj._message_buffer;
	_disconnected = obj._disconnected;
	return *this;
}

std::string	Client::getIp(void)
{
	return (this->_ip);
}


std::string	Client::getUsername(void)
{
	return (this->_username);
}

std::string	Client::getRealName(void)
{
	return (this->_real_name);
}

std::string	Client::getNickname(void)
{
	return (this->_nickname);
}

int	Client::getLogin()
{
	return (this->_isLogged);
}

int	Client::getSocketFd()
{
	return (this->_socket_fd);
}

std::string	Client::getMessage()
{
	return (this->_message_buffer);
}

bool	Client::getDisconnected()
{
	return (this->_disconnected);
}

void	Client::setUsername(std::string username)
{
	this->_username = username;
}

void	Client::setRealName(std::string real_name)
{
	this->_real_name = real_name;
}

void	Client::setNickname(std::string nickname)
{
	this->_nickname = nickname;
}

void	Client::setStatus(int status)
{
	this->_isLogged = status;
}

void	Client::setSocketFd(int socket_fd)
{
	this->_socket_fd = socket_fd;
}

void	Client::setMessage(std::string message)
{
	this->_message_buffer = message;
}

void	Client::setDisconnected(bool status)
{
	if (status == true)
		close(this->getSocketFd());
	this->_disconnected = status;
}

