
#ifndef SocketIncludes_h
#define SocketIncludes_h 1

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>

#ifdef _MSC_VER 
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#endif //_MSC_VER

#elif defined(__linux__) || defined(__APPLE__) //_WIN32

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#define SOCKET_ERROR -1
#define INVALID_SOCKET (~0)
#define SOCKET int
#define closesocket(x) close(x)
#define SD_BOTH 2

#endif //_WIN32

#endif //SocketIncludes_h
