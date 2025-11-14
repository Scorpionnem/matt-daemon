#ifndef SERVER_HPP
# define SERVER_HPP

# define NB_MAX_CLIENTS 3

#include "prod.hpp"

# if PROD == 1
#  define SERVER_PORT 4242
# else
#  define SERVER_PORT 7002
# endif

# include "Client.hpp"
# include <arpa/inet.h>
# include <cstring>
# include <iostream>
# include <vector>
# include <deque>
# include <netinet/in.h>
# include <sys/socket.h>
# include <unistd.h>
# include <poll.h>
# include <algorithm>
# include <csignal>
# include <cstdlib>
# include <errno.h>
# include <iomanip>
# include <string>
# include <ctime>
# include <array>
# include "Define.hpp"
# include "Channel.hpp"
# include "UsersDatabase.hpp"

# include "Tintin_reporter.hpp"

class Client;
class Channel;

enum class Command
{
	LOGIN,
	QUIT,
	LEAVE,
	LIST,
	HELP,
	PRIVMSG,
	UNKNOW,
	MSG
};

class Server
{
	private :
		int						_server_socket = -1;
		int						_pollFD = -1;
		sockaddr_in 			_serverAddress;
		std::vector<Client*>	_client_list;
		Channel					*_channel = NULL;
		Tintin_reporter			*_logger = NULL;
		size_t					_id = 0;
		UsersDatabase			_db;


		bool					_stop = false;

		struct 	pollfd fds[NB_MAX_CLIENTS + 1];
		
		void		stopServer();

		Command		commandLexer(const std::string input);
		void		execCommand(Command cmd, std::deque<std::string> args, Client &client);

		void		login(Client &client, std::deque<std::string> args);
		void		quit(Client &client, std::deque<std::string> args);
		void		leave(Client &client, std::deque<std::string> args);
		void		list(Client &client, std::deque<std::string> args);
		void		help(Client &client, std::deque<std::string> args);
		void		msg(Client &client, std::deque<std::string> args);
		void		privMsg(Client &client, std::deque<std::string> args);


		void		initializePollFds(struct pollfd fds[NB_MAX_CLIENTS + 1]);
		bool		addClient();
		void		readAllClients(struct pollfd fds[NB_MAX_CLIENTS + 1], bool new_client);
		bool		processCommands(Client &client);


		void 		sendToClient(Client &client, std::string msgToSend);

		void		LogMsgClient(Client &client, std::string msg_cl, LogType type, std::string log_msg);

		void		sendAll(std::string msg_cl);
		void		sendAll(Client &client, std::string msg_cl);

	public :
		bool		running() {return (!_stop);}
		void		commandsParsing(Client &client, std::string commande);

		Client*		findClientByName(std::string recipient);

		Server();
		Server(const Server &cpy)
		{
			*this = cpy;
		}
		Server	&operator=(const Server &copy)
		{
			if (this != &copy)
			{

			}
			return (*this);
		}
		~Server();


		void		setup(Tintin_reporter &logger);
		void		stop();
		void		runtime();

		int						getServerSocket();
		std::vector<Client*>&	getListClient(void);

};

std::deque<std::string>	splitCommand(std::string input);

#endif
