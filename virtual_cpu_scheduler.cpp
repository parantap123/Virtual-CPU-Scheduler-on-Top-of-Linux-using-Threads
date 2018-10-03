	#include<iostream>
	#include<pthread.h>
	#include<stdlib.h>
	#include<time.h>
	#include<bits/stdc++.h>
	#include<unistd.h>
	#include<signal.h>
	using namespace std;
	pthread_t* th_id;
	sigset_t fsigset;
	int status[100],i,j;
	queue<int>q;
	int N;
	int thread_running;
	int thread_number_array[100];
	void signal_handler(int siguser)
	{
		if(siguser==SIGUSR1)
		pause();
		//signal(SIGUSR2,signal_handler);
		
	}
	void*scheduler(void* x)
	{
		signal(SIGUSR2,signal_handler);
		while(!q.empty())
		{
			thread_running=q.front();
			if(!status[thread_running])
			{
				cout<<"thread running = "<<thread_running<<endl;
				pthread_kill(th_id[thread_running],SIGUSR2);
				sleep(1);
				pthread_kill(th_id[thread_running],SIGUSR1);
				q.pop(); 
				if(status[thread_running]==0)
				{
					q.push(thread_running);
				}
				else
					cout<<"over="<<thread_running<<endl;	
				pthread_kill(th_id[N+1],SIGUSR2);
				pause();
				
				
			}
			
		}
		pthread_exit(NULL);
	}
	void* reporter(void* x)
	{
		signal(SIGUSR2,signal_handler);
		pause();
		while(!q.empty())
		{
			if(status[thread_running]==1)
				cout<<thread_running<<" = "<<"Terminated"<<endl;
			else 
				cout<<thread_running<<" = switched"<<endl;
			pthread_kill(th_id[N],SIGUSR2);
			pause();
			if(q.size()==0)
				cout<<thread_running<<" = "<<"Terminated "<<endl;
			
		}
		cout<<"every thread executed"<<endl;
		pthread_kill(th_id[N],SIGUSR2);
		pthread_exit(NULL);	
	}
	void* worker(void* ptr )
	{
		sigset_t set;
		int * thread_number = (int *)ptr;
		signal(SIGUSR1,signal_handler);
		signal(SIGUSR2,signal_handler);
		
		int int_array[1002];
		pause();
		
		srand(time(0));
		for(int k=0;k<1000;k++)
		{
			
			int_array[k]=rand()%200;
		}
		
		sort(int_array,int_array+1000);
		
		sleep(rand()%10);
		status[*thread_number]=1;
		pthread_exit(NULL);	
		
	}
	
	
	
	
	int main()
	{
		srand(time(NULL));
		sigset_t set;
		sigfillset(&set);//put all the signal in a set
		sigdelset(&set,SIGINT);//SIGINT will be removed from the set
		sigemptyset(&fsigset);
		sigaddset(&fsigset, SIGUSR2);
		cin>>N;
		memset(status, 0, sizeof(status));
		th_id=(pthread_t*)calloc(N+2,sizeof(pthread_t));
		
		for(int k=0;k<N;k++)
		{
			thread_number_array[k]=k;
		}
		for(j=0;j<N;j++)
		{
			pthread_create(&th_id[j], NULL, &worker, (void *)&thread_number_array[j]);
			q.push(j);
		}
		cout<<"scheduler will start"<<endl;
		pthread_create(&th_id[N], NULL, &scheduler, (void *)N);
		cout<<"reporter will start"<<endl;
		pthread_create(&th_id[N+1], NULL, &reporter, (void *)(N+1)); 
		for(int k=0;k<N;k++)
		{
			pthread_join(th_id[k],NULL);
		}
		pthread_join(th_id[N],NULL);
		pthread_join(th_id[N+1],NULL);
		return 0;
	}
	
	
	