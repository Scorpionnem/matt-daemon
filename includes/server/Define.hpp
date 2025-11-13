#ifndef DEFINE_HPP
# define DEFINE_HPP

//LOGIN

/*LOGGER*/
# define LOGIN_CMD(cl_id) std::to_string(cl_id) + " used /login"

# define LOGIN_LOG_FAILED(cl_id) std::to_string(cl_id) + " failed /login"

# define LOGIN_LOG_ALRD_CO(cl_id) std::to_string(cl_id) + " used /login : Already connected"

# define LOGIN_LOG_ERR_DB(cl_id) std::to_string(cl_id) + " used /login : Already connected"

# define LOGIN_LOG_SUCCESS(cl_id) std::to_string(cl_id) + " used /login : Connexion successfull"

/*CLIENT*/
# define LOGIN_CL_INV_ARG "/login : Invalid arguments\n\r"

# define LOGIN_CL_ALRD_CO "/login : You're already connected\n\r"

# define LOGIN_CL_ERR_DB "/login : Choosed another name or wrong password\n\r"

# define LOGIN_CL_SUCCESS "/login : Connexion successfull\n\r"

//PRIVMSG
# define ERR_ALREADYREGISTRED ":127.0.0.1 462 :You may not reregister\r\n"
# define ERR_PASSWDMISMATCH ":127.0.0.1 464 :Password incorrect\r\n"
# define ERR_NEEDMOREPARAMS(name, cmd) ":127.0.0.1 461 " + name + " " + cmd + " :Not enough parameters\r\n"
# define ERR_TOOMANYPARAMS(name,cmd) ":127.0.0.1 460 " + name + " " + cmd + " :Too many parameters\r\n"
# define AUTHENTIFICATED(name) ":127.0.0.1 " + name + " :Your are authentificated\r\n"
# define NOTAUTHENTIFICATED ":127.0.0.1 :Your are not authentificated\r\n"
# define ERR_NOTREGISTERED ":127.0.0.1 451 :You have not registered\r\n"
# define ERR_NICKNAMEINUSE(name) ":127.0.0.1 433 " + name + " :Name is already in use\r\n"
# define ERR_NONICKNAMEGIVEN ":127.0.0.1 431 :No nickname given\r\n"

# define QUIT(nameLeave, userNameLeave, ipLeave) ":" + nameLeave + "!" + userNameLeave + "@" + ipLeave + " QUIT :Client Quit\r\n"
# define CHANGENICKNAME(name) ":127.0.0.1 " + name + " :You have changed your Name\r\n"
# define CHANGENICKNAMEFORALL(name, ipUser, newName) ":" + name + "!" + "@" + ipUser + " NICK :" + newName + "\r\n"

# define SELECTNICKNAME(name) ":127.0.0.1 " + name + " :You have choose your Name\r\n"
# define SELECTUSER(name) ":127.0.0.1 " + name + " :You have choose your User and Realname\r\n"
# define ERR_NOSUCHNICK(name, nameSuch) ":127.0.0.1 401 " + name + " " + nameSuch + " :No such nick/channel\r\n"
# define ERR_NOSUCHCHANNEL(name) ":127.0.0.1 403 " + name + " :No such channel\r\n"
# define ERR_NORECIPIENT(cmd) ":127.0.0.1 411 :No recipient given " + cmd + "\r\n"
# define ERR_NOTEXTTOSEND ":127.0.0.1 412 :No text to send\r\n"
# define ERR_CANNOTSENDTOCHAN(channel) ":127.0.0.1 404 " + channel + " :Cannot send to channel\r\n"
# define ERR_INVITEONLYCHAN(channel) ":127.0.0.1 473 " + channel + " :Cannot join channel (+i)\r\n"
# define ERR_BADCHANNELKEY(channel) ":127.0.0.1 475 " + channel + " :Cannot join channel (+k)\r\n"
# define ERR_CHANNELISFULL(name, channel) ":127.0.0.1 471 " + name + " " + channel + " :Cannot join channel (+l) - channel is full, try again later\r\n"

# define MSGSEND(nameReceiver, msg) nameReceiver + " " + msg + "\r\n"
# define MSGJOIN(nameSender, userNameSender, ipSender) ":" + nameSender + "!" + userNameSender + "@" + ipSender + " JOIN " + "\r\n"

# define CHANNELLEAVE(nameSender, userNameSender, ipSender, channelReceiver) ":" + nameSender + "!" + userNameSender + "@" + ipSender + " " + channelReceiver + " :Leave\r\n"
# define USERDISCONNECTED(nameSender, ipSender, nickReceiver) ":" + nameSender + "!" + "@" + ipSender + " " + nickReceiver + " Disconnected\r\n"

# define CHANNELMODE(name, channel, msg) ":127.0.0.1 324 " + name + " " + channel + " " + msg + "\r\n"
# define CHANNELMODEJOIN(channel, msg) ":127.0.0.1 324 " + channel + " " + msg + "\r\n"
# define CHANNELEND(name, channel) ":127.0.0.1 366 " + name + " " + channel + " :End of /NAMES list." + "\r\n"


# define RPL_TOPIC(name, channel, topic) ":127.0.0.1 332 " + name + " " + channel + " " + topic + "\r\n"
# define RPL_NOTOPIC(channel) ":127.0.0.1 331 " + channel + " :No topic is set\r\n"
# define CHANELLTOPIC(name, channel, topic) ":127.0.0.1 " + name + " " + channel + " " + topic + "\r\n"
# define CHANGEDTOPIC(nameSender, userNameSender, ipSender, channel, topic) ":" + nameSender + "!" + userNameSender + "@" + ipSender + " TOPIC " + channel + " " + topic + "\r\n"

# define INVITE(nameReceiver, userNameReceiver, ipReceiver, invited, channel) ":" + nameReceiver + "!" + userNameReceiver + "@" + ipReceiver + " INVITE " + invited + " :" + channel + "\r\n" 
# define INVITESENDER(nameSender, invited, channel) ":127.0.0.1 341 " + nameSender + " " + invited + " " + channel + "\r\n"

# define KICK(nameReceiver, userNameReceiver, ipReceiver, userKicked, channel, msg) ":" + nameReceiver + "!" + userNameReceiver + "@" + ipReceiver + " KICK " + channel + " " + userKicked + msg + "\r\n"
# define LEAVE(nameReceiver, ipReceiver, msg) ":" + nameReceiver + "!" + "@" + ipReceiver + " PART " + " " + msg + "\r\n"

# define ERR_UNKNOWNMODE(name, char) ":127.0.0.1 472 " + name + " " + char + " :is unknown mode char to me\r\n"
# define ERR_NOTONCHANNEL(name, channel) ":127.0.0.1 442 " + name + " " + channel + " :You're not on that channel\r\n"
# define ERR_USERONCHANNEL(name, channel) ":127.0.0.1 443 " + name + " " + channel + " :is already on channel\r\n"
# define ERR_CHANALREADYOP(name, channel) ":127.0.0.1 482 " + name + " " + channel + " :is already on channel\r\n"
# define ERR_CHANNOTOPSNEEDED(name, channel) ":127.0.0.1 482 " + name + " " + channel + " :You're not a channel operator\r\n"
# define ERR_KEYSET(name, channel) ":127.0.0.1 467 " + name + " " + channel + " :Channel key already set\r\n"
# define CHANNELLIST(name, msg) ":127.0.0.1 353 " + name + " @ " + " :" + msg + "\r\n"


# define MSGPARAM(nameSender, userNameSender, ipSender, channel, msg, param) ":" + nameSender + "!" + userNameSender + "@" + ipSender + " MODE " + channel + " " + msg + " " + param + "\r\n"
# define ERR_UNKNOWNCOMMAND(name, cmd) ":127.0.0.1 421 " + name + " " + cmd + " :Unknown command\r\n"

#endif