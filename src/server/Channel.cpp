/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 10:46:15 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/12 10:46:15 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Client*	Channel::findClientByName(std::string sender, std::deque<Client*> &list)
{
	for (std::deque<Client*>::iterator it = list.begin(); it != list.end(); it++)
	{
		if ((*it)->getName() == sender)
			return ((*it));
	}
	return (NULL);
}

void	Channel::deleteClient(Client &client, std::deque<Client*> &list)
{
	for (std::deque<Client*>::iterator it = list.begin(); it != list.end(); it++)
	{
		if ((*it)->getName() == client.getName())
		{
			list.erase(it);
			return ;
		}
	}
}

void	Channel::removeClient(Client &client)
{
	if (this->findClientByName(client.getName(), this->getAllClient()) != NULL)
		deleteClient(client, this->getAllClient());
}

void	Channel::addClient(Client &new_client)
{
	this->_list_client.push_back(&new_client);
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
