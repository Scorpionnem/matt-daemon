#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <deque>
# include "Client.hpp"
# include "Server.hpp"
# include "Define.hpp"

class Channel
{
	private :
		std::deque<Client*>	_list_client;
	public :
		Channel();
		Channel(const Channel &cpy)
		{
			*this = cpy;
		}
		Channel	&operator=(const Channel &copy)
		{
			if (this != &copy)
			{

			}
			return (*this);
		}
		~Channel();
		
		std::deque<Client*>&	getAllClient(void);

		void					addClient(Client &new_client);
		void					removeClient(Client &client);
		void					deleteClient(Client &client, std::deque<Client*> &list);

		Client*					findClientByName(std::string sender, std::deque<Client*> &list);

};

#endif
