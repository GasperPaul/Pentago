/*
 * NetCommunication.h
 *
 *  Created on: 15 dec. 2013
 *      Author: Youw
 */

#ifndef NETCOMMUNICATION_H_
#define NETCOMMUNICATION_H_
#include "SocketIncludes.h"

#include <string>

using std::string;

extern int ReceiveStr(SOCKET from, string& key, string& value);

extern int SendStr(const string& key, const string& value, SOCKET to);

#endif /* NETCOMMUNICATION_H_ */
