/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tintin_reporter.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 18:07:01 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/23 15:13:03 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TINTIN_REPORTER_HPP
# define TINTIN_REPORTER_HPP

# include <fstream>
# include <iostream>
# include <ctime>
# include <iomanip>
# include <exception>
# include <filesystem>
# include <sys/stat.h>

// Path to log directory
# define VAR_LOG_PATH			"/var/log/"
// Path to matt_daemon log directory inside of logs
# define MATT_DAEMON_LOG_PATH	"matt_daemon/"
// Name of the log file without the timestamp
# define LOG_PATH				"matt_daemon_"
// Extension format of the log file
# define LOG_EXTENSION			".log"

/*
	Type given to Tintin_reporter, used to add the "header" in the logs
*/
enum class LogType
{
	LOG,
	INFO,
	ERROR,
	NONE,
};

/*
	Logger for the program, saves and logs any log
*/
class	Tintin_reporter
{
	public:
		Tintin_reporter();
		~Tintin_reporter();
		
		void	init()
		{
			std::string logFilePath = MATT_DAEMON_LOG_PATH LOG_PATH + _getLogFileTimeString() + LOG_EXTENSION;
	
			if (!std::filesystem::exists(MATT_DAEMON_LOG_PATH))
				if (mkdir(MATT_DAEMON_LOG_PATH, 0755) == -1)
					throw std::runtime_error("Failed to create directory: " MATT_DAEMON_LOG_PATH);
			_file.open(logFilePath, std::ios::app);
			if (!_file.is_open())
				throw std::runtime_error("Failed to open log file: " + logFilePath);
		}
		/*
			Calls log with LogType::NONE
		*/
		void	log(const std::string &str);
		/*
			Logs a message with LogType "header" ([ LOG ], [ INFO ] ...)
		*/
		void	log(LogType type, const std::string &str);
	private:
		void	_log(const std::string &str);
		
		/*
			Returns the timestamp in this format [D/M/Y-H:m:s]
		*/
		std::string	_getLogTimeString();
		/*
			Returns the timestamp in this format D_M_Y_H_m_s
		*/
		std::string	_getLogFileTimeString();
		
		std::ofstream	_file;
};

#endif
