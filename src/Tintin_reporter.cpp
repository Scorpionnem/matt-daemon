/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tintin_reporter.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 13:35:51 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/23 14:20:21 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Tintin_reporter.hpp"

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
		default :
			header = " ";
			break ;
	}

	getInstance()._log(header + str);
}

void	Tintin_reporter::_log(const std::string &str)
{
	_file << _getLogTimeString() << " " << str << std::endl;
}

Tintin_reporter& Tintin_reporter::getInstance()
{
	static Tintin_reporter	instance;
	return (instance);
}

Tintin_reporter::Tintin_reporter()
{
	std::string logFilePath = MATT_DAEMON_LOG_PATH LOG_PATH + _getLogFileTimeString() + LOG_EXTENSION;

	
	if (!std::filesystem::exists(MATT_DAEMON_LOG_PATH))
		if (mkdir(MATT_DAEMON_LOG_PATH, 0755) == -1)
			throw std::runtime_error("Failed to create directory: " MATT_DAEMON_LOG_PATH);
	_file.open(logFilePath, std::ios::app);
	if (!_file.is_open())
		throw std::runtime_error("Failed to open log file: " + logFilePath);
}

Tintin_reporter::~Tintin_reporter()
{
	_file.close();
}

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
