#include "Request.hpp"

Request::Request(const string &raw_request) {
    parseRequest(raw_request);
}

Request::~Request() {}

string Request::getMethod() const {
    return method_;
}

string Request::getPath() const {
    return path_;
}

string Request::getBody() const {
    return body_;
}

string Request::getHeader(const string &field) const {
    map<string, string>::const_iterator it = headers_.find(field);
    if (it != headers_.end()) {
        return it->second;
    }
    return "";
}

void Request::parseRequest(const string &raw_request) {
    istringstream request_stream(raw_request);
    string line;
    getline(request_stream, line);
    
    istringstream line_stream(line);
    line_stream >> method_ >> path_ >> version_;
    // getline(line_stream, version_, ' ');

    while (getline(request_stream, line) && line != "\r") {
        size_t colon_pos = line.find(':');
        if (colon_pos != string::npos) {
            string key = line.substr(0, colon_pos);
            string value = line.substr(colon_pos + 2, line.length() - colon_pos - 3);
            headers_[key] = value;
        }
    }

    // The rest of the request_stream, if any, is the body
    getline(request_stream, body_, '\0');
}
      