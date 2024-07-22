/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunrodr <brunrodr@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 13:59:37 by brunrodr          #+#    #+#             */
/*   Updated: 2024/07/22 17:58:15 by brunrodr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Config.hpp"
#include "Request.hpp"
#include "defines.hpp"

typedef enum s_error_page {
    DEFAULT = 0,
    OK = 200,
    BAD_REQUEST = 400,
    FORBIDDEN = 403,
    NOT_FOUND = 404,
    METHOD_NOT_ALLOWED = 405,
    INTERNAL_SERVER_ERROR = 500,
    BAD_GATEWAY = 502
} t_error_page;

typedef struct s_route {
        bool        autoindex;
        std::string root;
        std::string allow_methods;
        std::string index;
        std::string cgi;
        std::string redirect;
} t_route;

class Server {
    private:
        int                       _listen;
        std::string               _server_name;
        std::string               _host;
        std::string               _root;
        int                       _client_max_body_size;
        std::vector<t_error_page> _error_page;
        std::vector<t_route>      _route;

    public:
        Server(void);
        ~Server();

        t_route createRoute();
        void    setListen(int port);
        void    setServerName(std::string name);
        void    setHost(std::string host);
        void    setBodySize(std::string size);
        void    setRoute(std::vector<std::string> routeLines, size_t& i);

        void    creatingServers(int numServers, std::vector<std::string> lines);
        // void start(); // void loadConfig(const std::string& config_path);
        // void handleClient(int client_socket);
        // Config              config_;

        class exception : public std::exception {
            private:
                std::string msg;

            public:
                exception(const std::string& msg);
                virtual ~exception() throw();
                virtual const char* what() const throw();
        };
};

std::string    setRoot(std::string root);

#endif  // SERVER_HPP
