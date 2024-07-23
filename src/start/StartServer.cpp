/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StartServer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunrodr <brunrodr@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 18:21:01 by brunrodr          #+#    #+#             */
/*   Updated: 2024/07/23 15:24:02 by brunrodr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"

void	Server::startServer(std::vector<Server> servers)
{
	int	inUse = 1;
	int socketFd;
	struct sockaddr_in serverAddr;

	bzero(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
		
	for (size_t i = 0; i < servers.size(); i++)
	{
		serverAddr.sin_port = htons(servers[i]._listen);
		socketFd = socket(AF_INET, SOCK_STREAM, 0);

		if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &inUse, sizeof(int)) == -1)
				std::cout << "oi" << std::endl;	
	
		if (bind(socketFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
		{
			if (errno == EADDRINUSE)
				std::cout << "RIP" << std::endl;
			else
				std::cout << "lalal" << std::endl;
		}
		
		if (listen(socketFd, 10) < 0)
		{
			std::cout << "RIP 2" << std::endl;
		}
	}
}