#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "../Server/Server.hpp"
#include "../Utils/Utils.hpp"

class Request {
    private:
        map<string, string> _headers;
        map<string, vector<char> > _formData;
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
        void parseMultidata(istringstream &request_stream, const string &boundary);
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
        Request(Server& server);
        Request(const string& raw_request, Server& server);
        Request();
        Request(const Request& toCopy);
        ~Request();
        Request& operator=(const Request& other);

        string getHeader(const string& field) const;
        string getMethod() const;
        string getURI() const;
        string getVersion() const;
        map<string, string> getHeaders() const;
        map<string, vector<char> > getFormData() const;
        string getBody() const;
        bool getIsCgi() const;
        int getStatusCode() const;
        map<int, Request> getRequest() const;
        Server& getServer();
        bool getIsReadyForResponse() const;

        int setStatusCode(HttpStatus code);

        void printRequest() const;
        bool validateRequest();
        bool isRequestComplete();
        void parseRequest(const string& raw_request);
        // void readRequest(struct pollfd& actualFd);
        // void readRequest(vector<struct pollfd> &pollFds, int i);
        static void readRequest(vector<struct pollfd> &pollFds, int i, map<int, Request> &requests, Server server);
        // void readRequest(int clientFd);
        void clear();
        friend class Response;
};

#endif  // REQUEST_HPP
