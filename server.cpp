#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <chrono>
#include <ctime> 

#include <time.h> 
#include <string>  
#include <fstream>

#define die(e) do { fprintf(stderr, "%s\n", e); exit(EXIT_FAILURE); } while (0);
#pragma warning(disable : 4996)  //for Visual studio ¯\_(ツ)_/¯

//#include "boost/filesystem.hpp"   
//using namespace boost::filesystem;

#define REMOTE_PORT 8080   //i trided any numbers that the same on client.cpp

std::string CONTENT_TYPE = "";
std::string CONTENT_LENGTH = "";
std::string LAST_MODIFIED = "Last modified:     ";
std::string Host = "";

std::string SCRIPT = "";
std::string SERVER_REQUEST = "";

std::string GATEWAY_INTERFACE = "Gatway interface:  CGI / 1.1\n";
std::string REMOTE_ADDR = "";
std::string REMOTEs_PORT;
std::string QUERY_STRING = "";
std::string SERVER_ADDR;
std::string SERVER_NAME = "Server:            Model HTTP Server/0.1 \n";
std::string SERVER_PORT = "Remote port:       8080 \n";;
std::string SERVER_PROTOCOL = "";
std::string SERVER_SOFTWARE = "Server software:   Apache / 1.3.12 (Unix)\n";
std::string SCRIPT_NAME;
std::string SCRIPT_FILENAME;
std::string DOCUMENT_ROOT;
std::string HTTP_USER_AGENT;
std::string HTTP_REFERER = "";

std::string DATE;

std::string file_script;
const std::string currentDateTime() 
{
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
	 
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%a %b %d %X %Y", &tstruct);

    return buf;
}

std::string call_back_http(std::vector <std::string> vec)
{
    std::string stroka = vec[0];
    std::string work_str = "";

    SERVER_PROTOCOL += "\n" + vec[vec.size() - 1] + "          ";
    REMOTE_ADDR += "Addres:            " + vec[0] + "\n";
    work_str = vec[0];
    int error_s = 0;

    int i = stroka.length() - 1;
    while ((stroka[i] != '.') and (i > 0))
    {
        i--;
        if (i < 0) error_s = 1;
    }
    std::ifstream file(work_str);
        work_str = "";

    if (error_s == 0)
    {
        for (i; i < stroka.length(); i++) work_str += stroka[i];

        
        //std::cout << work_str << "\n";

        std::string str_mas = ".jpeg";
        for (int k = 1; k < work_str.length(); k++)
        {
            if (work_str[k] != str_mas[k]) { error_s = 1; break; }
            if (k == work_str.length() - 1)
            {
                CONTENT_TYPE += "Content-type:      image / jpeg\n";
                if (!file) { SERVER_REQUEST += "404 Not Found"; break; }
                else SERVER_REQUEST += "200 OK";
            }
        }
        str_mas = ".html";
        for (int k = 1; k < work_str.length(); k++)
        {
            if (work_str[k] != str_mas[k]) { error_s = 1; break; }
            if (k == work_str.length() - 1) {
                HTTP_REFERER +=  "URL:               " + vec[0];
                CONTENT_TYPE += "Content-type:      text / html\n";
                if (!file) { SERVER_REQUEST += "404 Not Found"; break; }
                else SERVER_REQUEST += "200 OK";
            }
        }
        str_mas = ".com";
        for (int k = 1; k < work_str.length(); k++)
        {
            if (work_str[k] != str_mas[k]) { error_s = 1; break; }
            if (k == work_str.length() - 1)
            {
                HTTP_REFERER += "URL: " + vec[0] + "\n";
				SERVER_REQUEST += "200 OK";
			
				CONTENT_TYPE += "Content-type:      text / html\n";
			}
        }
		
        bool rf = 0;
        str_mas = ".txt";
        for (int k = 1; k < work_str.length(); k++)
        {
            if (work_str[k] != str_mas[k]) { error_s = 1; break; }
            if (k == work_str.length() - 1)  CONTENT_TYPE += "Content-type:      text / plain\n";
            if (!file) { SERVER_REQUEST += "404 Not Found"; break; }
            else if (k == work_str.length() - 1) { SERVER_REQUEST += " 200 OK"; rf = 1; }
        }

        if (rf == 1)
        {
             
            file_script = std::string ((std::istreambuf_iterator<char>(file)),
                (std::istreambuf_iterator<char>()));
        };

        file.close();
        
    }

    if ((SERVER_REQUEST.length() == 0)) SERVER_REQUEST += "500 Internal Server Error";

    if (vec.size() > 2) QUERY_STRING += "Parameters:        ";
    for (int i = 1; i < vec.size() - 1; i++)
    {
        QUERY_STRING += vec[i] + " = " + vec[i + 1] + "\n";
        i++;
        if (i < vec.size() - 2) QUERY_STRING += "                   ";
    }

    //std::cout << CONTENT_TYPE << body_html << http_send << QUERY_STRING << strlen(buffer) << std::endl;

    SERVER_REQUEST += "\n";
 
    time_t ttime = time(0);
    DATE = "Date:              ";
    DATE += currentDateTime() + '\n';
	
	struct stat attr;
    stat("client.cpp", &attr);
	
	LAST_MODIFIED += ctime(&attr.st_mtime);
	
    return (SERVER_PROTOCOL + SERVER_REQUEST + DATE + LAST_MODIFIED + SERVER_NAME + REMOTE_ADDR + CONTENT_TYPE + CONTENT_LENGTH + QUERY_STRING + GATEWAY_INTERFACE + REMOTEs_PORT + SERVER_ADDR + SERVER_PORT
        + SERVER_SOFTWARE + SCRIPT_NAME + SCRIPT_FILENAME + DOCUMENT_ROOT + HTTP_USER_AGENT + HTTP_REFERER + "\nScript: \n\n" + file_script + '\n');
}


//------------------------PARSING HHTP REQUEST ONLY! 
//------------------------and get <html> script body
//------------------------without any get/hed params in request

std::vector <std::string> parse__http(const char* buffer)
{
    std::vector <std::string> result;
    std::string lopstr = "";
    std::string lopstr_adres = "";
    int len;
    int chet = 0;
    int indicator = 0;
    int m_lenght = 0;

    CONTENT_LENGTH += "Content-Length:    " + std::to_string(strlen(buffer)) + '\n';
    for (int k = 1; k < strlen(buffer); k++)
    {
        if (buffer[k] == '\n') { len = k - 1; break; }
    }

//-----------------------------------------Parse Get HTML request
    std::string str_mas = "GET";
    for (int k = 1; k < str_mas.length(); k++)
    {
        if (buffer[k] != str_mas[k]) { m_lenght = 0;  indicator = 5; break;}
        if (k == str_mas.length() - 1)  m_lenght = 6;
    }
//-----------------------------------------Parse Head HTML request	
    str_mas = "HEAD";
    for (int k = 1; k < str_mas.length(); k++)
    {
        if (buffer[k] != str_mas[k]) 
		{ 
		if (indicator == 0 ) break; 
		m_lenght = 0;  indicator = 5; break;}
        if (k == str_mas.length() - 1) { m_lenght = 7; indicator = 0;}
    }
//-----------------------------------------Parse body params of HTML request
 
    for (int i = 0; i < strlen(buffer) - 1 - m_lenght; i++)
    {
        if (buffer[i + m_lenght - 1] == '\n') { chet = i + m_lenght;  break; }
        if (buffer[i + m_lenght - 1] == ' ') { chet = i + m_lenght; break; }
        if (buffer[i + m_lenght - 1] == '?') { chet = i + m_lenght; indicator = 1; break; }
        if ((indicator == 5) and (buffer[i] != '?')) lopstr += buffer[i]; 
		else { if (buffer[i] != '?') lopstr += buffer[i + m_lenght - 1]; else break;}
        //std::cout << buffer[i + m_lenght - 1] << "\n";
        chet++;
    }

	//std::cout << lopstr;

    if (indicator == 0) //no params in html request
    {
        result.push_back(lopstr);
    }
    else
    {
        if ((indicator == 1) or (indicator == 5)) //some params in html request
        {
            result.push_back(lopstr);
            lopstr = "";

            int i = chet;
			if (buffer[i] == '?') i++;
            while (i < strlen(buffer))
            {
                if ((buffer[i] == ' ') or (buffer[i] == '\n')) {
                    chet = i + 1;
                    result.push_back(lopstr);
                    break;
                }
                if ((buffer[i] == '=') or (buffer[i] == '&')) { result.push_back(lopstr); lopstr = ""; }
                else lopstr += buffer[i];
                i++;
            }
            //std::cout << "\n??\n" << std::endl;
        }
        else //no GET / HEAD method
        {
            result.push_back(lopstr);
            //system(lopstr.c_str());
        }
    }
    lopstr = "";

    //std::cout << buffer[chet] << std::endl;
	bool replay = 0;
    int i = chet;
	 
    while (i < strlen(buffer)) //read every line before \n or end buffer
    {
		if (buffer[i] == '\n') { chet = i + 1;  replay = 1; break;}
        lopstr += buffer[i];
        i++;
    }
	 
    result.push_back(lopstr);


    if (replay == 1)
	{
		int i = chet - 1;
		//-------------skip lines before <HTML SCRIPT> (if we have them)
		while (((i + 1 ) < strlen(buffer))) 
		{
			if ((buffer[i + 1] == '\n') and (buffer[i] == '\n')) break;
			i++;
		}
		
		i+=2;
		//-------------record <HTML SCRIPT> (if we have them)
		if ((buffer[i+1] != '\n') and (buffer[i] != '\n')) 
		{
			for (i; i < strlen(buffer); i++) SCRIPT += buffer[i];
		}
	}
    
	//-----------------RETURN VECTOR RESULT
	//-----------------vector [0] - link adress
	//-----------------vector [i[ - html req params
	//-----------------vector last elem - HTTP PROTOCOL
	
    return result;  
}


//------------------------SERVER MAIN WORK@@@
int main(int argc, char const* argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = { 0 };

    
     
     

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
        &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(REMOTE_PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr*)&address,
        sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    while (1)
    {

        std::string request_js = "";

        if ((new_socket = accept(server_fd, (struct sockaddr*)&address,
            (socklen_t*)&addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        if (fork()) continue;

        valread = read(new_socket, buffer, 1024);
        std::cout << "SERVER GET \n----------\n" << buffer << std::endl;

        std::vector <std::string>  result = parse__http(buffer);
        std::string answer = call_back_http(result);
        //std::cout << SCRIPT.c_str() << "------" << std::endl;

        //------------------------------------------If we have script in request
        //------------------------------------------just do it 
        //------------------------------------------but later

        //std::cout << file_script;

        if (file_script.length())
        {
            int link[2];
            pid_t pid;
            char foo[4096];

            std::cout << "SCRIPT: \n\n" << file_script << std::endl;

            if (pipe(link) == -1)
                die("pipe");

            if ((pid = fork()) == -1)
                die("fork");

            if ((pid) < 0)
            {
                std::cout << "ERROR RUN fork\n";

            }
            else
            {
                if (pid == 0)
                {
                    
                    std::cout << "PROSSES: ----> child\n";
                    dup2(link[1], 1);
                    //dup2(link[1], 2);

                    close(link[0]);
                    close(link[1]);



                    const char* path = "./js";
                    //std::cout << SCRIPT.c_str() << std::endl;
                    ::execl(path, (std::to_string(file_script.length())).c_str(), file_script.c_str(), NULL);
                    exit(0);
                }
                else {

                    close(link[1]);
                    while (1)
                    {
                        int nbytes = read(link[0], foo, sizeof(foo) - 1);
                        foo[nbytes] = 0;
                        if (nbytes == 0) break;
                        request_js += foo;

                    }
                    //std::cout << sizeof(foo) << " res = " << nbytes << " output: " << foo << std::endl;
                    //std::cout << "\n" << request_js;
                    pid_t cpid = wait(NULL);
                    close(link[0]);
                    std::cout << "PROSSES: ----> parent\n" << std::endl;
                }


            }
        }
        std::cout << "RESULT:   " << request_js;
        answer += "\nRESULT:            ";
        answer += request_js;
        send(new_socket, answer.c_str(), answer.length(), 0);
        exit (0);
    }
    return 0;
}