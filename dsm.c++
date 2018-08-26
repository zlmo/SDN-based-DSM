#include<pthread.h>
#include"iostream"
#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <fcntl.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>
#include <string.h>  
#include <errno.h>  
#include <sys/types.h> 
#include"dsm.h"
#define MCAST_PORT 8888
#define MCAST_ADDR "224.0.0.88"
#define shared_x "dsm_shared_x"
using namespace std;
static pthread_t id_recv;

char buffer[40960000];
pthread_mutex_t mutex;
pthread_mutex_t mutex2;
List q;
List create(){
	List head;
	head = (List)malloc(sizeof(LNode));
	head->next = NULL;
	strcpy(head->name, "");
	return head;
}
List insert(List head, share *q){
	List p;
	p = (List)malloc(sizeof(LNode));
	p->data = 2;
	p->p = q;
	strcpy(p->name, (*q).name);
	p->next = head->next;
	head->next = p;
	return head;
}
struct write_data{
	int x;
	share_int *p;
};
struct recvdata{
    int a;
    List s;
};
int send_read(share *p){
	int s;
	int ttl = 10;
	int loop = 0;

	struct sockaddr_in localaddr;
	struct sockaddr_in fromaddr;
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s<0){
		perror("socket error");
	}
	localaddr.sin_family = AF_INET;
	localaddr.sin_port = htons(MCAST_PORT);
	localaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    int opt = 1;  
    setsockopt( s, SOL_SOCKET,SO_REUSEADDR, (const void *)&opt, sizeof(opt) );
	int ret = bind(s, (struct sockaddr*)&localaddr, sizeof(localaddr));
	if (ret<0){
		perror("bind error");
	}
	setsockopt(s, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl));
	setsockopt(s, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop));

	struct ip_mreq mreq;
	mreq.imr_multiaddr.s_addr = inet_addr(MCAST_ADDR);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	setsockopt(s, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));

	socklen_t len = sizeof(fromaddr);
	memset(buffer, 0, sizeof(buffer));


    int s_read;
	struct sockaddr_in Multi_addr;
	struct sockaddr_in client_addr;
	s_read = socket(AF_INET, SOCK_DGRAM, 0);
	if (s_read<0){
		perror("socket error");
	}
	Multi_addr.sin_family = AF_INET;
	Multi_addr.sin_port = htons(MCAST_PORT);
	Multi_addr.sin_addr.s_addr = inet_addr(MCAST_ADDR);
    setsockopt(s_read, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop));
	memset(buffer, 0, sizeof(buffer));
	share *q;
	q=(share*)malloc(sizeof(share));
	memset(q,0,sizeof(share));
	memcpy(q,p,sizeof(share));
	sendto(s_read, q, sizeof(share), 0, (struct sockaddr*)&Multi_addr, sizeof(Multi_addr));
    
    recvfrom(s, buffer, sizeof(buffer), 0, NULL, 0);
	close(s_read);
    close(s);
	setsockopt(s, IPPROTO_IP, IP_DROP_MEMBERSHIP, &mreq, sizeof(mreq));
	
}
int send_change(share *p,int x){
    int loop=0;
	(*p).change_data = x;
	int s;
	struct sockaddr_in Multi_addr;
	struct sockaddr_in client_addr;
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s<0){
	perror("socket error");
	}
	Multi_addr.sin_family = AF_INET;
	Multi_addr.sin_port = htons(MCAST_PORT);
	Multi_addr.sin_addr.s_addr = inet_addr(MCAST_ADDR);
	setsockopt(s, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop));
	memset(buffer, 0, sizeof(buffer));
	share *q;
	q=(share*)malloc(sizeof(share));
	memcpy(q,p,sizeof(share));
	sendto(s, q, sizeof(share), 0, (struct sockaddr*)&Multi_addr, sizeof(Multi_addr));
	close(s);
}
void *recvfrom(void *r){
    struct recvdata *p;
    p=(recvdata*)r;
    List t;
    int s;
    t=p->s;
    s=p->a;
    recvfrom(s, buffer, sizeof(buffer), 0, NULL, 0);
    share *y;
    y = (share*)malloc(sizeof(share));
    memcpy(y, buffer, sizeof(share));
    while (t != NULL)
    {
        if ((strcmp((*y).name, t->name) == 0) && ((*y).read == 1)){

            share *rr;
            share_int *si;
            share_double *sd;
            share_float *sf;
            share_doubleArray *sdA;
            share_floatArray *sfA;
            share_char *sc;
            share_intArray *siA;
            share_charArray *scA;
            share_memory *sm;			
            rr = t->p;
            si=(*rr).p_int;
            sd=(*rr).p_double;
            sf=(*rr).p_float;
            sc=(*rr).p_char;
            sdA=(*rr).p_doubleArray;
            sfA=(*rr).p_floatArray;
            siA=(*rr).p_intArray;
            scA=(*rr).p_charArray;
            sm=(*rr).p_memory;
            int ask_type=(*y).c_type;
            switch(ask_type){
            	case 1:
            	if(((*rr).write_allow==1)&&((*rr).share_type)==1){
            	    (*rr).i=(*si).getValue();
            	    (*rr).c_type=1;
         
            	    send_read(rr);
            	    break;
            	}
            	    break;
            	case 2:
            	if(((*rr).write_allow==1)&&((*rr).share_type)==3){
            	    
            	    (*rr).c_type=2;
            	    memcpy((*rr).array,(*siA).getPtr(),sizeof((*siA).getPtr()));
            	    send_read(rr);
            	    break;
            	}
            	    break;
            	case 3:
            	if(((*rr).write_allow==1)&&((*rr).share_type)==2){
                    (*rr).s=(*sc).getValue();
                    (*rr).c_type=3;           
                    
                    send_read(rr);            
                    break;
                }
                    break; 
                case 4:            
            	if(((*rr).write_allow==1)&&((*rr).share_type)==4){
           
                    (*rr).c_type=4;           
                    memcpy((*rr).c_array,(*scA).getPtr(),sizeof((*scA).getPtr())); 
                    send_read(rr);            
                    break;
                }
                    break; 
                case 7:
            	if(((*rr).write_allow==1)&&((*rr).share_type)==7){
            	    (*rr).d=(*sd).getValue();
            	    (*rr).c_type=7;
         
            	    send_read(rr);
            	    break;
            	}
            	    break;   
            	case 8:
            	if(((*rr).write_allow==1)&&((*rr).share_type)==8){
            	    (*rr).f=(*sf).getValue();
            	    (*rr).c_type=8;
         
            	    send_read(rr);
            	    break;
            	}
            	    break; 
            	if(((*rr).write_allow==1)&&((*rr).share_type)==9){
            	    
            	    (*rr).c_type=9;
            	    memcpy((*rr).d_array,(*sdA).getPtr(),sizeof((*sdA).getPtr()));
            	    send_read(rr);
            	    break;
            	}
            	    break;
            	if(((*rr).write_allow==1)&&((*rr).share_type)==10){
            	    
            	    (*rr).c_type=10;
            	    memcpy((*rr).f_array,(*sfA).getPtr(),sizeof((*sfA).getPtr()));
            	    send_read(rr);
            	    break;
            	}
            	    break;
            	case 13:
            	if(((*rr).write_allow==1)&&((*rr).share_type)==13){
            	    
            	    (*rr).c_type=13;
            	    memcpy((*rr).share_buff,(*sm).share_buff,sizeof((*sm).share_buff));
            	    send_read(rr);
            	    break;
            	}
            	    break; 
            }
            break;
                        
        }
        else if ((strcmp((*y).name, t->name) == 0) && ((*y).write == 1)){
            share *rr;
            share_int *si;
            share_double *sd;
            share_float *sf;
            share_doubleArray *sdA;
            share_floatArray *sfA;
            share_char *sc;
            share_intArray *siA;
            share_charArray *scA;	
            share_memory *sm;		
            rr = t->p;
            si=(*rr).p_int;
            sd=(*rr).p_double;
            sf=(*rr).p_float;
            sc=(*rr).p_char;
            sdA=(*rr).p_doubleArray;
            sfA=(*rr).p_floatArray;
            siA=(*rr).p_intArray;
            scA=(*rr).p_charArray;
            sm=(*rr).p_memory;
            int ask_type=(*y).c_type;
           
        	switch(ask_type){
        		case 1:// int
        		if(((*rr).write_allow==1)&&((*rr).share_type)==1){
                    int wd;
                    wd=(*y).write_data;
            	    (*si).writex(wd);
            	    (*rr).i=wd;
            	    (*rr).change=1;
            	    (*rr).c_type=1;
            	    send_change(rr,wd);
            	    (*rr).change=0;
            	    (*rr).c_type=0;           
                    break;
                }else{break;}
                case 2://intArray
                if(((*rr).write_allow==1)&&((*rr).share_type)==3){
                    int size;
                    size=(*y).array_cpy_size;
        		    memcpy((*siA).getPtr(),(*y).array,size*4);
        		    (*rr).array_cpy_size=(*y).array_cpy_size;
        		    memcpy((*rr).array,(*y).array,size*4);
        		    (*rr).change=1;
        		    (*rr).c_type=2;
        		    send_change(rr,0);
        		    (*rr).change=0;
        		    (*rr).c_type=0;
        		    break;
        		}else{break;}
        		case 3:// char
        		if(((*rr).write_allow==1)&&((*rr).share_type)==2){
        		    char cd;
                    cd=(*y).s;
            	    (*sc).writex(cd);            	 
            	    (*rr).change=1;
            	    (*rr).c_type=3;
            	    (*rr).s=cd;
            	    send_change(rr,0);
            	    (*rr).change=0;
            	    (*rr).c_type=0;  
        		    break; 
        		}else{break;}
        		case 4:// charArray
        		if(((*rr).write_allow==1)&&((*rr).share_type)==4){
        		    int csize;
                    csize=(*y).array_cpy_size;
        		    memcpy((*scA).getPtr(),(*y).c_array,csize);
        		    (*rr).array_cpy_size=(*y).array_cpy_size;
        		    memcpy((*rr).c_array,(*y).c_array,csize);
        		    (*rr).change=1;
        		    (*rr).c_type=4;
        		    send_change(rr,0);
        		    (*rr).change=0;
        		    (*rr).c_type=0;
        		    break;
        		}else{break;}
        		case 5://int ()
        		if(((*rr).write_allow==1)&&((*rr).share_type)==3){
                    int aid;
                    int adata;
                    aid=(*y).array_id;
                    adata=(*y).i;
        		    (*siA).wtArray(aid,adata);
        		    (*rr).array[aid]=adata;
        		    (*rr).array_id=aid;
        		    (*rr).change=1;
        		    (*rr).c_type=5;
        		    send_change(rr,0);
        		    (*rr).change=0;
        		    (*rr).c_type=0;
        		    break;
        		}else{break;}
        		case 6: //  char()
        		if(((*rr).write_allow==1)&&((*rr).share_type)==4){
                    int caid;
                    char cdata;
                    caid=(*y).array_id;
                    cdata=(*y).s;
        		    (*scA).wtArray(caid,cdata);
        		    (*rr).c_array[caid]=cdata;
        		    (*rr).array_id=caid;
        		    (*rr).change=1;
        		    (*rr).c_type=6;
        		    send_change(rr,0);
        		    (*rr).change=0;
        		    (*rr).c_type=0;
        		}else{break;}
        		case 7:// double
        		if(((*rr).write_allow==1)&&((*rr).share_type)==7){
                    double dd;
                    dd=(*y).d;
            	    (*sd).writex(dd);
            	    (*rr).d=dd;
            	    (*rr).change=1;
            	    (*rr).c_type=7;
            	    send_change(rr,0);
            	    (*rr).change=0;
            	    (*rr).c_type=0;           
                    break;
                }else{break;}
                case 8:// float
        		if(((*rr).write_allow==1)&&((*rr).share_type)==8){
                    float fd;
                    fd=(*y).f;
            	    (*sf).writex(fd);
            	    (*rr).f=fd;
            	    (*rr).change=1;
            	    (*rr).c_type=8;
            	    send_change(rr,0);
            	    (*rr).change=0;
            	    (*rr).c_type=0;           
                    break;
                }else{break;}
                case 9://doubleArray
                if(((*rr).write_allow==1)&&((*rr).share_type)==9){
                    int size;
                    size=(*y).array_cpy_size;
        		    memcpy((*sdA).getPtr(),(*y).d_array,size*8);
        		    (*rr).array_cpy_size=(*y).array_cpy_size;
        		    memcpy((*rr).d_array,(*y).d_array,size*8);
        		    (*rr).change=1;
        		    (*rr).c_type=9;
        		    send_change(rr,0);
        		    (*rr).change=0;
        		    (*rr).c_type=0;
        		    break;
        		}else{break;}
        		case 10://floatArray
                if(((*rr).write_allow==1)&&((*rr).share_type)==10){
                    int size;
                    size=(*y).array_cpy_size;
        		    memcpy((*sfA).getPtr(),(*y).f_array,size*4);
        		    (*rr).array_cpy_size=(*y).array_cpy_size;
        		    memcpy((*rr).f_array,(*y).f_array,size*4);
        		    (*rr).change=1;
        		    (*rr).c_type=10;
        		    send_change(rr,0);
        		    (*rr).change=0;
        		    (*rr).c_type=0;
        		    break;
        		}else{break;}
        		case 11://double ()
        		if(((*rr).write_allow==1)&&((*rr).share_type)==11){
                    int daid;
                    double dadata;
                    daid=(*y).array_id;
                    dadata=(*y).d;
        		    (*sdA).wtArray(daid,dadata);
        		    (*rr).d_array[daid]=dadata;
        		    (*rr).array_id=daid;
        		    (*rr).change=1;
        		    (*rr).c_type=11;
        		    send_change(rr,0);
        		    (*rr).change=0;
        		    (*rr).c_type=0;
        		    break;
        		}else{break;}
        		case 12://float ()
        		if(((*rr).write_allow==1)&&((*rr).share_type)==12){
                    int faid;
                    float fadata;
                    faid=(*y).array_id;
                    fadata=(*y).f;
        		    (*sfA).wtArray(faid,fadata);
        		    (*rr).d_array[faid]=fadata;
        		    (*rr).array_id=faid;
        		    (*rr).change=1;
        		    (*rr).c_type=12;
        		    send_change(rr,0);
        		    (*rr).change=0;
        		    (*rr).c_type=0;
        		    break;
        		}else{break;}
        		case 13://memory
        		if(((*rr).write_allow==1)&&((*rr).share_type)==13){
        		    memcpy((*sm).share_buff,(*y).share_buff,sizeof((*y).share_buff));
        		    memcpy((*rr).share_buff,(*y).share_buff,sizeof((*y).share_buff));
        		    (*rr).change=1;
        		    (*rr).c_type=13;
        		    send_change(rr,0);
        		    (*rr).change=0;
        		    (*rr).c_type=0;
        		    break;
        		}else{break;}

        	    }
        	    break;
            
		}
		else if ((strcmp((*y).name, t->name) == 0) && ((*y).change == 1))
		{
			share *rr;
            share_int *si;
            share_double *sd;
            share_float *sf;
            share_doubleArray *sdA;
            share_floatArray *sfA;
            share_char *sc;
            share_intArray *siA;
            share_charArray *scA;
            share_memory *sm;			
            rr = t->p;
            si=(*rr).p_int;
            sd=(*rr).p_double;
            sf=(*rr).p_float;
            sc=(*rr).p_char;
            sdA=(*rr).p_doubleArray;
            sfA=(*rr).p_floatArray;
            siA=(*rr).p_intArray;
            scA=(*rr).p_charArray;
            sm=(*rr).p_memory;

            int ask_type=(*y).c_type;
            int x;
            x=(*y).array_cpy_size;
            switch(ask_type){
            	case 1:
				    int xd;
				    xd=(*y).change_data;
				    (*si).writex(xd);
				    break;
				case 2:	
				    memcpy((*siA).getPtr(),(*y).array,x*4);
				    break;
				case 3:
				    char cd;
				    cd=(*y).s;
				    (*sc).writex(cd);
				    break;
				case 4:
				    memcpy((*scA).getPtr(),(*y).c_array,x);				
				    break;	
				case 5:
				    int aid;
				    int adata;
				    aid=(*y).array_id;
				    adata=(*y).array[aid];
				    (*siA).wtArray(aid,adata);
				    break;
				case 6:
				    int caid;
				    char cdata;
				    caid=(*y).array_id;
				    cdata=(*y).c_array[caid];
				    (*scA).wtArray(caid,cdata);
				    break;
				case 7:
				    double dd;
				    dd=(*y).d;
				    (*sd).writex(dd);
				    break;
				case 8:
				    float fd;
				    fd=(*y).f;
				    (*sf).writex(fd);
				    break;
				case 9:	
				    memcpy((*sdA).getPtr(),(*y).d_array,x*8);
				    break;
				case 10:	
				    memcpy((*sfA).getPtr(),(*y).f_array,x*4);
				    break;
				case 11:
				    int daid;
				    double dadata;
				    daid=(*y).array_id;
				    dadata=(*y).d_array[daid];
				    (*sdA).wtArray(daid,dadata);
				    break;
				case 12:
				    int faid;
				    float fadata;
				    faid=(*y).array_id;
				    fadata=(*y).f_array[faid];
				    (*sfA).wtArray(faid,fadata);
				    break;
				case 13:	
				    memcpy((*sm).share_buff,(*y).share_buff,sizeof((*y).share_buff));
				    break;
			}
			break;
		}
		
		
		else
		{ t = t->next; 
		} 
	}
    free(y);
}
void *pshare_recv(void *rt){    
    List t = (List)rt;
	int s;
	int ttl = 10;
	int loop = 0;

	struct sockaddr_in localaddr;
	struct sockaddr_in fromaddr;
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s<0){
		perror("socket error");

	}
	localaddr.sin_family = AF_INET;
	localaddr.sin_port = htons(MCAST_PORT);
	localaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    int opt = 1;  
    setsockopt( s, SOL_SOCKET,SO_REUSEADDR, (const void *)&opt, sizeof(opt) );
	int ret = bind(s, (struct sockaddr*)&localaddr, sizeof(localaddr));
	if (ret<0){
		perror("bind error");

	}
	setsockopt(s, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl));
	setsockopt(s, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop));

	struct ip_mreq mreq;
	mreq.imr_multiaddr.s_addr = inet_addr(MCAST_ADDR);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	setsockopt(s, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));

	socklen_t len = sizeof(fromaddr);
	memset(buffer, 0, sizeof(buffer));
    struct recvdata *k;
    k=(recvdata*)malloc(sizeof(recvdata));
    k->a=s;
    k->s=t;
	while(1){
	int ret1;
    pthread_t id1;
    pthread_mutex_init(&mutex2,NULL);
    pthread_mutex_lock(&mutex2);
    pthread_create(&id1, NULL, recvfrom, k);
    pthread_join(id1,NULL);
    pthread_mutex_unlock(&mutex2);
    }	
}
List share_start(){

	q = (List)malloc(sizeof(LNode));
	q = create();
	pthread_create(&id_recv,NULL,pshare_recv,q);	
	return q;
}
void share_end(){
	
	pthread_join(id_recv,NULL);
}
int read_int(List q,share *p){
	share_int *si;
	si=(*p).p_int;
    if((*si).local==1){
        int x=(*si).getValue();
        return x;
    }
    else{
        (*p).read = 1;
	    send_read(p);
	    (*p).read=0;
	    share *s;
	    s=(share*)malloc(sizeof(share));
	    memcpy(s, buffer, sizeof(share));
	    (*si).local=1;
	    int x=(*s).i;
        (*si).writex(x);
	    (*p).local=1;
	    read_int(q,p);         	
    }        		
}
double read_double(List q,share *p){
	share_double *sd;
	sd=(*p).p_double;
    if((*sd).local==1){
        double x=(*sd).getValue();
        return x;
    }
    else{
        (*p).read = 1;
	    send_read(p);
	    (*p).read=0;
	    share *s;
	    s=(share*)malloc(sizeof(share));
	    memcpy(s, buffer, sizeof(share));
	    (*sd).local=1;
	    double x=(*s).d;
        (*sd).writex(x);
	    (*p).local=1;
	    read_int(q,p);         	
    }       
}
float read_float(List q,share *p){
	share_float *sf;
	sf=(*p).p_float;
    if((*sf).local==1){
        int x=(*sf).getValue();
        return x;
    }
    else{
        (*p).read = 1;
	    send_read(p);
	    (*p).read=0;
	    share *s;
	    s=(share*)malloc(sizeof(share));
	    memcpy(s, buffer, sizeof(share));
	    (*sf).local=1;
	    int x=(*s).f;
        (*sf).writex(x);
	    (*p).local=1;
	    read_int(q,p);         	
    }        		
}
char read_char(List q,share *p){
	share_char *sc;
	sc=(*p).p_char;
	if((*sc).local==1){
	    char x=(*sc).getValue();
	    return x;
	}
	else{
		(*p).read = 1;
		send_read(p);
		(*p).read=0;
		share *s;
		s=(share*)malloc(sizeof(share));
		memcpy(s, buffer, sizeof(share));
		(*sc).local=1;
		char c=(*s).s;
        (*sc).writex(c);
		(*p).local=1;     
		read_char(q,p);
		}			    				
}
int read_intArray(List q,share *p){
	share_intArray *siA;
	siA=(*p).p_intArray;
   
        (*p).read = 1;
	    send_read(p);
	    (*p).read=0;
	    share *s;
	    s=(share*)malloc(sizeof(share));
	    memcpy(s, buffer, sizeof(share));
	    (*siA).local=1;
	    memcpy((*siA).getPtr(),(*s).array,sizeof((*siA).getPtr()));
	    (*p).local=1;        	     
}
double read_doubleArray(List q,share *p){
	share_doubleArray *sdA;
	sdA=(*p).p_doubleArray;
   
        (*p).read = 1;
	    send_read(p);
	    (*p).read=0;
	    share *s;
	    s=(share*)malloc(sizeof(share));
	    memcpy(s, buffer, sizeof(share));
	    (*sdA).local=1;
	    memcpy((*sdA).getPtr(),(*s).d_array,sizeof((*sdA).getPtr()));
	    (*p).local=1;        	     
}
float read_floatArray(List q,share *p){
	share_floatArray *sfA;
	sfA=(*p).p_floatArray;
   
        (*p).read = 1;
	    send_read(p);
	    (*p).read=0;
	    share *s;
	    s=(share*)malloc(sizeof(share));
	    memcpy(s, buffer, sizeof(share));
	    (*sfA).local=1;
	    memcpy((*sfA).getPtr(),(*s).f_array,sizeof((*sfA).getPtr()));
	    (*p).local=1;        	     
}
char read_charArray(List q,share *p){
	share_charArray *scA;
	scA=(*p).p_charArray;
   
        (*p).read = 1;
	    send_read(p);
	    (*p).read=0;
	    share *s;
	    s=(share*)malloc(sizeof(share));
	    memcpy(s, buffer, sizeof(share));
	    (*scA).local=1;
	    memcpy((*scA).getPtr(),(*s).array,sizeof((*scA).getPtr()));
	    (*p).local=1;
}
void read_memory(List q,share *p){
	share_memory *sm;
	sm=(*p).p_memory;
   
        (*p).read = 1;
	    send_read(p);
	    (*p).read=0;
	    share *s;
	    s=(share*)malloc(sizeof(share));
	    memcpy(s, buffer, sizeof(share));
	    (*sm).local=1;
	    memcpy((*sm).share_buff,(*s).share_buff,sizeof((*s).share_buff));
	    (*p).local=1;
}
int share_int::newValue(){
	share *p;
	p=(share*)malloc(sizeof(share));
	memset(p,0,sizeof(share));
	(*p).share_type=1;	
	(*p).c_type=1;
	strcpy((*p).name, name);
	(*p).read = 1;
	send_read(p);
	(*p).read=0;
	share *sd;
	sd=(share*)malloc(sizeof(share));
	memcpy(sd, buffer, sizeof(share));
    i=(*sd).i;
	return i;
}
char share_char::newValue(){
	share *p;
	p=(share*)malloc(sizeof(share));
	memset(p,0,sizeof(share));	
	(*p).share_type=0;
	(*p).c_type=3;
	strcpy((*p).name,name);
	(*p).read = 1;
	send_read(p);
	(*p).read=0;
	share *sd;
	sd=(share*)malloc(sizeof(share));
	memcpy(sd, buffer, sizeof(share));
    s=(*sd).s;
	return s;   	
}
void share_memory::copyfrom(void *s,int count){
	if ((local == 1)&&(write_allow==1)){
		pthread_mutex_lock(&mutex);
		memcpy(share_buff,s,count);
		pthread_mutex_unlock(&mutex);
		share *p;
		p=(share*)malloc(sizeof(share));
		memcpy((*p).share_buff,share_buff,count);
		(*p).change=1;
        int x=0;
        (*p).c_type=12;
		send_change(p,x);
		(*p).change=0;
		free(p);

	}
	else{
		share *p;
		p=(share*)malloc(sizeof(share));
		memset(p,0,sizeof(share));
		strcpy((*p).name,name);
		memcpy((*p).share_buff,s,count);
		(*p).write = 1;
		(*p).c_type=13;
		send_read(p);
		share *s;
        s=(share*)malloc(sizeof(share));
        memset(s,0,sizeof(share));
        memcpy(s,buffer,sizeof(share));
        if ((strcmp((*s).name, name) == 0) && ((*s).change == 1))
		{
			if((*s).c_type==13){
			memcpy(share_buff,(*s).share_buff,count);	
			(*s).c_type=0;
			(*s).change=0;
		}
		}     	              
        free(p);
        free(s);
    }
}
void share_memory::copyto(void *s,int count){
	
        memcpy(s,share_buff,count);          
}
void share_intArray::copy(int *m,int x){
	if ((local == 1)&&(write_allow==1)){
		pthread_mutex_lock(&mutex);
		memcpy(array,m,x*4);
		pthread_mutex_unlock(&mutex);
		share *p;
		p=(share*)malloc(sizeof(share));
		memset(p,0,sizeof(share));
		strcpy((*p).name, name);
		memcpy((*p).array,m,x*4);
        int xd=0;
        (*p).change=1;
        (*p).c_type=2;
        (*p).array_cpy_size=x;
		send_change(p,xd);
		(*p).change=0;

	}
	else{	
    share *p;
	p=(share*)malloc(sizeof(share));
	memset(p,0,sizeof(share));
    strcpy((*p).name, name);
	memcpy((*p).array,m,x*4);
	(*p).write = 1;
	(*p).c_type=2;
	(*p).array_cpy_size=x;	
	send_read(p);
    share *y;
	y = (share*)malloc(sizeof(share));
	memcpy(y, buffer, sizeof(share));

	if ((strcmp((*y).name, name) == 0) && ((*y).change == 1))
		{
			if((*y).c_type==2){
			memcpy(array,(*y).array,x*4);	
			(*y).c_type=0;
			(*y).change=0;
		}
		}     	
	free(y);
	    } 
}
void share_doubleArray::copy(int *m,int x){
	if ((local == 1)&&(write_allow==1)){
		pthread_mutex_lock(&mutex);
		memcpy(array,m,x*8);
		pthread_mutex_unlock(&mutex);
		share *p;
		p=(share*)malloc(sizeof(share));
		memset(p,0,sizeof(share));
		strcpy((*p).name, name);
		memcpy((*p).d_array,m,x*8);
        int xd=0;
        (*p).change=1;
        (*p).c_type=2;
        (*p).array_cpy_size=x;
		send_change(p,xd);
		(*p).change=0;

	}
	else{	
    share *p;
	p=(share*)malloc(sizeof(share));
	memset(p,0,sizeof(share));
    strcpy((*p).name, name);
	memcpy((*p).d_array,m,x*8);
	(*p).write = 1;
	(*p).c_type=9;
	(*p).array_cpy_size=x;	
	send_read(p);
    share *y;
	y = (share*)malloc(sizeof(share));
	memcpy(y, buffer, sizeof(share));

	if ((strcmp((*y).name, name) == 0) && ((*y).change == 1))
		{
			if((*y).c_type==9){
			memcpy(array,(*y).d_array,x*8);	
			(*y).c_type=0;
			(*y).change=0;
		}
		}     	
	free(y);
	    } 
}
void share_floatArray::copy(int *m,int x){
	if ((local == 1)&&(write_allow==1)){
		pthread_mutex_lock(&mutex);
		memcpy(array,m,x*4);
		pthread_mutex_unlock(&mutex);
		share *p;
		p=(share*)malloc(sizeof(share));
		memset(p,0,sizeof(share));
		strcpy((*p).name, name);
		memcpy((*p).f_array,m,x*4);
        int xd=0;
        (*p).change=1;
        (*p).c_type=2;
        (*p).array_cpy_size=x;
		send_change(p,xd);
		(*p).change=0;

	}
	else{	
    share *p;
	p=(share*)malloc(sizeof(share));
	memset(p,0,sizeof(share));
    strcpy((*p).name, name);
	memcpy((*p).f_array,m,x*4);
	(*p).write = 1;
	(*p).c_type=10;
	(*p).array_cpy_size=x;	
	send_read(p);
    share *y;
	y = (share*)malloc(sizeof(share));
	memcpy(y, buffer, sizeof(share));

	if ((strcmp((*y).name, name) == 0) && ((*y).change == 1))
		{
			if((*y).c_type==10){
			memcpy(array,(*y).f_array,x*4);	
			(*y).c_type=0;
			(*y).change=0;
		}
		}     	
	free(y);
	    } 
}
void share_charArray::copy(char *m,int x){
	if ((local == 1)&&(write_allow==1)){
		pthread_mutex_lock(&mutex);
		memcpy(array,m,x);
		pthread_mutex_unlock(&mutex);
		share *p;
		p=(share*)malloc(sizeof(share));
		memset(p,0,sizeof(share));
		strcpy((*p).name, name);
		memcpy((*p).c_array,m,x);
        (*p).change=1;
        (*p).c_type=4;
        (*p).array_cpy_size=x;
		send_change(p,0);
		(*p).change=0;

	}
	else{	
    share *p;
	p=(share*)malloc(sizeof(share));
	memset(p,0,sizeof(share));
    strcpy((*p).name, name);
	memcpy((*p).c_array,m,x);
	(*p).write = 1;
	(*p).c_type=4;
	(*p).array_cpy_size=x;	
	send_read(p);
    share *y;
	y = (share*)malloc(sizeof(share));
	memcpy(y, buffer, sizeof(share));

	if ((strcmp((*y).name, name) == 0) && ((*y).change == 1))
		{
			if((*y).c_type==4){
			memcpy(array,(*y).c_array,x);	
			(*y).c_type=0;
			(*y).change=0;
		}
		}     	
	free(y);
	    } 
}
char share_char::operator=(const char j){
	if ((local == 1)&&(write_allow==1)){
	pthread_mutex_lock(&mutex);
	writex(j);
	pthread_mutex_unlock(&mutex);
	share *p;
	p=(share*)malloc(sizeof(share));
	memset(p,0,sizeof(share));
	strcpy((*p).name, name);
	(*p).change=1;
	(*p).c_type=3;
	(*p).s=j;
	send_change(p,0);
	(*p).change=0;
	}
	else{
	share *p;
	p=(share*)malloc(sizeof(share));
	memset(p,0,sizeof(share));
    strcpy((*p).name, name);
	(*p).s = j;
	(*p).write = 1;
	(*p).c_type=3;
	send_read(p);
    share *y;
	y = (share*)malloc(sizeof(share));
	memcpy(y, buffer, sizeof(share));
	if ((strcmp((*y).name, name) == 0) && ((*y).change == 1))
		{
			if((*y).c_type==3){
			char x=(*y).s;
			writex(x);	
		}
		}     	
	free(y);
	    }
}
char share_char::operator=(const share_char share1){
	char j=share1.s;
	if ((local == 1)&&(write_allow==1)){
	pthread_mutex_lock(&mutex);
	writex(j);
	pthread_mutex_unlock(&mutex);
	share *p;
	p=(share*)malloc(sizeof(share));
	memset(p,0,sizeof(share));
	strcpy((*p).name, name);
	(*p).change=1;
	(*p).c_type=3;
	(*p).s=j;
	send_change(p,0);
	(*p).change=0;
	}
	else{
	share *p;
	p=(share*)malloc(sizeof(share));
	memset(p,0,sizeof(share));
    strcpy((*p).name, name);
	(*p).s = j;
	(*p).write = 1;
	(*p).c_type=3;
	send_read(p);
    share *y;
	y = (share*)malloc(sizeof(share));
	memcpy(y, buffer, sizeof(share));
	if ((strcmp((*y).name, name) == 0) && ((*y).change == 1))
		{
			if((*y).c_type==3){
			char x=(*y).s;
			writex(x);	
		}
		}     	
	free(y);
	    }
}
int share_int::operator=(const int j){

	if ((local == 1)&&(write_allow==1)){
	pthread_mutex_lock(&mutex);
	writex(j);
	pthread_mutex_unlock(&mutex);
	share *p;
	p=(share*)malloc(sizeof(share));
	memset(p,0,sizeof(share));
	strcpy((*p).name, name);
	(*p).change=1;
	(*p).c_type=1;
	send_change(p,j);
	(*p).change=0;	
	}
	else{
	share *p;
	p=(share*)malloc(sizeof(share));
	memset(p,0,sizeof(share));
    strcpy((*p).name, name);
	(*p).write_data = j;
	(*p).write = 1;
	(*p).c_type=1;
	send_read(p);
    share *y;
	y = (share*)malloc(sizeof(share));
	memcpy(y, buffer, sizeof(share));
	if ((strcmp((*y).name, name) == 0) && ((*y).change == 1))
		{
			if((*y).c_type==1){
			int x=(*y).change_data;
			writex(x);	
		}
		}     	
	free(y);
	    }
}
int share_int::operator=(const share_int share1){
	int j=share1.i;
	if ((local == 1)&&(write_allow==1)){
	pthread_mutex_lock(&mutex);
	writex(j);
	pthread_mutex_unlock(&mutex);
	share *p;
	p=(share*)malloc(sizeof(share));
	memset(p,0,sizeof(share));
	strcpy((*p).name, name);
	(*p).change=1;
	(*p).c_type=1;
	send_change(p,j);
	(*p).change=0;	
	}
	else{
	share *p;
	p=(share*)malloc(sizeof(share));
	memset(p,0,sizeof(share));
    strcpy((*p).name, name);
	(*p).write_data = j;
	(*p).write = 1;
	(*p).c_type=1;
	send_read(p);
    share *y;
	y = (share*)malloc(sizeof(share));
	memcpy(y, buffer, sizeof(share));
	if ((strcmp((*y).name, name) == 0) && ((*y).change == 1))
		{
			if((*y).c_type==1){
			int x=(*y).change_data;
			writex(x);	
		}
		}     	
	free(y);
	    }
}
double share_double::operator=(const double j){

	if ((local == 1)&&(write_allow==1)){
	pthread_mutex_lock(&mutex);
	writex(j);
	pthread_mutex_unlock(&mutex);
	share *p;
	p=(share*)malloc(sizeof(share));
	memset(p,0,sizeof(share));
	strcpy((*p).name, name);
	(*p).change=1;
	(*p).c_type=7;
	(*p).d=j;
	send_change(p,0);
	(*p).change=0;	
	}
	else{
	share *p;
	p=(share*)malloc(sizeof(share));
	memset(p,0,sizeof(share));
    strcpy((*p).name, name);
	(*p).d = j;
	(*p).write = 1;
	(*p).c_type=7;
	send_read(p);
    share *y;
	y = (share*)malloc(sizeof(share));
	memcpy(y, buffer, sizeof(share));
	if ((strcmp((*y).name, name) == 0) && ((*y).change == 1))
		{
			if((*y).c_type==7){
			double x=(*y).d;
			writex(x);	
		}
		}     	
	free(y);
	    }
}
double share_double::operator=(const share_double share1){
	double j=share1.i;
	if ((local == 1)&&(write_allow==1)){
	pthread_mutex_lock(&mutex);
	writex(j);
	pthread_mutex_unlock(&mutex);
	share *p;
	p=(share*)malloc(sizeof(share));
	memset(p,0,sizeof(share));
	strcpy((*p).name, name);
	(*p).change=1;
	(*p).c_type=7;
	(*p).d=j;
	send_change(p,0);
	(*p).change=0;	
	}
	else{
	share *p;
	p=(share*)malloc(sizeof(share));
	memset(p,0,sizeof(share));
    strcpy((*p).name, name);
	(*p).write_data = j;
	(*p).write = 1;
	(*p).c_type=7;
	send_read(p);
    share *y;
	y = (share*)malloc(sizeof(share));
	memcpy(y, buffer, sizeof(share));
	if ((strcmp((*y).name, name) == 0) && ((*y).change == 1))
		{
			if((*y).c_type==7){
			double x=(*y).d;
			writex(x);	
		}
		}     	
	free(y);
	    }
}
float share_float::operator=(const float j){

	if ((local == 1)&&(write_allow==1)){
	pthread_mutex_lock(&mutex);
	writex(j);
	pthread_mutex_unlock(&mutex);
	share *p;
	p=(share*)malloc(sizeof(share));
	memset(p,0,sizeof(share));
	strcpy((*p).name, name);
	(*p).change=1;
	(*p).c_type=8;
	(*p).f=j;
	send_change(p,0);
	(*p).change=0;	
	}
	else{
	share *p;
	p=(share*)malloc(sizeof(share));
	memset(p,0,sizeof(share));
    strcpy((*p).name, name);
	(*p).d = j;
	(*p).write = 1;
	(*p).c_type=8;
	send_read(p);
    share *y;
	y = (share*)malloc(sizeof(share));
	memcpy(y, buffer, sizeof(share));
	if ((strcmp((*y).name, name) == 0) && ((*y).change == 1))
		{
			if((*y).c_type==8){
			float x=(*y).f;
			writex(x);	
		}
		}     	
	free(y);
	    }
}
float share_float::operator=(const share_float share1){
	float j=share1.i;
	if ((local == 1)&&(write_allow==1)){
	pthread_mutex_lock(&mutex);
	writex(j);
	pthread_mutex_unlock(&mutex);
	share *p;
	p=(share*)malloc(sizeof(share));
	memset(p,0,sizeof(share));
	strcpy((*p).name, name);
	(*p).change=1;
	(*p).c_type=8;
	(*p).f=j;
	send_change(p,0);
	(*p).change=0;	
	}
	else{
	share *p;
	p=(share*)malloc(sizeof(share));
	memset(p,0,sizeof(share));
    strcpy((*p).name, name);
	(*p).write_data = j;
	(*p).write = 1;
	(*p).c_type=8;
	send_read(p);
    share *y;
	y = (share*)malloc(sizeof(share));
	memcpy(y, buffer, sizeof(share));
	if ((strcmp((*y).name, name) == 0) && ((*y).change == 1))
		{
			if((*y).c_type==8){
			float x=(*y).f;
			writex(x);	
		}
		}     	
	free(y);
	    }
}
void share_intArray::set(int x,int y){
	if ((local == 1)&&(write_allow==1)){
	
	array[x]=y;
	
	share *p;
	p=(share*)malloc(sizeof(share));
	memset(p,0,sizeof(share));
	strcpy((*p).name, name);
	(*p).change=1;
	(*p).c_type=5;
	(*p).array_id=x;
	(*p).i=y;
	send_change(p,0);
	(*p).change=0;
	}
	else{
	share *p;
	p=(share*)malloc(sizeof(share));
	memset(p,0,sizeof(share));
    strcpy((*p).name, name);
	(*p).i = y;
	(*p).write = 1;
	(*p).c_type=5;
	(*p).array_id=x;
	send_read(p);
    share *y;
	y = (share*)malloc(sizeof(share));
	memcpy(y, buffer, sizeof(share));
	if ((strcmp((*y).name, name) == 0) && ((*y).change == 1))
		{
			if((*y).c_type==5){
			
			int aiAd;
			aiAd=(*y).array_id;
			int x=(*y).array[aiAd];
			array[aiAd]=x;
				
		}
		}     	
	free(y);
	    }
}
void share_doubleArray::set(int x,double y){
	if ((local == 1)&&(write_allow==1)){
	
	array[x]=y;
	
	share *p;
	p=(share*)malloc(sizeof(share));
	memset(p,0,sizeof(share));
	strcpy((*p).name, name);
	(*p).change=1;
	(*p).c_type=12;
	(*p).array_id=x;
	(*p).d=y;
	send_change(p,0);
	(*p).change=0;
	}
	else{
	share *p;
	p=(share*)malloc(sizeof(share));
	memset(p,0,sizeof(share));
    strcpy((*p).name, name);
	(*p).d = y;
	(*p).write = 1;
	(*p).c_type=12;
	(*p).array_id=x;
	send_read(p);
    share *y;
	y = (share*)malloc(sizeof(share));
	memcpy(y, buffer, sizeof(share));
	if ((strcmp((*y).name, name) == 0) && ((*y).change == 1))
		{
			if((*y).c_type==12){
			
			int aiAd;
			aiAd=(*y).array_id;
			double x=(*y).d_array[aiAd];
			array[aiAd]=x;
				
		}
		}     	
	free(y);
	    }
}
void share_floatArray::set(int x,float y){
	if ((local == 1)&&(write_allow==1)){
	
	array[x]=y;
	
	share *p;
	p=(share*)malloc(sizeof(share));
	memset(p,0,sizeof(share));
	strcpy((*p).name, name);
	(*p).change=1;
	(*p).c_type=11;
	(*p).array_id=x;
	(*p).f=y;
	send_change(p,0);
	(*p).change=0;
	}
	else{
	share *p;
	p=(share*)malloc(sizeof(share));
	memset(p,0,sizeof(share));
    strcpy((*p).name, name);
	(*p).f = y;
	(*p).write = 1;
	(*p).c_type=11;
	(*p).array_id=x;
	send_read(p);
    share *y;
	y = (share*)malloc(sizeof(share));
	memcpy(y, buffer, sizeof(share));
	if ((strcmp((*y).name, name) == 0) && ((*y).change == 1))
		{
			if((*y).c_type==11){
			
			int aiAd;
			aiAd=(*y).array_id;
			float x=(*y).f_array[aiAd];
			array[aiAd]=x;
				
		}
		}     	
	free(y);
	    }
}
void share_charArray::set(int x,char y){
	if ((local == 1)&&(write_allow==1)){
	
	array[x]=y;
	
	share *p;
	p=(share*)malloc(sizeof(share));
	memset(p,0,sizeof(share));
	strcpy((*p).name, name);
	(*p).change=1;
	(*p).c_type=6;
	(*p).array_id=x;
	(*p).s=y;
	send_change(p,0);
	(*p).change=0;	
	}
	else{
	share *p;
	p=(share*)malloc(sizeof(share));
	memset(p,0,sizeof(share));
    strcpy((*p).name, name);
	(*p).s = y;
	(*p).write = 1;
	(*p).c_type=6;
	(*p).array_id=x;
	send_read(p);
    share *y;
	y = (share*)malloc(sizeof(share));
	memcpy(y, buffer, sizeof(share));
	if ((strcmp((*y).name, name) == 0) && ((*y).change == 1))
		{
			if((*y).c_type==6){
			int caid;
			caid=(*y).array_id;
			int x=(*y).c_array[caid];
			array[caid]=x;
				
		}
		}     	
	free(y);
	    }
}		
share_int operator+(const int &j,const share_int &share1){
	
	return(j+share1.i);
}
share_int operator+(const share_int &share1,const int &j){
	
	return(share1.i+j);
}
share_int operator-(const int &j,const share_int &share1){
	
	return(j-share1.i);
}
share_int operator-(const share_int &share1,const int &j){
	
	return(share1.i-j);
}
share_int operator*(const int &j,const share_int &share1){
	
	return(j*share1.i);
}
share_int operator*(const share_int &share1,const int &j){
	
	return(share1.i*j);
}
share_int operator/(const int &j,const share_int &share1){
	
	return(j/share1.i);
}
share_int operator/(const share_int &share1,const int &j){
	
	return(share1.i/j);
}
share_double operator+(const double &j,const share_double &share1){
	
	return(j+share1.i);
}
share_double operator+(const share_double &share1,const double &j){
	
	return(share1.i+j);
}
share_double operator-(const double &j,const share_double &share1){
	
	return(j-share1.i);
}
share_double operator-(const share_double &share1,const double &j){
	
	return(share1.i-j);
}
share_double operator*(const double &j,const share_double &share1){
	
	return(j*share1.i);
}
share_double operator*(const share_double &share1,const double &j){
	
	return(share1.i*j);
}
share_double operator/(const double &j,const share_double &share1){
	
	return(j/share1.i);
}
share_double operator/(const share_double &share1,const double &j){
	
	return(share1.i/j);
}
share_float operator+(const float &j,const share_float &share1){
	
	return(j+share1.i);
}
share_float operator+(const share_float &share1,const float &j){
	
	return(share1.i+j);
}
share_float operator-(const float &j,const share_float &share1){
	
	return(j-share1.i);
}
share_float operator-(const share_float &share1,const float &j){
	
	return(share1.i-j);
}
share_float operator*(const float &j,const share_float &share1){
	
	return(j*share1.i);
}
share_float operator*(const share_float &share1,const float &j){
	
	return(share1.i*j);
}
share_float operator/(const float &j,const share_float &share1){
	
	return(j/share1.i);
}
share_float operator/(const share_float &share1,const float &j){
	
	return(share1.i/j);
}
void share_int::create(const char *cname){	
	
	strcpy(name, cname);
	local=1;
	write_allow=1;
	share *s;
	s=(share*)malloc(sizeof(share));
	memset(s,0,sizeof(share));
	(*s).share_type=1;
	(*s).p_int=this;
	(*s).local=1;
	(*s).write_allow=1;
	strcpy((*s).name, cname);
	insert(q,s);	  
}
void share_double::create(const char *cname){	
	
	strcpy(name, cname);
	local=1;
	write_allow=1;
	share *s;
	s=(share*)malloc(sizeof(share));
	memset(s,0,sizeof(share));
	(*s).share_type=7;
	(*s).p_double=this;
	(*s).local=1;
	(*s).write_allow=1;
	strcpy((*s).name, cname);
	insert(q,s);	  
}
void share_float::create(const char *cname){	
	
	strcpy(name, cname);
	local=1;
	write_allow=1;
	share *s;
	s=(share*)malloc(sizeof(share));
	memset(s,0,sizeof(share));
	(*s).share_type=8;
	(*s).p_float=this;
	(*s).local=1;
	(*s).write_allow=1;
	strcpy((*s).name, cname);
	insert(q,s);	  
}
void share_char::create(const char *cname){	
	
	strcpy(name, cname);
	local=1;
	write_allow=1;
	share *s;
	s=(share*)malloc(sizeof(share));
	memset(s,0,sizeof(share));
	(*s).share_type=2;
	(*s).p_char=this;
	(*s).local=1;
	(*s).write_allow=1;
	strcpy((*s).name, cname);
	insert(q,s);	  
}
void share_int::get(const char *cname){ 	
	strcpy(name, cname);
	share *s;
	s=(share*)malloc(sizeof(share));
	memset(s,0,sizeof(share));
	(*s).share_type=1;	
	(*s).p_int=this;
	(*s).c_type=1;
	strcpy((*s).name, cname);
	read_int(q,s);
	insert(q,s);   
}
void share_double::get(const char *cname){ 	
	strcpy(name, cname);
	share *s;
	s=(share*)malloc(sizeof(share));
	memset(s,0,sizeof(share));
	(*s).share_type=7;	
	(*s).p_double=this;
	(*s).c_type=7;
	strcpy((*s).name, cname);
	read_int(q,s);
	insert(q,s);   
}
void share_float::get(const char *cname){ 	
	strcpy(name, cname);
	share *s;
	s=(share*)malloc(sizeof(share));
	memset(s,0,sizeof(share));
	(*s).share_type=8;	
	(*s).p_float=this;
	(*s).c_type=8;
	strcpy((*s).name, cname);
	read_int(q,s);
	insert(q,s);   
}
void share_char::get(const char *cname){	
	strcpy(name, cname);
	share *s;
	s=(share*)malloc(sizeof(share));
	memset(s,0,sizeof(share));	
	(*s).share_type=2;
	(*s).p_char=this;
	(*s).c_type=3;
	strcpy((*s).name,cname);
	read_char(q,s);  
	insert(q,s); 
}
void share_intArray::create(const char *cname){
	strcpy(name, cname);
	local=1;
	write_allow=1;
	share *s;
	s=(share*)malloc(sizeof(share));
	memset(s,0,sizeof(share));
	(*s).share_type=3;
	(*s).p_intArray=this;
	(*s).local=1;
	(*s).write_allow=1;
	strcpy((*s).name, cname);
	insert(q,s);
}
void share_doubleArray::create(const char *cname){
	strcpy(name, cname);
	local=1;
	write_allow=1;
	share *s;
	s=(share*)malloc(sizeof(share));
	memset(s,0,sizeof(share));
	(*s).share_type=9;
	(*s).p_doubleArray=this;
	(*s).local=1;
	(*s).write_allow=1;
	strcpy((*s).name, cname);
	insert(q,s);
}
void share_floatArray::create(const char *cname){
	strcpy(name, cname);
	local=1;
	write_allow=1;
	share *s;
	s=(share*)malloc(sizeof(share));
	memset(s,0,sizeof(share));
	(*s).share_type=10;
	(*s).p_floatArray=this;
	(*s).local=1;
	(*s).write_allow=1;
	strcpy((*s).name, cname);
	insert(q,s);
}
void share_charArray::create(const char *cname){
	strcpy(name, cname);
	local=1;
	write_allow=1;
	share *s;
	s=(share*)malloc(sizeof(share));
	memset(s,0,sizeof(share));
	(*s).share_type=4;
	(*s).p_charArray=this;
	(*s).local=1;
	(*s).write_allow=1;
	strcpy((*s).name, cname);
	insert(q,s);
}
void share_intArray::get(const char *cname){
	strcpy(name, cname);
	share *s;
	s=(share*)malloc(sizeof(share));
	memset(s,0,sizeof(share));
	(*s).share_type=3;	
	(*s).p_intArray=this;
	(*s).c_type=2;
	strcpy((*s).name, cname);
	read_int(q,s);
	insert(q,s); 
}
void share_doubleArray::get(const char *cname){
	strcpy(name, cname);
	share *s;
	s=(share*)malloc(sizeof(share));
	memset(s,0,sizeof(share));
	(*s).share_type=9;	
	(*s).p_doubleArray=this;
	(*s).c_type=9;
	strcpy((*s).name, cname);
	read_int(q,s);
	insert(q,s); 
}
void share_floatArray::get(const char *cname){
	strcpy(name, cname);
	share *s;
	s=(share*)malloc(sizeof(share));
	memset(s,0,sizeof(share));
	(*s).share_type=10;	
	(*s).p_floatArray=this;
	(*s).c_type=10;
	strcpy((*s).name, cname);
	read_int(q,s);
	insert(q,s); 
}
void share_charArray::get(const char *cname){
	strcpy(name, cname);
	share *s;
	s=(share*)malloc(sizeof(share));
	memset(s,0,sizeof(share));
	(*s).share_type=4;	
	(*s).p_charArray=this;
	(*s).c_type=4;
	strcpy((*s).name, cname);
	read_charArray(q,s);
	insert(q,s);
}
void share_memory::create(const char *cname){
	strcpy(name, cname);
	local=1;
	write_allow=1;
	share *s;
	s=(share*)malloc(sizeof(share));
	memset(s,0,sizeof(share));
	(*s).share_type=13;
	(*s).p_memory=this;
	(*s).local=1;
	(*s).write_allow=1;
	strcpy((*s).name, cname);
	insert(q,s);
}
void share_memory::get(const char *cname){
	strcpy(name, cname);
	share *s;
	s=(share*)malloc(sizeof(share));
	memset(s,0,sizeof(share));
	(*s).share_type=13;	
	(*s).p_memory=this;
	(*s).c_type=13;
	strcpy((*s).name, cname);
	read_memory(q,s);
	insert(q,s); 
}


