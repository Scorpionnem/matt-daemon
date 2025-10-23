/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tintin_reporter.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 18:07:01 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/23 14:19:03 by mbatty           ###   ########.fr       */
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
		/*
			Calls log with LogType::NONE
		*/
		static void	log(const std::string &str);
		/*
			Logs a message with LogType "header" ([ LOG ], [ INFO ] ...)
		*/
		static void	log(LogType type, const std::string &str);

		/*
			Returns instance of Tintin_reporter singleton
		*/
		static Tintin_reporter& getInstance();
	private:
		void	_log(const std::string &str);
		
		Tintin_reporter();
		~Tintin_reporter();

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
