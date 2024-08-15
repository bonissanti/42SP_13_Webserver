#include "Response.hpp"
#include "../../../include/Utils.hpp"
#include <sys/stat.h>

Response::Response(Request& req){
	// runRedirect ou..
	callMethod(req);
}

Response::~Response() {}

int	Response::getMethodIndex(string method){
	string types[] = {"GET", "POST", "DELETE"};

	for (size_t i = 0; i < 3; i++)
		if (types[i] == method)
			return (i);
	return (-1);
}

void Response::callMethod(Request& req){

	switch(getMethodIndex(req.getMethod())){
		case GET:
			runGetMethod(req);
			break;
		case POST:
			cout << "POST" << endl;
			runPostMethod(req);
			break;
		// case DELETE:
		// 	runDeleteMethod();
	}
}

string setContentType(string filePath)
{
	size_t index;
	string extension;	
	map<string, string> mimeTypes;
	mimeTypes[".html"] = "text/html";
	mimeTypes[".css"] = "text/css";
	mimeTypes[".txt"] = "text/plain";
	mimeTypes[".png"] = "image/png";
	mimeTypes[".jpg"] = "image/jpg";
	mimeTypes[".gif"] = "image/gif";
	mimeTypes[".js"] = "application/js";
	mimeTypes[".pdf"] = "application/pdf";

	index = filePath.rfind('.');
	if (index != string::npos){
		extension = filePath.substr(index);
		map<string, string>::iterator it = mimeTypes.begin();	
		
		for (; it != mimeTypes.end(); ++it)
		if (it->first == extension)
				cout << it->second << endl;
	}
	return ("text/plain");
}

/*  */

void Response::runGetMethod(Request& req){
	if (req._path.empty()){
		_statusCode = BAD_REQUEST;
		return ;
	}
	
	string filePath = req._path;
	if (filePath.length() - 1 == '/')
		filePath += "index.html";
	
	string contentType = setContentType(filePath);
	contentType += "; charset=UTF-8";
}

//-----------------POST FUNCTIONS----------------------

string getFilename(const string& contentDisposition) {
    size_t pos = contentDisposition.find("filename=");
    if (pos != string::npos) {
        size_t endPos = contentDisposition.find(";", pos);
        if (endPos == string::npos) {
            endPos = contentDisposition.length();
        }
        return contentDisposition.substr(pos + 10, endPos - pos - 10);
    }
    return "";
}

void Response::runPostMethod(Request& req) {
    string contentType = req.getHeader("content-type");
	cout << contentType << endl;
    if (contentType.find("multipart/form-data") != string::npos) {
        string boundary = contentType.substr(contentType.find("boundary=") + 9);
        map<string, string> formData = parseMultipartData(req.getBody(), boundary);

		string filename = getFilename(formData["filename"]);
		string directory = formData["directory"];
        for (map<string, string>::const_iterator it = formData.begin(); it != formData.end(); ++it) {
            if (it->first == "file") {
                saveUploadedFile(filename, it->second, directory);
            }
        }

        _statusCode = OK;
        _statusMessage = "OK";
        _body = "File uploaded successfully";
    } else {
        // Handle other POST data
        _statusCode = BAD_REQUEST;
        _statusMessage = "Bad Request";
        _body = "Unsupported Content-Type";
    }

    _headers["Content-Type"] = "text/plain";
}

map<string, string> Response::parseMultipartData(const string& body, const string& boundary) {
    map<string, string> formData;
    string delimiter = "--" + boundary;
    size_t start = body.find(delimiter) + delimiter.length();
    size_t end = body.find(delimiter, start);

    while (end != string::npos) {
        string part = body.substr(start, end - start);
        size_t headerEnd = part.find("\r\n\r\n");
        string headers = part.substr(0, headerEnd);
        string content = part.substr(headerEnd + 4, part.length() - headerEnd - 6);

        // Parse the Content-Disposition header
        size_t dispositionStart = headers.find("Content-Disposition: ");
        if (dispositionStart != string::npos) {
            size_t dispositionEnd = headers.find("\r\n", dispositionStart);
            string disposition = headers.substr(dispositionStart + 20, dispositionEnd - dispositionStart - 20);

            // Extract the name and filename from the Content-Disposition header
            size_t nameStart = disposition.find("name=\"");
            size_t nameEnd = disposition.find("\"", nameStart + 6);
            string name = disposition.substr(nameStart + 6, nameEnd - nameStart - 6);

            size_t filenameStart = disposition.find("filename=\"");
            size_t filenameEnd = disposition.find("\"", filenameStart + 10);
            string filename = "";
            if (filenameStart != string::npos) {
                filename = disposition.substr(filenameStart + 10, filenameEnd - filenameStart - 10);
            }

            // Add the form field to the formData map
            if (!filename.empty()) {
                formData[name] = filename;
            } else {
                formData[name] = content;
            }
        }

        start = end + delimiter.length();
        end = body.find(delimiter, start);
    }

    return formData;
}

bool directoryExists(const std::string& path) {
    struct stat sb;
    if (stat(path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)) {
        return true;
    }
    return false;
}

void Response::saveUploadedFile(const string& filename, const string& fileContent, const string& directory) {
	cout << "Saving file" << endl;
	if (!directoryExists(directory)) {
		mkdir(directory.c_str(), 0777);
	}

	string finalFilename = filename.empty() ? "default_filename.txt" : filename;
	cout << "Final filename: " << finalFilename << endl;

	string path = directory + "/" + finalFilename;
    ofstream outFile(path.c_str(), ios::binary);
    outFile.write(fileContent.c_str(), fileContent.size());
    outFile.close();
}

string Response::getResponse() const {
	string response = "HTTP/1.1 " + to_string(_statusCode) + " " + _statusMessage + "\r\n";
	for (map<string, string>::const_iterator it = getHeaders().begin(); it != getHeaders().end(); ++it) {
		response += it->first + ": " + it->second + "\r\n";
	}
	response += "\r\n" + _body;
	return response;
}

void Response::setHeaders(map<string, string> headers) {
	_headers = headers;
}

void Response::setBody(const string& body) {
	_body = body;
}

void Response::sendResponse(vector<struct pollfd>& pollFds, int i, map<int, Request>& requests)
{
	Request &req = requests[pollFds[i].fd];
	string hello =
    	"HTTP/1.1 200 OK\r\n"
    	"Content-Type: text/plain\r\n"
    	"Content-Length: 17\r\n"
    	"Connection: close\r\n"
    	"\r\n"
    	"Hello from server";


    Response resp(req);

	resp.setStatusCode(req.getStatusCode());
	resp.setBody(req.getBody());
	resp.setHeaders(req.getHeaders());

	string response = resp.getResponse();

	send(pollFds[i].fd, response.c_str(), response.size(), 0);
	cout << "Response sent" << endl;
    // send(pollFds[i].fd, hello.c_str(), hello.size(), 0);
    // cout << "Message sent" << endl;
   
    requests.erase(pollFds[i].fd);
    close(pollFds[i].fd);
    pollFds.erase(pollFds.begin() + i);
    (void)req;
}

void Response::setStatusCode(int code)
{
    _statusCode = code;
    _statusMessage = getStatusMessage(code);
}

string Response::getStatusMessage(int code) const
{
    switch (code) {
        // talvez tenha em excesso
        case 200:
            return ("OK");
        case 400:
            return ("Bad request");
        case 403:
            return ("Forbidden");
        case 404:
            return ("Not found");
        case 405:
            return ("Method not allowed");
        case 500:
            return ("Internal server error");
        case 502:
            return ("Bad gateway");
        default:
            return ("Unknown status");
    }
}