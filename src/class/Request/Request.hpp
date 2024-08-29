#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "../Utils/Utils.hpp"
#include "../Server/Server.hpp"

class Request {
    private:
        map<string, string> _headers;
        string _method;
        string _uri;
        string _version;
        string _body;
        string _buffer;
        Server _server;
        bool _isCgi;
        bool _readyForResponse;
        HttpStatus _statusCode;

        void parseRequestLine(const string& line);
        void parseHeaders(istringstream& request_stream);
        void parseBody(istringstream& request_stream);
        string generateErrorResponse(int statusCode) const;
        class exception : public std::exception {
            private:
                string msg;

            public:
                exception(const string& msg);
                virtual ~exception() throw();
                virtual const char* what() const throw();
        };

    public:
        Request(const string& raw_request, Server& server);
        ~Request();
        Request();
        Request& operator=(const Request &other);

        string getHeader(const string& field) const;
        string getMethod() const;
        string getURI() const;
        string getVersion() const;
        map<string, string> getHeaders() const;
        string getBody() const;
        bool getIsCgi() const;
        int getStatusCode() const;
        map<int, Request> getRequest() const;
        Server getServer() const;
        bool isReadyForResponse() const;

        int setStatusCode(HttpStatus code);

        void printRequest() const;
        bool validateRequest() const;
        bool isRequestComplete(const std::string& request);
        void parseRequest(const string& raw_request);
        static void readRequest(vector<struct pollfd>& pollFds, int i, map<int, Request>& requests);
        friend class Response;

        void clear();
};

#endif  // REQUEST_HPP
