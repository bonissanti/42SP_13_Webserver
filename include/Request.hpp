#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <map>
#include "defines.hpp"

class Request {
    public:
        Request(const string &raw_request);
        ~Request();
		Request();

        string  getMethod() const;
        string  getPath() const;
		string  getVersion() const;
        string  getHeader(const string &field) const;
        string  getBody() const;

        void    printRequest() const;
        bool    validateRequest(string& errorResponse) const;
        void    parseRequest(const string &raw_request);

    private:
		bool    validateMethod() const;
        bool    validateHeaders() const;
        bool    validateVersion() const;

        void parseRequestLine(const string &line);
        void parseHeaders(istringstream &request_stream);
        void parseBody(istringstream &request_stream);

        string generateErrorResponse(int statusCode) const;

        map<string, string> headers_;
        string  method_;
        string  path_;
        string  version_;
        string  body_;
};

void handle_request(int client_socket);

#endif  // REQUEST_HPP
