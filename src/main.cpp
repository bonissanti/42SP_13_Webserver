/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunrodr <brunrodr@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 16:47:10 by brunrodr          #+#    #+#             */
/*   Updated: 2024/07/16 18:45:08 by brunrodr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Parser.hpp"

int main(int argc, char **argv)
{
	std::string aaaa = "";
	try
	{
		if (argc != 2)
			throw Parser::exception(RED "Error: invalid number of arguments" RESET);
		Parser server(argv[1]);
	}
	catch(const Parser::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	(void)aaaa;
}
