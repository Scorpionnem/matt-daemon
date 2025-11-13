/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tintin_reporter.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 13:35:51 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/13 16:16:31 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Tintin_reporter.hpp"


void	Tintin_reporter::init()
{
	std::string logFilePath = VAR_LOG_PATH MATT_DAEMON_LOG_PATH LOG_PATH + _getLogFileTimeString() + LOG_EXTENSION;

	if (!std::filesystem::exists(VAR_LOG_PATH MATT_DAEMON_LOG_PATH))
		std::filesystem::create_directories(VAR_LOG_PATH MATT_DAEMON_LOG_PATH);

	_file.open(logFilePath, std::ios::app);
	if (!_file.is_open())
		throw std::runtime_error("Failed to open log file: " + logFilePath);
}


// LOG FUNCTIONS
void	Tintin_reporter::log(const std::string &str)
{
	log(LogType::NONE, str);
}

void	Tintin_reporter::log(LogType type, const std::string &str)
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
		case LogType::CMD:
			header = "[ CMD ] - ";
			break ;
		default :
			header = " ";
			break ;
	}

	_log(header + str);
}

void	Tintin_reporter::_log(const std::string &str)
{
	if (str.find('\n') != str.npos)
		_file << _getLogTimeString() << " " << str << std::flush;
	else
		_file << _getLogTimeString() << " " << str << std::endl;
}

Tintin_reporter::Tintin_reporter() {}

Tintin_reporter::~Tintin_reporter() {}

void	Tintin_reporter::close()
{
	_file.close();
}

// TIME FORMATS
std::string	Tintin_reporter::_getLogFileTimeString()
{
	std::time_t t = std::time(0);
	std::tm* tm = std::localtime(&t);
	char buf[64];

	std::strftime(buf, sizeof(buf), "%d_%m_%Y_%H_%M_%S", tm);
	return std::string(buf);
}

std::string	Tintin_reporter::_getLogTimeString()
{
	std::time_t t = std::time(0);
	std::tm* tm = std::localtime(&t);
	char buf[64];

	std::strftime(buf, sizeof(buf), "[%d/%m/%Y-%H:%M:%S]", tm);
	return std::string(buf);
}
