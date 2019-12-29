#include <winsock2.h>
#include <stdio.h>
#include <windows.h>
#pragma warning (disable: 4786) 
#include<map>
#include<string>
#include<ctime>
using namespace std;
bool ParseRequest(string szRequest, string &szResponse, bool &bKeepAlive);

BOOL writeToSocket(SOCKET s,const char *buff, int count)
{
	int sendCount,currentPosition=0;
	while( count>0 && (sendCount=send(s ,buff+currentPosition,count,0))!=SOCKET_ERROR)
	{
		count-=sendCount;
		currentPosition+=sendCount;
	}
	if(sendCount==SOCKET_ERROR)return FALSE;
	return TRUE;
}
BOOL writeToSocket(SOCKET s,char *pszBuff)
{
	return writeToSocket(s,pszBuff,strlen(pszBuff));
}

DWORD WINAPI HttpProcess(
	__in  LPVOID lpParameter
	)
{
	SOCKET clientSocket=(SOCKET) lpParameter;
	//接收缓冲区的大小是20480个字符
	char recvBuf[20480];
	bool bkeepalive=true;
	// Loop as long as the connection is opened.
	while( bkeepalive )
	{
		int count =recv(clientSocket ,recvBuf,20480,0);
		if(count==0)break;//被对方关闭
		if(count==SOCKET_ERROR)break;//错误count<0
		//if(count>0)
		recvBuf[count]=0;

		string szRequest(recvBuf);
		string szResponse;
		bkeepalive=false;
		ParseRequest(szRequest,szResponse,bkeepalive);
		writeToSocket(clientSocket,szResponse.c_str(),(int)szResponse.length());
	}


	//结束连接
	closesocket(clientSocket);
	return 0;
}

typedef map<string, string>				MIMETYPES;
MIMETYPES  MimeTypes;
//string m_HomeDir;
string m_DefIndex;
#define ERROR404 "/404.html"
#define ERROR501 "/501.html"
#define SERVERNAME "localhost"


void Init()
{
	//初始化 设置虚拟目录
	//m_HomeDir		= ".";
	m_DefIndex		= "/index.html";
	//printf("%d\n", m_HomeDir.size());
	// m_HomeDir.substr(m_HomeDir.size() - 1, 1)
	//if(m_HomeDir.substr(m_HomeDir.size() - 1, 1) != "\\")//substr(字符串,截取开始位置,截取长度) //返回截取的字substr('Hello World', 0, 1) //返回结果为 'H'  *从字符串第一个字符开始截取长度为1的字符串substr('Hello World', 1, 1) //返回结果为 'H'  *0和1都是表示截取的开始位置为第一个字符substr('Hello World', 2, 4) //返回结果为 'ello'
	//	m_HomeDir += "\\";	
	//printf("%s",m_HomeDir.c_str());
	//
	// Init MIME Types
	//
	MimeTypes["doc"]	= "application/msword";
	MimeTypes["bin"]	= "application/octet-stream";
	MimeTypes["dll"]	= "application/octet-stream";
	MimeTypes["exe"]	= "application/octet-stream";
	MimeTypes["pdf"]	= "application/pdf";
	MimeTypes["p7c"]	= "application/pkcs7-mime";
	MimeTypes["ai"]		= "application/postscript";
	MimeTypes["eps"]	= "application/postscript";
	MimeTypes["ps"]		= "application/postscript";
	MimeTypes["rtf"]	= "application/rtf";
	MimeTypes["fdf"]	= "application/vnd.fdf";
	MimeTypes["arj"]	= "application/x-arj";
	MimeTypes["rar"]	= "application/x-compressed";
	MimeTypes["gz"]		= "application/x-gzip";
	MimeTypes["class"]	= "application/x-java-class";
	MimeTypes["js"]		= "application/x-javascript";
	MimeTypes["lzh"]	= "application/x-lzh";
	MimeTypes["lnk"]	= "application/x-ms-shortcut";
	MimeTypes["tar"]	= "application/x-tar";
	MimeTypes["hlp"]	= "application/x-winhelp";
	MimeTypes["cert"]	= "application/x-x509-ca-cert";
	MimeTypes["zip"]	= "application/zip";
	MimeTypes["cab"]	= "application/x-compressed";
	MimeTypes["arj"]	= "application/x-compressed";
	MimeTypes["aif"]	= "audio/aiff";
	MimeTypes["aifc"]	= "audio/aiff";
	MimeTypes["aiff"]	= "audio/aiff";
	MimeTypes["au"]		= "audio/basic";
	MimeTypes["snd"]	= "audio/basic";
	MimeTypes["mid"]	= "audio/midi";
	MimeTypes["rmi"]	= "audio/midi";
	MimeTypes["mp3"]	= "audio/mpeg";
	MimeTypes["vox"]	= "audio/voxware";
	MimeTypes["wav"]	= "audio/wav";
	MimeTypes["ra"]		= "audio/x-pn-realaudio";
	MimeTypes["ram"]	= "audio/x-pn-realaudio";
	MimeTypes["bmp"]	= "image/bmp";
	MimeTypes["gif"]	= "image/gif";
	MimeTypes["jpeg"]	= "image/jpeg";
	MimeTypes["jpg"]	= "image/jpeg";
	MimeTypes["tif"]	= "image/tiff";
	MimeTypes["tiff"]	= "image/tiff";
	MimeTypes["xbm"]	= "image/xbm";
	MimeTypes["wrl"]	= "model/vrml";
	MimeTypes["htm"]	= "text/html";
	MimeTypes["html"]	= "text/html";
	MimeTypes["c"]		= "text/plain";
	MimeTypes["cpp"]	= "text/plain";
	MimeTypes["def"]	= "text/plain";
	MimeTypes["h"]		= "text/plain";
	MimeTypes["txt"]	= "text/plain";
	MimeTypes["rtx"]	= "text/richtext";
	MimeTypes["rtf"]	= "text/richtext";
	MimeTypes["java"]	= "text/x-java-source";
	MimeTypes["css"]	= "text/css";
	MimeTypes["mpeg"]	= "video/mpeg";
	MimeTypes["mpg"]	= "video/mpeg";
	MimeTypes["mpe"]	= "video/mpeg";
	MimeTypes["avi"]	= "video/msvideo";
	MimeTypes["mov"]	= "video/quicktime";
	MimeTypes["qt"]		= "video/quicktime";
	MimeTypes["shtml"]	= "wwwserver/html-ssi";
	MimeTypes["asa"]	= "wwwserver/isapi";
	MimeTypes["asp"]	= "wwwserver/isapi";
	MimeTypes["cfm"]	= "wwwserver/isapi";
	MimeTypes["dbm"]	= "wwwserver/isapi";
	MimeTypes["isa"]	= "wwwserver/isapi";
	MimeTypes["plx"]	= "wwwserver/isapi";
	MimeTypes["url"]	= "wwwserver/isapi";
	MimeTypes["cgi"]	= "wwwserver/isapi";
	MimeTypes["php"]	= "wwwserver/isapi";
	MimeTypes["wcgi"]	= "wwwserver/isapi";
}


//解析数据并且将文件中的内容读出来
//szRequest 用户发过来的数据
//szResponse 用来保存要发送回给客户的数据
bool ParseRequest(string szRequest, string &szResponse, bool &bKeepAlive)
{
	//
	// Simple Parsing of Request
	//
	string szMethod;
	string szFileName;
	string szFileExt;
	string szStatusCode("200 OK");
	string szContentType("text/html");
	string szConnectionType("close");
	string szNotFoundMessage;
	string szDateTime;
	char pResponseHeader[2048];
	fpos_t lengthActual = 0, length = 0;
	char *pBuf = NULL;
	int n;

	//
	// Check Method
	//
	n = szRequest.find(" ", 0);
	if(n != string::npos)
	{
		szMethod = szRequest.substr(0, n);
		if(szMethod == "GET")
		{
			//
			// Get file name
			// 
			int n1 = szRequest.find(" ", n + 1);
			if(n != string::npos)
			{
				szFileName = szRequest.substr(n + 1, n1 - n - 1);
				if(szFileName == "/")
				{
					szFileName = m_DefIndex;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			szStatusCode = "501 Not Implemented";
			szFileName = ERROR501;
		}
	}
	else
	{
		return false;
	}
	
	//
	// Determine Connection type
	//
	n = szRequest.find("\nConnection: Keep-Alive", 0);
	if(n != string::npos)
		bKeepAlive = true;

	//
	// Figure out content type
	//
	int nPointPos = szFileName.rfind(".");
	if(nPointPos != string::npos)
	{
		szFileExt = szFileName.substr(nPointPos + 1, szFileName.size());//_Count或_Count+_Off超出了源字符串的长度，则子字符串将延续到源字符串的结尾。
		strlwr((char*)szFileExt.c_str());//strlwr()用于将字符串中的字符转换为小写,c_str()函数返回一个指向正规C字符串的指针常量, 内容与本string串相同。
        //这是为了与c语言兼容，在c语言中没有string类型，故必须通过string类对象的成员函数c_str()把string 对象转换成c中的字符串样式。
		MIMETYPES::iterator it;
		it = MimeTypes.find(szFileExt);//MAP遍历寻找
		if(it != MimeTypes.end())
			szContentType = (*it).second;
	}

	//
	// Obtain current GMT date/time
	//
	char szDT[128];
	struct tm *newtime;
	time_t ltime;

	time(&ltime);//time_t time(time_t *seconds) 返回自纪元 Epoch（1970-01-01 00:00:00 UTC）起经过的时间，以秒为单位。如果 seconds 不为空，则返回值也存储在变量 seconds 中。
	newtime = gmtime(&ltime); //struct tm *gmtime(const time_t *timep);gmtime()将参数timep 所指的time_t 结构中的信息转换成真实世界所使用的时间日期表示方法，然后将结果由结构tm 返回
	strftime(szDT, 128,
		"%a, %d %b %Y %H:%M:%S GMT", newtime);//size_t strftime(char* strDest,size_t maxsize,const char* format,const  struct tm* timeptr);参数说明：我们可以根据format指向字符串中格式命令把timeptr中保存的时间信息放在strDest指向的字符串中，最多向strDest中存放maxsize个字符。该函数返回向strDest指向的字符串中放置的字符数。
	int printflag = 1;
	//
	// Read the file
	//
	FILE *f;
	//printf("\n%s", (m_HomeDir + szFileName).c_str());
	printf("\n%s", (szFileName.substr(1, szFileName.size())).c_str());
	//f = fopen((m_HomeDir +szFileName).c_str(), "r+b");
	f = fopen((szFileName.substr(1,szFileName.size())).c_str(), "r+b");
	if(f != NULL)				
	{
		// Retrive file size
		fseek(f, 0, SEEK_END);//int fseek(FILE *stream, long offset, int fromwhere);函数设置文件指针stream的位置。fseek函数和lseek函数类似，但lseek返回的是一个off_t数值，而fseek返回的是一个整型。
		fgetpos(f, &lengthActual);//fgetpos是一个函数名，功能是依据当前文件的句柄，获取当前访问指针位置信息。
		fseek(f, 0, SEEK_SET);

		pBuf = new char[lengthActual + 1];

		length = fread(pBuf, 1, lengthActual, f);
		fclose(f);

		//
		// Make Response
		//
		sprintf(pResponseHeader, "HTTP/1.0 %s\r\nDate: %s\r\nServer: %s\r\nAccept-Ranges: bytes\r\nContent-Length: %d\r\nConnection: %s\r\nContent-Type: %s\r\n\r\n",
			szStatusCode.c_str(), szDT, SERVERNAME, (int)length, bKeepAlive ? "Keep-Alive" : "close", szContentType.c_str());
	}
	else
	{
		//
		// In case of file not found	   
		//
		printflag = 0;
		bKeepAlive = true;
		if(szFileName==ERROR501)
		{
			szNotFoundMessage="<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></head>"
			"<body><h2>2017141223035's Simple Web Server</h2><div>501 - Method Not Implemented</div></body></html>";
			bKeepAlive = false;
		}
		else
		{
		//f = fopen((m_HomeDir + ERROR404).c_str(), "r+b");
			f = fopen((((string)ERROR404).substr(1, szFileName.size())).c_str(), "r+b");
			/*测试printf("\n路径%s\n",ERROR404);*/
		if(f != NULL)				
		{
			// Retrive file size
			fseek(f, 0, SEEK_END);
			fgetpos(f, &lengthActual);
			fseek(f, 0, SEEK_SET);
			pBuf = new char[lengthActual + 1];
			length = fread(pBuf, 1, lengthActual, f);
			fclose(f);
			szNotFoundMessage = string(pBuf, length);
			delete pBuf;
			pBuf = NULL;
		}else szNotFoundMessage="<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></head>"
		"<body><h2>2017141223035's Simple Web Server</h2><div>404 - Not Found</div></body></html>";
		szStatusCode = "404 Resource not found";
		bKeepAlive = false;
		}

		sprintf(pResponseHeader, "HTTP/1.0 %s\r\nContent-Length: %d\r\nContent-Type: text/html\r\nDate: %s\r\nServer: %s\r\n\r\n%s",
			szStatusCode.c_str(), szNotFoundMessage.size(), szDT, SERVERNAME, szNotFoundMessage.c_str());
		//bKeepAlive = false;  
	}

	szResponse = string(pResponseHeader);
	if(printflag)
	printf("\n%s\n", szResponse.c_str());
	if(pBuf)
		szResponse += string(pBuf, length);
	delete pBuf;
	pBuf = NULL;
	

	return false;
}
