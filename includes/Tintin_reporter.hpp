/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tintin_reporter.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 18:07:01 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/14 14:14:44 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TINTIN_REPORTER_HPP
# define TINTIN_REPORTER_HPP

#include "prod.hpp"

# include <fstream>
# include <iostream>
# include <ctime>
# include <iomanip>
# include <exception>
# include <filesystem>
# include <sys/stat.h>

// Path to log directory
# if PROD == 1
#  define VAR_LOG_PATH			"/var/log/"
# else
#  define VAR_LOG_PATH			""
# endif

// Path to matt_daemon log directory inside of logs
# define MATT_DAEMON_LOG_PATH	"matt_daemon/"
// Name of the log file without the timestamp
# define LOG_PATH				"matt_daemon_"
# define LOG_PATH_NOTS				"matt_daemon"
// Extension format of the log file
# define LOG_EXTENSION			".log"

/*
	Type given to Tintin_reporter, used to add the "header" in the logs
*/
enum class LogType
{
	// Used when logging user messages
	LOG,
	// Used when logging infos about the program
	INFO,
	// Used when logging errors
	ERROR,
	CMD,
	NONE,
};

/*
	Logger for the program
	(Goofy name because of the subject)
*/
class	Tintin_reporter
{
	public:
		Tintin_reporter();
		~Tintin_reporter();
		
		Tintin_reporter(const Tintin_reporter &copy)
		{
			*this = copy;
		}
		Tintin_reporter	&operator=(const Tintin_reporter &copy)
		{
			if (this != &copy)
			{

			}
			return (*this);
		}

		//	Initializes the logger, opens the file and creates the log directory
		void	init();
		//	Close log file
		void	close();
		//	Calls log with LogType::NONE
		void	log(const std::string &str);
		//	Logs a message with LogType "header" ([ LOG ], [ INFO ] ...)
		void	log(LogType type, const std::string &str);
	private:
		void	_log(const std::string &str);

		//	Returns the timestamp in this format [D/M/Y-H:m:s]
		std::string	_getLogTimeString();
		//	Returns the timestamp in this format D_M_Y_H_m_s
		std::string	_getLogFileTimeString();
		
		std::ofstream	_timestampFile;
		std::ofstream	_file;
};

#endif
