#include "client/includes.hpp"


void errorHandler(int err, Client &client)
{
    std::string reply;

    // 401 - Target nickname does not exist.
    // Example: PRIVMSG, INVITE, MODE.
    if (err == 401)
        reply = ":irc.local 401 " + client.getNickname() + " :No such nick/channel\r\n";

    // 403 - Channel does not exist.
    // Example: JOIN, PART, PRIVMSG, MODE, TOPIC, KICK.
    else if (err == 403)
        reply = ":irc.local 403 " + client.getNickname() + " :No such channel\r\n";

    // 404 - Cannot send messages to the channel.
    // Example: channel is moderated or user is not allowed to speak.
    else if (err == 404)
        reply = ":irc.local 404 " + client.getNickname() + " :Cannot send to channel\r\n";

    // 411 - PRIVMSG/NOTICE without a recipient.
    else if (err == 411)
        reply = ":irc.local 411 " + client.getNickname() + " :No recipient given (PRIVMSG)\r\n";

    // 412 - PRIVMSG without a message.
    else if (err == 412)
        reply = ":irc.local 412 " + client.getNickname() + " :No text to send\r\n";

    // 421 - Unknown command.
    else if (err == 421)
        reply = ":irc.local 421 " + client.getNickname() + " :Unknown command\r\n";

    // 431 - NICK command without a nickname.
    else if (err == 431)
        reply = ":irc.local 431 * :No nickname given\r\n";

    // 432 - Invalid nickname.
    else if (err == 432)
        reply = ":irc.local 432 " + client.getNickname() + " :Erroneous nickname\r\n";

    // 433 - Nickname already in use.
    else if (err == 433)
        reply = ":irc.local 433 " + client.getNickname() + " :Nickname is already in use\r\n";

    // 441 - Target user is not in the channel.
    // Example: KICK, INVITE, MODE.
    else if (err == 441)
        reply = ":irc.local 441 " + client.getNickname() + " :They aren't on that channel\r\n";

    // 442 - Client is not on the specified channel.
    // Example: PART, TOPIC, MODE, KICK, PRIVMSG.
    else if (err == 442)
        reply = ":irc.local 442 " + client.getNickname() + " :You're not on that channel\r\n";

    // 451 - Client has not completed PASS/NICK/USER registration.
    else if (err == 451)
        reply = ":irc.local 451 * :You have not registered\r\n";

    // 461 - Command missing required parameters.
    else if (err == 461)
        reply = ":irc.local 461 " + client.getNickname() + " :Not enough parameters\r\n";

    // 462 - PASS or USER sent after registration.
    else if (err == 462)
        reply = ":irc.local 462 " + client.getNickname() + " :You may not reregister\r\n";

    // 464 - Incorrect PASS password.
    else if (err == 464)
        reply = ":irc.local 464 * :Password incorrect\r\n";

    // 471 - JOIN failed because channel is full (+l).
    else if (err == 471)
        reply = ":irc.local 471 " + client.getNickname() + " :Cannot join channel (+l)\r\n";

    // 472 - Unknown MODE flag.
    else if (err == 472)
        reply = ":irc.local 472 " + client.getNickname() + " :is unknown mode char to me\r\n";

    // 473 - JOIN failed because channel is invite-only (+i).
    else if (err == 473)
        reply = ":irc.local 473 " + client.getNickname() + " :Cannot join channel (+i)\r\n";

    // 474 - JOIN failed because client is banned (+b).
    else if (err == 474)
        reply = ":irc.local 474 " + client.getNickname() + " :Cannot join channel (+b)\r\n";

    // 475 - JOIN failed because the channel key is incorrect (+k).
    else if (err == 475)
        reply = ":irc.local 475 " + client.getNickname() + " :Cannot join channel (+k)\r\n";

    // 476 - Invalid channel name.
    else if (err == 476)
        reply = ":irc.local 476 " + client.getNickname() + " :Bad Channel Mask\r\n";

    // 482 - Client is not a channel operator.
    // Example: KICK, MODE, TOPIC, INVITE.
    else if (err == 482)
        reply = ":irc.local 482 " + client.getNickname() + " :You're not channel operator\r\n";

    if (!reply.empty())
        send(client.getFd(), reply.c_str(), reply.length(), 0);
}