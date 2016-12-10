#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int server_sockfd;//服务器端套接字
	int client_sockfd;//客户端套接字
	int len;
	struct sockaddr_in my_addr;   //服务器网络地址结构体
	struct sockaddr_in remote_addr; //客户端网络地址结构体
	int sin_size;
	char buf[BUFSIZ];  //数据传送的缓冲区
	memset(&my_addr,0,sizeof(my_addr)); //数据初始化--清零
	my_addr.sin_family=AF_INET; //设置为IP通信
	my_addr.sin_addr.s_addr=INADDR_ANY;//服务器IP地址--允许连接到所有本地地址上
	my_addr.sin_port=htons(8000); //服务器端口号
	
	/*创建服务器端套接字--IPv4协议，面向连接通信，TCP协议*/
	if((server_sockfd=socket(PF_INET,SOCK_STREAM,0))<0)
	{  
		perror("socket");
		return 1;
	}
	// 设置套接字选项避免地址使用错误  
 	int on=1;  
 	if((setsockopt(server_sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)))<0)  
     	{  
     		perror("setsockopt failed");  
        	exit(EXIT_FAILURE);  
    	}  
        /*将套接字绑定到服务器的网络地址上*/
	if (bind(server_sockfd,(struct sockaddr *)&my_addr,sizeof(struct sockaddr))<0)
	{
		perror("bind");
		return 1;
	}
	
	/*监听连接请求--监听队列长度为5*/
	listen(server_sockfd,5);
	
	sin_size=sizeof(struct sockaddr_in);
	int time=0;	
	while(1){
		time++;
		if (time>2) break;
		/*等待客户端连接请求到达*/
		if((client_sockfd=accept(server_sockfd,(struct sockaddr *)&remote_addr,&sin_size))<0)
		{
			perror("accept");
			return 1;
		}
		printf("accept client %s\n",inet_ntoa(remote_addr.sin_addr));
		len=send(client_sockfd,"Welcome to my server\n",21,0);//发送欢迎信息


	
		/*接收客户端的数据并将其发送给客户端--recv返回接收到的字节数，send返回发送的字节数*/
		while((len=recv(client_sockfd,buf,BUFSIZ,0))>0)
		{
			buf[len]='\0'; //file name
			printf("asking file : %s\n",buf);
		
			FILE * f=NULL;
			len=1;
			f=fopen(buf,"r");
			if (f==NULL)
			{
				sprintf(buf,"0\0");  //if file not exist,return 0
				printf("file not find\n");		
			}
			else
			{
				sprintf(buf,"1\0");	//if file exist,return 1
				int ret=fread(buf+1,1,sizeof(buf),f);
				len+=ret;
				printf("file find\n");
			}
			if(send(client_sockfd,buf,len,0)<0)
			{
				perror("write");
				return 1;
			}
			if (f) fclose(f);
		}
		close(client_sockfd);
	}
	close(server_sockfd);
        return 0;
}
