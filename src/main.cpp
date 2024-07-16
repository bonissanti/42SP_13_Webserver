/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunrodr <brunrodr@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 16:47:10 by brunrodr          #+#    #+#             */
/*   Updated: 2024/07/15 21:54:08 by brunrodr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Webserver.hpp"

int main(int argc, char **argv)
{
	(void)argv;
	try
	{
		if (argc != 2)
			throw Webserver::exception(RED "Error: invalid number of arguments" RESET);
	}
	catch(const Webserver::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}
