#ifndef DEFINE_HPP
# define DEFINE_HPP

//ERROR

# define LOG_NOT_LOGIN(cl_id) std::to_string(cl_id) + " not connected"

# define LOG_CMD_UNKNOW(cl_id) std::to_string(cl_id) + " command doesn't exist"

# define CL_NOT_LOGIN "Please login first. (/login <user> <pass>)\n\r"

# define CL_CMD_UNKNOW "Bad command. You can use /help.\n\r"

//LOGIN

/*LOGGER*/
# define LOGIN_CMD(cl_id) std::to_string(cl_id) + " used /login"

# define LOGIN_LOG_FAILED(cl_id) std::to_string(cl_id) + " failed /login"

# define LOGIN_LOG_ALRD_CO(cl_id) std::to_string(cl_id) + " used /login : Already connected"

# define LOGIN_LOG_ERR_DB(cl_id) std::to_string(cl_id) + " used /login : Name already choosed or bad password"

# define LOGIN_LOG_SUCCESS(cl_id) std::to_string(cl_id) + " used /login : Connexion successfull"

/*CLIENT*/
# define LOGIN_CL_INV_ARG "/login : Invalid arguments\n\r"

# define LOGIN_CL_ALRD_CO "/login : You're already connected\n\r"

# define LOGIN_CL_ERR_DB "/login : Choosed another name or wrong password\n\r"

# define LOGIN_CL_SUCCESS "/login : Connexion successfull\n\r"

//LIST

/*LOGGER*/

# define LIST_CMD(cl_id) std::to_string(cl_id) + " used /list"

# define LIST_LOG_BAD_FLAG(cl_id) std::to_string(cl_id) + " /list: invalid option"

/*CLIENT*/

# define LIST_CL_BAD_FLAG " /list: invalid option. Try with -a\n\r"

# define LIST_CL(name) name + "\n"

//HELP

/*LOGGER*/

# define HELP_CMD(cl_id) std::to_string(cl_id) + " used /help"

# define HELP_LOG_BAD_FLAG(cl_id) std::to_string(cl_id) + " /help: invalid option"

/*CLIENT*/

# define HELP_CL_BAD_FLAG " /help: invalid option. No flag (coming soon)\n\r"

# define HELP_CL_CMD "Commands :\n/login <user> <pass>\n/leave\n/msg <user> : <msg>\n/list (-a)\n/help\n/quit or quit\n\r"

//PRIVMSG

/*LOGGER*/
# define PRIVMSG_CMD(cl_id) std::to_string(cl_id) + " used /msg"

# define PRIVMSG_LOG_ERR_ARG(cl_id) std::to_string(cl_id) + " used /msg : Invalid argument(s)"

# define PRIVMSG_LOG_ERR_USER_NOT_FOUND(cl_id) std::to_string(cl_id) + " used /msg : User doesn't exist or not connected"

/*CLIENT*/

# define PRIVMSG_CL_ERR_ARG "/msg : Invalid argument(s). /msg <user> : <msg>\n\r"

# define PRIVMSG_CL_ERR_USER_NOT_FOUND "/msg : Target doesn't exist or not connected\n\r"

//MSG

/*LOGGER*/
# define MSG_CMD(cl_id) std::to_string(cl_id) + " send a message"

/*CLIENT*/
# define MSG_CL(sender, msg) "From " + sender + " : " + msg + "\n\r"

#endif