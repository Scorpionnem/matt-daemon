#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iomanip>
# include <unistd.h>
# include <iostream>
# include <sstream>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>

class Client
{
	private :
		std::string	_ip;
		std::string	_name;
		int			_isLogged;
		int			_socket_fd;
		std::string	_message_buffer;
		bool		_disconnected;
		size_t		_id;

	public :
		Client();
		Client(int socket_fd, std::string ip, size_t id);
		~Client();
		Client(const Client &obj);

		Client		&operator=(const Client &obj);

		void		setName(std::string name);
		void		setLogStatus(int status);
		void		setSocketFd(int socket_fd);
		void		setMessage(std::string message);
		void		setDisconnected(bool status);

		std::string	getIp(void);
		size_t		getId(void);
		std::string	getName(void);
		int			getLogin(void);
		int			getSocketFd(void);
		std::string	getMessage(void);
		bool		getDisconnected(void);
};

#endif