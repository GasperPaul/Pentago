/*
 * NetCommunication.cpp
 *
 *  Created on: 15 груд. 2013
 *      Author: Youw
 */

#include "NetCommunication.h"

int ReceiveStr(SOCKET from, string& key, string& value) {
	int iResult, iSum = 0;
	int32_t sizeVal, sizeKey;
	char *buf;
	iSum += iResult = recv(from, (char*) &sizeKey, sizeof(sizeKey), MSG_WAITALL);
	if (iResult <= 0) {
		return iResult;
	}
	iSum += iResult = recv(from, (char*) &sizeVal, sizeof(sizeVal), MSG_WAITALL);
	if (iResult <= 0) {
		return iResult;
	}
	if (sizeKey > 0) {
		buf = new char[sizeKey];
		iSum += iResult = recv(from, buf, sizeKey, MSG_WAITALL);
		if (iResult <= 0) {
			delete buf;
			return iResult;
		}
		key.assign(buf, sizeKey);
		delete buf;
	}
	if (sizeVal > 0) {
		buf = new char[sizeVal];
		iSum += iResult = recv(from, buf, sizeVal, MSG_WAITALL);
		if (iResult <= 0) {
			delete buf;
			return iResult;
		}
		value.assign(buf, sizeVal);
		delete buf;
	}
	return iSum;
}

int SendStr(const string& key, const string& value, SOCKET to) {
	size_t buf_size = key.length() + value.length() + 2 * sizeof(int32_t);
	char *sendbuf = new char[buf_size];
	((int32_t*) sendbuf)[0] = key.length();
	((int32_t*) sendbuf)[1] = value.length();
	memcpy((char*) (((int32_t*) sendbuf) + 2), key.c_str(), key.length());
	memcpy(((char*) (((int32_t*) sendbuf) + 2)) + key.length(), value.c_str(), value.length());
	int iResult = send(to, sendbuf, buf_size, 0);
	delete[] sendbuf;
	if (iResult == SOCKET_ERROR) {
		return iResult;
	}
	return 0;
}
