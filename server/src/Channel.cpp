#include "../include/Channel.hpp"

Client*	Channel::findClientByNick(std::string sender, std::deque<Client*> &list)
{
	for (std::deque<Client*>::iterator it = list.begin(); it != list.end(); it++)
	{
		if ((*it)->getNickname() == sender)
			return ((*it));
	}
	return (NULL);
}

std::string	Channel::sendAllClientMsg(Client &sender, std::string msg)
{
	for (std::deque<Client*>::iterator it = this->_list_client.begin(); it != this->_list_client.end(); it++)
	{
		if (sender.getNickname() != (*it)->getNickname())
			send((*it)->getSocketFd(), msg.c_str(), msg.size(), 0);
	}
	return ("");
}

std::string	Channel::sendAllClient(Client &sender, std::string msg)
{
	(void) sender;
	for (std::deque<Client*>::iterator it = this->_list_client.begin(); it != this->_list_client.end(); it++)
	{
		send((*it)->getSocketFd(), msg.c_str(), msg.size(), 0);
	}
	return ("");
}

void	Channel::deleteClient(Client &client, std::deque<Client*> &list)
{
	for (std::deque<Client*>::iterator it = list.begin(); it != list.end(); it++)
	{
		if ((*it)->getNickname() == client.getNickname())
		{
			list.erase(it);
			return ;
		}
	}
}

void	Channel::removeClient(Client &client)
{
	if (this->findClientByNick(client.getNickname(), this->getAllClient()) != NULL)
	{
		this->sendAllClient(client, QUIT(client.getNickname(), client.getUsername(), client.getIp()));
		deleteClient(client, this->getAllClient());
	}
}

void	Channel::addClient(Client &new_client)
{
	this->_list_client.push_back(&new_client);
	this->sendAllClient(new_client, MSGJOIN(new_client.getNickname(), new_client.getUsername(), new_client.getIp()));
}

//////////////////////////////////////////////////////
//				  (CONS/DES)TRUCTOR					//
//////////////////////////////////////////////////////

Channel::Channel()
{}

Channel::~Channel() {}

//////////////////////////////////////////////////////
//						GETTERS						//
//////////////////////////////////////////////////////

std::deque<Client*>&	Channel::getAllClient(void)
{
	return (this->_list_client);
}
