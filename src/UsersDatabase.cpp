/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UsersDatabase.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 14:19:16 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/12 14:21:50 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UsersDatabase.hpp"

std::string	UsersDatabase::_sha256(const std::string& input)
{
	EVP_MD_CTX* context = EVP_MD_CTX_new();
	if (!context) throw std::runtime_error("EVP_MD_CTX_new failed");

	const EVP_MD* md = EVP_sha256();
	if (!EVP_DigestInit_ex(context, md, nullptr))
		throw std::runtime_error("EVP_DigestInit_ex failed");

	if (!EVP_DigestUpdate(context, input.c_str(), input.size()))
		throw std::runtime_error("EVP_DigestUpdate failed");

	unsigned char hash[EVP_MAX_MD_SIZE];
	unsigned int lengthOfHash = 0;

	if (!EVP_DigestFinal_ex(context, hash, &lengthOfHash))
		throw std::runtime_error("EVP_DigestFinal_ex failed");

	EVP_MD_CTX_free(context);

	std::stringstream ss;
	for (unsigned int i = 0; i < lengthOfHash; ++i)
		ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];

	return (ss.str());
}

void	UsersDatabase::addUser(const std::string &user, const std::string &pass)
{
	if (userExists(user))
		throw std::runtime_error("User already exists in database");

	_users.insert({user, _sha256(pass)});
}

bool	UsersDatabase::userExists(const std::string &user)
{
	if (_users.find(user) == _users.end())
		return (false);
	return (true);
}

bool	UsersDatabase::passMatch(const std::string &user, const std::string &pass)
{
	if (!userExists(user))
		throw std::runtime_error("Tried to match pass for a user that does not exist");

	if (_users.find(user)->second == _sha256(pass))
		return (true);
	return (false);
}

void	UsersDatabase::loadDB()
{
	std::ifstream	file(USERSDB_FILE);
	if (!file.is_open())
		throw std::runtime_error("Failed to open" USERSDB_FILE);
	
	std::string	line;

	while (getline(file, line))
	{
		std::istringstream	stream(line);
		std::string			user;
		std::string			pass;

		if (!(stream >> user >> pass))
			std::cerr << "Invalid line in users database" << std::endl;

		if (_users.find(user) == _users.end())
			_users.insert({user, pass});
		else
			std::cerr << "Duplicate user in database" << std::endl;
	}
}

void	UsersDatabase::exportDB()
{
	std::ofstream	file(USERSDB_FILE);
	if (!file.is_open())
		throw std::runtime_error("Failed to open" USERSDB_FILE);

	for (auto pair : _users)
		file << pair.first << " " << pair.second << std::endl;
}
