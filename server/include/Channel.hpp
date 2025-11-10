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
		~Channel();
		
		std::deque<Client*>&	getAllClient(void);

		void					addClient(Client &new_client);
		void					removeClient(Client &client);
		void					deleteClient(Client &client, std::deque<Client*> &list);

		std::string				sendAllClient(Client &sender, std::string msg);
		std::string				sendAllClientMsg(Client &sender, std::string msg);
		Client*					findClientByNick(std::string sender, std::deque<Client*> &list);

};

#endif