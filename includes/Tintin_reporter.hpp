/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tintin_reporter.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 18:07:01 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/21 19:30:38 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TINTIN_REPORTER_HPP
# define TINTIN_REPORTER_HPP

// [ DD / MM / YYYY - HH : MM : SS]

# include <fstream>
# include <iostream>
# include <ctime>
# include <iomanip>
# include <exception>

///var/log/matt_daemon/
# define LOG_PATH "matt_daemon.log"

enum class LogType
{
	LOG,
	INFO,
	ERROR,
	NONE,
};

class	Tintin_reporter
{
	public:
		static void	log(const std::string &str)
		{
			log(LogType::NONE, str);
		}
		static void	log(LogType type, const std::string &str)
		{
			std::string	header;

			switch (type)
			{
				case LogType::LOG:
					header = "[ LOG ] - ";
					break ;
				case LogType::INFO:
					header = "[ INFO ] - ";
					break ;
				case LogType::ERROR:
					header = "[ ERROR ] - ";
					break ;
				default :
					header = " ";
					break ;
			}

			getInstance()._log(header + str);
		}
		
		static Tintin_reporter& getInstance()
		{
			static Tintin_reporter	instance;
			return (instance);
		}
	private:
		Tintin_reporter()
		{
			std::string logFilePath = LOG_PATH;

			_file.open(logFilePath, std::ios::app);
			if (!_file.is_open())
				throw std::runtime_error("Failed to open log file: " + logFilePath);
		}
		~Tintin_reporter()
		{
			_file.close();
		}

		void	_log(const std::string &str)
		{
			_file << _getTimeString() << " " << str << std::endl;
		}
		
		std::string _getTimeString()
		{
			std::time_t t = std::time(0);
			std::tm* tm = std::localtime(&t);
			char buf[64];
		
			std::strftime(buf, sizeof(buf), "[%d/%m/%Y-%H:%M:%S]", tm);
			return std::string(buf);
		}
		std::ofstream	_file;
};

#endif
