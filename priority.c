
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
struct node{
	int pid;
	int priority;
	int arrival_time;
	int burst_time;
	int ft;
	char state[20];
	struct node *next;
};
void schedule(struct node *,int,int,int,int);
void Free(struct node *head);
void Print(struct node*,int);
void schedule2(struct node *head, int pid,int pri,int arr,int bur);


FILE *wp;

int main(int argc,char*argv[])
{

	struct node* head=(struct node*)malloc(sizeof(struct node));

	int pid,priority,arr,bur;
	int alltime=0;
	FILE *fp;
	fp=fopen("input1.txt","r");
	wp=fopen("output.txt","w");
	if(fp==NULL)
	{
		printf(" ERROR");
	}
	if(wp==NULL)
	{
		printf(" ERROR");
	}
	head->next=NULL;
	
	while(fscanf(fp,"%d %d %d %d",&pid,&priority,&arr,&bur)!=EOF)
	{
		schedule(head,pid,priority,arr,bur);
		alltime=alltime+bur;
	}

	Print(head,alltime);

	Free(head);
	fclose(fp);
	fclose(wp);
}
void Print(struct node *head,int allt)
{
	struct node *p=head->next;
	struct node *temp=p;
	struct node* ready=(struct node*)malloc(sizeof(struct node));
	int time=0,burst=0;
	int waitingtime=0,turnaroundtime=0,responsetime=0;
	int i,arr[10],k=0,idle=0;
	int finish[11],j=0,s,sum=0,f=0;
	ready->next=NULL;

	while(1)
	{

		while(p->arrival_time>time){
			fprintf(wp,"<time %d>  ----system idle ----\n",time);
			idle++;
			time++;
		}

		for(i=0;i<p->burst_time;i++)
		{
			if(temp!=NULL)
			{
				if (temp->arrival_time==time){
					fprintf(wp,"<time %d>  [new arrival]process %d\n",time,temp->pid);
					strcpy(p->state,"Ready");
					arr[j++]=time;
					if(temp->priority>p->priority)
					{
						if(temp->arrival_time==time)
						{
							fprintf(wp,"-----------------context switching\n\n");
							p->burst_time-=temp->arrival_time-p->arrival_time;
							schedule2(ready,p->pid,p->priority,time,p->burst_time);
							p=temp;
							i=0;
						}
					}
					else
					{
						if(p->pid!=temp->pid)
						{
							schedule2(ready,temp->pid,temp->priority,time,temp->burst_time);
						}
					}
					temp=temp->next;

				}
			}
			fprintf(wp,"<time %d> process %d is running\n",time,p->pid);
			strcpy(p->state,"Running");

			time++;
		}

		finish[k++]=time;
		fprintf(wp,"<time %d>  process %d is finished\n",time,p->pid);
		strcpy(p->state,"Terminated");
		
		if(p->next!=NULL)
			fprintf(wp,"---------------(context switching)\n\n");
		p=p->next;

		if(p==NULL)
		{
			if(f!=1)
				fprintf(wp,"---------------(context switching)\n\n");
			p=ready->next;
			f++;
		}
		if(f==2)
		{
			fprintf(wp,"\n\n<time %d> all process is finished.\n",time);
			break;
		}
	}
	for(s=0;s<9;s++)
	{
		waitingtime=waitingtime+finish[s];
		responsetime+=finish[s];
	}
	for(s=0;s<10;s++)
	{
		sum=finish[s]-arr[s];
		turnaroundtime=sum+turnaroundtime;
	}

	fprintf(wp,"-----------------------------------\n");
	fprintf(wp,"-----------------------------------\n");
	fprintf(wp,"Avarage Cpu usage : %d percent\n",((time-idle)*100)/time);
	fprintf(wp,"Avarage waiting time : %d\n",waitingtime/10);
	fprintf(wp,"Avarage response time : %d\n",(responsetime+arr[0])/10);
	fprintf(wp,"Avarage turnaround time : %d\n",turnaroundtime/10);
}


void schedule(struct node *head, int pid,int pri,int arr,int bur)
{
	struct node *prev=head,*p=head->next;
	struct node *newnode;

	while(p)
	{
		if(p->arrival_time>=arr)
			break;
		prev=p;
		p=p->next;
	}

	newnode=(struct node*)malloc(sizeof(struct node));
	newnode->pid=pid;
	newnode->priority=pri;
	newnode->arrival_time=arr;
	newnode->burst_time=bur;
	strcpy(newnode->state,"New");

	prev->next=newnode;
	newnode->next=p;

}

void schedule2(struct node *head, int pid,int pri,int time,int bur)
{
	struct node *prev=head,*p=head->next;
	struct node *newnode;
	while(p)
	{

		if(p->priority<=pri)
			break;

		prev=p;
		p=p->next;
	}
	time++;
	newnode=(struct node*)malloc(sizeof(struct node));
	newnode->pid=pid;
	
	newnode->priority=pri;
	
	newnode->burst_time=bur;
	strcpy(newnode->state,"waiting");

	prev->next=newnode;
	newnode->next=p;

}
void Free(struct node *head)
{
	struct node *p=head,*prev=NULL;
	while(p)
	{
		prev=p;
		p=p->next;
		free(prev);
	}
}


