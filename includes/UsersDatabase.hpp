/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UsersDatabase.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 14:18:09 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/12 14:23:49 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USERSDATABASE_HPP
# define USERSDATABASE_HPP

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <openssl/evp.h>
#include <map>
#include <fstream>

# define USERSDB_FILE "users"

class	UsersDatabase
{
	public:
		UsersDatabase() {}
		~UsersDatabase() {}

		void	loadDB();
		void	exportDB();

		/*
			Adds a new user in the db, will throw if the user already exists

			@param user username of the user to add
			@param pass password to give the user (hashed by the function)
		*/
		void	addUser(const std::string &user, const std::string &pass);

		/*
			Returns wether the user exists in the db or not
		*/
		bool	userExists(const std::string &user);

		/*
			Returns wether the password matches the hashed one from the database.

			@param user user whos password will be checked
			@param pass password to try and match (Dont give a hashed pass)
		*/
		bool	passMatch(const std::string &user, const std::string &pass);
	private:
		std::string	_sha256(const std::string& input);

		std::map<std::string, std::string>	_users;
};

#endif
