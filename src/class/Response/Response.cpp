#include "Response.hpp"
#include "../../../include/Utils.hpp"
#include <sys/stat.h>
#include <cstring>

Response::Response() {}

Response::~Response() {}

string Response::buildMessage(void)
{
    // string response =
    //     "HTTP/1.1 200 OK\r\n"
    //     "Content-Type: text/plain\r\n"
    //     "Content-Length: 17\r\n"
    //     "Connection: close\r\n"
    //     "\r\n"
    //     "Hello from server";W

    string response =
        "HTTP/1.1 " + Utils::statusCodeToString(_statusCode) + "\r\n"
        "Access-Control-Allow-Credentials: true\r\n"
        "Access-Control-Allow-Headers: Content-Type, Authorization\r\n"
        "Access-Control-Allow-Methods: DELETE, GET, POST\r\n"
        "Content-Length:" + _contentLength +
        "\r\n"
        "Content-Type:" + _contentType +
        "\r\n"
        "Connection: close" +
        "\r\n"
        "\r\n" +
        _responseBody;

    return (response);
}



void Response::clear() {
    _statusCode = 0;
    _index.clear();
    _executor.clear();
    _statusMessage.clear();
    _responseBody.clear();
    _filePath.clear();
    _contentType.clear();
    _contentLength.clear();
    _headers.clear();
}
