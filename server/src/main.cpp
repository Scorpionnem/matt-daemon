# include <csignal>
# include "../include/Server.hpp"

void	handler(int num)
{
	(void) num;
}

int main()
{
	signal(SIGINT, handler);
	Server	serv;
	serv.setup();
	serv.runtime();
    close(serv.getServerSocket());
}
