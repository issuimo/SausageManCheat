#pragma once
#include <windows.h>
#include <WinSock2.h>
#include <functional>
#include <ws2tcpip.h>
#include <socketapi.h>

#include "Logger.h"

#pragma comment(lib,"ws2_32.lib")

class Socket {
	static void InitWSA() {
		static bool init = false;
		
		WSADATA  wd;
		if (!init && WSAStartup(MAKEWORD(2, 2), &wd) != 0) {
			LOG_ERROR("初始化套接字失败: %d", WSAGetLastError());
			return;
		}
		LOG_INFO("初始化套接字成功");

		init = true;
	}

	class Server {
	public:
		bool Build_IPv4(const int type = SOCK_STREAM, const int protocol = IPPROTO_TCP, const int port = 0, const unsigned long ip = INADDR_ANY) {
			// 创建套接字
			sock = socket(AF_INET, type, protocol);
			if (sock == INVALID_SOCKET) {
				LOG_ERROR("创建套接字失败: %d", WSAGetLastError());
				return false;
			}

			// 绑定IP和端口
			sockaddr_in addr;
			addr.sin_family = AF_INET;
			addr.sin_port = htons(port);
			addr.sin_addr.S_un.S_addr = htonl(ip);
			int iRet = bind(sock, (sockaddr*)&addr, sizeof(addr));
			if (iRet == SOCKET_ERROR) {
				LOG_ERROR("绑定套接字失败: %d", WSAGetLastError());
				return false;
			}

			iPort = port;
			Ip = ip;

			return true;
		}

		bool Build_IPv6(const int type = SOCK_STREAM, const int protocol = IPPROTO_TCP, const int port = 0, const std::string& ip = "fe80::ce6:3cc:f93a:4203%5") {
			// 创建套接字
			sock = socket(AF_INET6, type, protocol);
			if (sock == INVALID_SOCKET) {
				LOG_ERROR("创建套接字失败: %d", WSAGetLastError());
				return false;
			}

			// 绑定IP和端口
			sockaddr_in6 addr;
			addr.sin6_family = AF_INET6;
			addr.sin6_port = htons(port);
			inet_pton(AF_INET6, ip.data(), &addr.sin6_addr);
			int iRet = bind(sock, (sockaddr*)&addr, sizeof(addr));
			if (iRet == SOCKET_ERROR) {
				LOG_ERROR("绑定套接字失败: %d", WSAGetLastError());
				return false;
			}

			iPort = port;
			Ipv6 = ip;

			return true;
		}

		// 保留给自定义socket
		void SetSocket(const SOCKET _sock) { sock = _sock; }
		[[nodiscard]] SOCKET GetSocket() const { return sock; }
		void Close() {
			closesocket(sock);
			sock = INVALID_SOCKET;
		}

		void ListenAndAccept_IPv4(int backlog = 5, const std::function<void(SOCKET, struct socketaddr_in)>& acceptCallBack = nullptr) {
			if (!acceptCallBack) {
				LOG_ERROR("未指定回调函数");
				return;
			}
			listen(sock, backlog);

			while (true) {
				sockaddr_in addr;
				int addrLen = sizeof(sockaddr_in);
				int iRet = accept(sock, (sockaddr*)&addr, &addrLen);
				if (iRet == INVALID_SOCKET) {
					LOG_ERROR("接受连接失败: %d", WSAGetLastError());
					return;
				}
				LOG_INFO("接受连接成功: %s:%d", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
				std::thread(acceptCallBack, sock, addr).detach();
			}
		}

		void ListenAndAccept_IPv6(int backlog = 5, const std::function<void(SOCKET, sockaddr_in6)>& acceptCallBack = nullptr) {
			if (!acceptCallBack) {
				LOG_ERROR("未指定回调函数");
				return;
			}
			listen(sock, backlog);

			while (true) {
				sockaddr_in6 addr;
				int addrLen = sizeof(sockaddr_in6);
				int iRet = accept(sock, (sockaddr*)&addr, &addrLen);
				if (iRet == INVALID_SOCKET) {
					LOG_ERROR("接受连接失败: %d", WSAGetLastError());
					return;
				}
				char ipbuf[100] = { 0 };
				LOG_INFO("接受连接成功: %s:%d", inet_ntop(AF_INET6, &addr.sin6_addr, ipbuf, 100), ntohs(addr.sin6_port));
				std::thread(acceptCallBack, sock, addr).detach();
			}
		}

		template <typename T>
		static bool Send_IPv4(const socketaddr_in socketaddr, const T* data, const int len = sizeof(T)) {
			const int iRet = send(socketaddr, reinterpret_cast<const char*>(data), len, 0);
			if (iRet == SOCKET_ERROR) {
				LOG_ERROR("发送失败: %d | %s:%d", WSAGetLastError(), inet_ntoa(socketaddr.sin_addr), ntohs(socketaddr.sin_port));
				return false;
			}
			return true;
		}

		template <typename T>
		static bool Send_IPv6(const sockaddr_in6 socketaddr, const T* data, const int len = sizeof(T)) {
			const int iRet = send(socketaddr, reinterpret_cast<const char*>(data), len, 0);
			if (iRet == SOCKET_ERROR) {
				char ipbuf[100] = { 0 };
				LOG_ERROR("发送失败: %d | %s:%d", WSAGetLastError(), inet_ntop(AF_INET6, &socketaddr.sin6_addr, ipbuf, 100), ntohs(socketaddr.sin6_port));
				return false;
			}
			return true;
		}

		template <typename T>
		static int Recv_IPv4(const socketaddr_in socketaddr, const T* data, const int len = sizeof(T)) {
			const int iRet = recv(socketaddr, reinterpret_cast<const char*>(data), len, 0);
			if (iRet == SOCKET_ERROR) {
				LOG_ERROR("接收失败: %d | %s:%d", WSAGetLastError(), inet_ntoa(socketaddr.sin_addr), ntohs(socketaddr.sin_port));
				return iRet;
			}
			return iRet;
		}

		template <typename T>
		static int Recv_IPv6(const sockaddr_in6 socketaddr, const T* data, const int len = sizeof(T)) {
			const int iRet = recv(socketaddr, reinterpret_cast<const char*>(data), len, 0);
			if (iRet == SOCKET_ERROR) {
				char ipbuf[100] = { 0 };
				LOG_ERROR("接收失败: %d | %s:%d", WSAGetLastError(), inet_ntop(AF_INET6, &socketaddr.sin6_addr, ipbuf, 100), ntohs(socketaddr.sin6_port));
				return iRet;
			}
			return iRet;
		}
	private:
		SOCKET sock{0};
		int iPort{0};
		unsigned long Ip{INADDR_ANY};
		std::string Ipv6{INADDR_ANY};
	};

	class Client {
	public:
		bool Build_IPv4(const int type = SOCK_STREAM, const int protocol = IPPROTO_TCP, const int port = 0, const unsigned long ip = INADDR_ANY) {
			// 创建套接字
			sock = socket(AF_INET, type, protocol);
			if (sock == INVALID_SOCKET) {
				LOG_ERROR("创建套接字失败: %d", WSAGetLastError());
				return false;
			}

			// 绑定IP和端口
			addr.sin_family = AF_INET;
			addr.sin_port = htons(port);
			addr.sin_addr.S_un.S_addr = htonl(ip);
			int iRet = connect(sock, (sockaddr*)&addr, sizeof(addr));
			if (iRet == SOCKET_ERROR) {
				LOG_ERROR("连接套接字失败: %d", WSAGetLastError());
				return false;
			}

			iPort = port;
			Ip = ip;

			return true;
		}

		bool Build_IPv6(const int type = SOCK_STREAM, const int protocol = IPPROTO_TCP, const int port = 0, const std::string& ip = "fe80::ce6:3cc:f93a:4203%5") {
			// 创建套接字
			sock = socket(AF_INET6, type, protocol);
			if (sock == INVALID_SOCKET) {
				LOG_ERROR("创建套接字失败: %d", WSAGetLastError());
				return false;
			}

			// 绑定IP和端口
			addrIPv6.sin6_family = AF_INET6;
			addrIPv6.sin6_port = htons(port);
			inet_pton(AF_INET6, ip.data(), &addrIPv6.sin6_addr);
			int iRet = connect(sock, (sockaddr*)&addrIPv6, sizeof(addrIPv6));
			if (iRet == SOCKET_ERROR) {
				LOG_ERROR("连接套接字失败: %d", WSAGetLastError());
				return false;
			}

			iPort = port;
			Ipv6 = ip;

			return true;
		}

		// 保留给自定义socket
		void SetSocket(const SOCKET _sock) { sock = _sock; }
		[[nodiscard]] SOCKET GetSocket() const { return sock; }
		void Close() {
			closesocket(sock);
			sock = INVALID_SOCKET;
		}

		template <typename T>
		bool Send_IPv4(const T* data, const int len = sizeof(T)) {
			const int iRet = send(sock, reinterpret_cast<const char*>(data), len, 0);
			if (iRet == SOCKET_ERROR) {
				LOG_ERROR("发送失败: %d | %s:%d", WSAGetLastError(), inet_ntoa(addr.sin_addr), iPort);
				return false;
			}
			return true;
		}

		template <typename T>
		bool Send_IPv6(const T* data, const int len = sizeof(T)) {
			const int iRet = send(sock, reinterpret_cast<const char*>(data), len, 0);
			if (iRet == SOCKET_ERROR) {
				LOG_ERROR("发送失败: %d | %s:%d", WSAGetLastError(), Ipv6.data(), iPort);
				return false;
			}
			return true;
		}

		template <typename T>
		int Recv_IPv4(const T* data, const int len = sizeof(T)) {
			const int iRet = recv(sock, reinterpret_cast<const char*>(data), len, 0);
			if (iRet == SOCKET_ERROR) {
				LOG_ERROR("接收失败: %d | %s:%d", WSAGetLastError(), inet_ntoa(addr.sin_addr), iPort);
				return iRet;
			}
			return iRet;
		}

		template <typename T>
		int Recv_IPv6(const T* data, const int len = sizeof(T)) {
			const int iRet = recv(sock, reinterpret_cast<const char*>(data), len, 0);
			if (iRet == SOCKET_ERROR) {
				LOG_ERROR("接收失败: %d | %s:%d", WSAGetLastError(), Ipv6.data(), iPort);
				return iRet;
			}
			return iRet;
		}
	private:
		SOCKET sock{ 0 };
		int iPort{ 0 };
		unsigned long Ip{ INADDR_ANY };
		std::string Ipv6{ INADDR_ANY };
		sockaddr_in addr{};
		sockaddr_in6 addrIPv6{};
	};
};
