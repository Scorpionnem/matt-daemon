#ifndef SERVER_HPP
# define SERVER_HPP

# define NB_MAX_CLIENTS 3

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

# include "Tintin_reporter.hpp"

class Client;
class Channel;

enum class Command
{
	LOGIN,
	QUIT,
	LEAVE,
	SHELL,
	LIST,
	HELP,
	UNKNOW,
	MSG
};

class Server
{
	private :
		int						_server_socket;
		sockaddr_in 			_serverAddress;
		std::vector<Client*>	_client_list;
		Channel					*_channel;

		bool					_stop = false;

		struct 	pollfd fds[NB_MAX_CLIENTS + 1];
		
		void		stopServer();

		Command		CommandLexer(const std::string input);
		void		ExecCommand(Command cmd, std::deque<std::string> args);

		void		login(std::deque<std::string> args);


		void		initialize_poll_fds(struct pollfd fds[NB_MAX_CLIENTS + 1]);
		bool		add_client(Tintin_reporter &logger);
		void		read_all_clients(Tintin_reporter &logger, struct pollfd fds[NB_MAX_CLIENTS + 1], bool new_client);
		bool		process_commands(Tintin_reporter &logger, Client &client);

		std::string	checkUser(Client &client, std::deque<std::string> data);
		std::string	checkNick(Client &client, std::deque<std::string> list_arg);
		std::string	checkPrivmsg(Client &client, std::deque<std::string> data);
		std::string	checkPart(Client &client, std::deque<std::string> data);

		std::string sendToClient(Client &sender, std::string receiver, std::string msgToSend);
		std::string sendToChannel(Client &sender, std::string channel, std::string msgToSend);
		void		sendToAllClient(Client &client, std::string new_nickname);

	public :
		void		commands_parsing(Client &client, std::string commande);

		Client*		findClientByNick(std::string recipient);
		void		joinChannel(Client &client, Channel &channel) const;

		void		sendToAll(Client &client);

		Server();
		~Server();

		void		setup(Tintin_reporter &logger);
		void		stop(Tintin_reporter &logger);
		void		runtime(Tintin_reporter &logger);

		int						getServerSocket();
		std::vector<Client*>&	getListClient(void);

};

std::deque<std::string>	splitCommand(std::string input);
std::deque<std::string>	parsingMultiArgs(std::string data);

#endif
