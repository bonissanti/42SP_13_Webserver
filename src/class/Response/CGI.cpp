#include "Response.hpp"

void Response::freeEnviron(char **envp)
{
    for (int i = 0; envp[i]; i++)
        delete[] envp[i];
    delete[] envp;
}

bool Response::checkFile(const string &file)
{
    ifstream f(file.c_str());
    return (f.good());
}

char **Response::configEnviron(Request &req)
{
    char **envp = new char *[6];
    string contentType = "CONTENT_TYPE=" + _contentType;
    string contentLength = "CONTENT_LENGTH=" + _contentLength;
    string scriptName = "SCRIPT_NAME=" + req.getServer().getRoute()[0].getCgi();
    string pathInfo = "PATH_INFO=" + req._uri;
    string serverName = "SERVER_NAME=" + req.getServer().getServerName();
    string serverPort = "SERVER_PORT=" + Utils::itostr(req.getServer().getListen());
    string protocol = "SERVER_PROTOCOL=" + req._version;

    envp[0] = strdup(_contentType.c_str());
    envp[1] = strdup(_contentLength.c_str());
    envp[2] = strdup(pathInfo.c_str());
    envp[3] = strdup(serverName.c_str());
    envp[4] = strdup(serverPort.c_str());
    envp[5] = strdup(protocol.c_str());
    envp[6] = NULL;
    return (envp);
}

string Response::executeCGI(Request &req, const string& filePath)
{
    int pid;
    int fd[2];
    int status;
    string result;

    if (!checkFile(filePath)) {
        _statusCode = NOT_FOUND;
        return ("");
    }

    size_t found = filePath.find('.');
    if (filePath.substr(found) == ".py")
        _executor = "/usr/bin/python3";
    else
        _executor = "/usr/bin/php";

    pipe(fd);
    pid = fork();
    if (pid < 0) {
        throw Server::exception(RED "Error: Fork failed" RESET);
    }
    if (pid == 0) {
        char **envp = configEnviron(req);
        char *args[] = {const_cast<char *>(_executor.c_str()), const_cast<char *>(filePath.c_str()), NULL};

        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        if (execve(_executor.c_str(), args, envp) == -1) {
            freeEnviron(envp);
            cerr << RED << "Error: execve failed" << RESET << endl;
            exit(1);
        }
        freeEnviron(envp);
        close(fd[1]);
    }
    else {
        close(fd[1]);
        result = readCGI(fd[0]);
        close(fd[0]);
        waitpid(pid, &status, 0);
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
        	cerr << RED << "Error: child process failed" << RESET << endl;
        }
    }
    return (result);
}

string Response::readCGI(int fd_in)
{
    ssize_t bytesRead;
    char buffer[65535];
    string result;

    while ((bytesRead = read(fd_in, &buffer, sizeof(buffer))) > 0) {
        buffer[bytesRead] = '\0';
        result += buffer;
    }
    return (result);
}