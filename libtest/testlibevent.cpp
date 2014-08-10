#include "pch.h"
#include "testlibevent.h"
#include <event.h>
#include <evdns.h>
#include <event2/util.h>
#include <winsock2.h>
#include <thread>
#pragma comment(lib,"ws2_32.lib")

int g_fdlisten=0;
int create_listen_socket(int port);

void readcb(bufferevent* bev,void* args)
{
	evbuffer* buffer_input=bufferevent_get_input(bev);
	int len=evbuffer_get_length(buffer_input);
	char* szData=new char[len];
	memcpy(szData,evbuffer_pullup(buffer_input,len),len);

	evbuffer* buffer_output=bufferevent_get_output(bev);
	evbuffer_add(buffer_output,szData,len);

	evbuffer_drain(buffer_input,len);
}

void bevcb(bufferevent* bev,short flag,void* args)
{
}

void event_cb(int fd,short event_type,void* arg)
{
	event_base* base=(event_base*)arg;
	if(event_type & EV_READ)
	{
		sockaddr_in addr_client;
		int len_addr_client=sizeof(sockaddr_in);
		memset(&addr_client,0,len_addr_client);
		int fd_client=accept(fd,(sockaddr*)&addr_client,&len_addr_client);
		
		bufferevent* ev=bufferevent_socket_new(base,fd_client,BEV_OPT_CLOSE_ON_FREE);
		bufferevent_setcb(ev,readcb,NULL,bevcb,NULL);
		bufferevent_enable(ev,EV_READ | EV_PERSIST);
		//bufferevent*
	}
}
void cbdns(int errorcode,evutil_addrinfo* addrinfo,void *args)
{
	struct evutil_addrinfo* addrptr=addrinfo;
	while(addrptr)
	{
		char buf[128]={0};
		if(addrinfo->ai_family==AF_INET)
		{
			sockaddr_in* sockaddr=(sockaddr_in*)addrptr->ai_addr;
			evutil_inet_ntop(AF_INET,&sockaddr->sin_addr,buf,128);
		}
		addrptr=addrptr->ai_next;
	}
}
void duservice()
{
	int result=0;
	
	g_fdlisten=create_listen_socket(5555);
	
	event_base* base=event_init();
	event* ev_listen=event_new(base,g_fdlisten,EV_READ | EV_PERSIST,event_cb,base);
	if(!ev_listen)
		goto fini;
	event_add(ev_listen,NULL);

	evdns_base* dnsbase=evdns_base_new(base,1);
	evutil_addrinfo hints;
	evdns_getaddrinfo_request* dns_requ;
	memset(&hints,0,sizeof(evutil_addrinfo));
	hints.ai_family=AF_UNSPEC;
	hints.ai_flags=EVUTIL_AI_CANONNAME;
	hints.ai_socktype=SOCK_STREAM;
	hints.ai_protocol=IPPROTO_TCP;
	
	evdns_getaddrinfo(dnsbase,"www.baidu.com",NULL,&hints,cbdns,NULL);	
	//event_get_ba
	//event_base_dispatch(base);
fini:
	return;
}
int start_service(int port)
{	
	WSADATA data;
	WSAStartup(MAKEWORD(2,2),&data);

	std::thread thr(duservice);
	thr.detach();
	return 1;
}


int create_listen_socket(int port)
{
	sockaddr_in addr_local;
	int fd_listen=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);	
	memset(&addr_local,0,sizeof(sockaddr_in));
	addr_local.sin_port=htons(port);
	addr_local.sin_family=AF_INET;
	addr_local.sin_addr.s_addr=ADDR_ANY;
	bind(fd_listen,(sockaddr*)&addr_local,sizeof(sockaddr_in));
	listen(fd_listen,0);
	return fd_listen;	
}