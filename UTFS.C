//ghazal sahebzamani-810192411
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include <string.h>
#include"UTFS.h"
#define TRUE 1
#define FALSE 0
#define START_POINT -1
#define MOVE 128
int encryption_bit_operation(int k,int count,int i, char *ptr);
void get_out(int j,FILE *input,FILE *output, char *ptr);
int decryption_bit_operation(int k,int count,int i, char *ptr);
typedef struct file File;
typedef struct file_record record;

int init_storage( Storage *strg, unsigned int sector_size, unsigned int disk_size)
{ 
	 char* ptr;
	record* rec;
	Sector* empty_head;
	Sector* newly;
	int i=0;
	//checking for validity;
	if(strg==NULL || sector_size<0 || disk_size<0 ||sector_size>disk_size || strg->empty==NULL || strg->rec== NULL || strg->disk_size==NULL || strg->sector_size==NULL || strg->ptr==NULL)
		return -1;

	ptr=(char*)malloc(disk_size); //we should allocate this whole size for disk
	empty_head=(Sector*)malloc(sizeof(Sector)); //and for each secotor & building a dummy head
	rec=(record*)malloc(sizeof(record)); //an for recs,and for each secotor & building a dummy head
	rec->next=NULL; //the linked list should have an end
	empty_head->next=NULL;	
	if(ptr==NULL||empty_head==NULL||rec==NULL) //in case allocation is unsuccessful
	{   //it should free all the data
		free(ptr);
		free(empty_head); 

		free(rec);
		return -1;
	}

	for(i=0;i<(int)disk_size/(int)sector_size;i++) //now we should construct a linked list for empty nodes according to the spaces
	{
		newly=(Sector*)malloc(sizeof(Sector)); 
		if (newly==NULL)
		{
			free(newly);
			free(ptr);
			free(empty_head);

			free(rec);
			return -1;
		}

		newly->next=empty_head; //the following lines put the recent nodes at the beginning the list,and the beginning of the linked list,pushing the dummy head to the end
		empty_head=newly;
		newly->number=i+1;
		newly->head=&ptr[i*sector_size]; //each head should be located after sectors in the disk
	}

	strg->empty=empty_head; //now initializing strg...
	strg->empty->next=strg->empty->next;
	strg->disk_size=disk_size;
	strg->sector_size=sector_size;
	strg->ptr=ptr;
	strg->rec=rec;

	return 0;
}
int store_file( Storage *strg, char* file_name, unsigned char key)
{ 

	Sector* current;
	record* curr_record;
	record* new_rec;
	Sector* new_file_node;
	Sector* file_head;
	int available_space;
	FILE *input;//these pointers point to the files opened below.
	int i=0,j=0,mod,flag1=FALSE,read=-2,flag;
   unsigned char q,k; //assignment is necessary due to bitwise operators used as shift.
	char *ptr;
	long size;

	int count=START_POINT; //because at the end of getting the first sector,counter should be zero(counter of sectors should start from zero
	k=key;

	if(strg==NULL|| strlen(file_name)>255||strg->rec==NULL || strg->empty==NULL) //invalid data
		return -1;
	for(curr_record=strg->rec;curr_record!=NULL;curr_record=curr_record->next) //searches for a record having the specefied name
	{
		if(curr_record->name==file_name)
			return -5;
	}

	new_rec=(record*)malloc(sizeof (record)); //makes a new record
	if (new_rec==NULL)
		return -3; //unsuccessful allocation
	for(curr_record=strg->rec;curr_record!=NULL;curr_record=curr_record->next) //searches for end of the list
	{
		if(curr_record->next==NULL)
		{
			curr_record->next=new_rec;
			new_rec->next=NULL;
			break;
		}

	}



	file_head=(Sector*)malloc(sizeof(Sector)); //makes a new file node in each record
	if(file_head==NULL)
		return -3;

	file_head->next=NULL;
	current=file_head;
	current->next=file_head->next;
	new_rec->head=file_head; //initializing new records...
	new_rec->name=file_name;
	input=fopen(file_name,"rb");
	if ( input == NULL) //which means in case there's an error in opening the input file,program should terminate.
		return -2;


	available_space=avail_space( strg );
	fseek(input,0,SEEK_END);
	size=ftell(input);
	if((size+(int)strg->sector_size)>available_space) //if there's not enough space available
		return -4;
	fseek(input,0,SEEK_SET);
	
	flag=0;

	while(input=input) //this loop goes un until end of file is found.
	{
			new_file_node=strg->empty; //first we should get an empty node from the list
	     strg->empty=strg->empty->next;

          read=fread(new_file_node->head,sizeof(char),strg->sector_size,input);
		  count++;
          for(i=0;i<read;i++)
		{
	
			(new_file_node->head[i])=encryption_bit_operation(k,count,i,new_file_node->head);



		}
		  new_file_node->sector_size=read;
            current->next=new_file_node;
			new_file_node->next=NULL; //adding it to files
			current=current->next;
			
if (read<strg->sector_size)
	break;

	}
	new_rec->head=file_head;
	fclose(input);
	return (count+1);
}
/////////////////////////////////////////////
int encryption_bit_operation(int k,int count,int i,char *ptr)
{
    unsigned char q;
	int mod;
	q=ptr[i] ^ k;   //first each character should be XORed with the key.
	if ((k+count)%2)
	{

		mod=q%2; //gets the least important bit
		q=q>>1;
		*(ptr+i)=mod*MOVE+q;/*by *128 opperation,this least important bit turns to the most important one,there for after shifting 
							would have a right rotation*/
	}
	else
	{
		mod=q/MOVE; /*because mod is an integer,the quotiont would be assigned to mod */
		q=q<<1;
		*(ptr+i)=q+mod; /*again ,by saving the most important bit and moving it to the beginning,we would have a left rotation*/
	}
	return (*(ptr+i)) ;
}



//////////////////////////////////
int retrieve_file( Storage *strg, char* file_name, unsigned char key){
	record* current_rec;
	Sector* curr_file;
	int flag2=FALSE;
	FILE *output;
	int i,j,mod,flag=0;
	int num;
	unsigned char q,k;
	 char *ptr;
	int flag_finish=0;
	int count=START_POINT;
	k=key;

	if(strg==NULL|| strlen(file_name)>255||strg->rec==NULL || strg->sector_size==NULL) //invalid data
		return -1;
	for(current_rec=strg->rec->next;current_rec!=NULL;current_rec=current_rec->next)
	{

		if(strcmp(current_rec->name,file_name) == 0) //if the file with the name specified isnt found
		{  flag2=TRUE;
		break;
		}
	}
	if (flag2==FALSE)
		return -4;
	curr_file=current_rec->head->next; //head is for the dummy one
	if(curr_file==NULL)   ////yani ja nabude vaqti store karde
		return -3;
	output= fopen(file_name,"wb");
	if(output==NULL) 
		return-2;

	while(strg->sector_size==strg->sector_size) //a true while!!
	{

		count ++;
		for(i=0;i<curr_file->sector_size;i++)
		{
			if(curr_file==NULL)
			{   //j=i;
				flag=TRUE;

			}
		
			j=decryption_bit_operation(k,count,i,curr_file->head);  
			

		}
		fwrite(curr_file->head,sizeof(char),curr_file->sector_size,output);
		
		if(flag==TRUE)
			break;
		curr_file=curr_file->next;
		if(curr_file==NULL)
			break;
	}
	fclose(output);
	return (count+1)*strg->sector_size ;
}
int decryption_bit_operation(int k,int count,int i, char *ptr)
{
	unsigned char q;
	int temp,mod;

	q=ptr[i]; /*for decryption,first the encrypted fom should be rotated to diverse orientation and the xored with the key again*/
	temp=q+256; 
	if((temp>127) && (temp<256))
		q=temp; /*in the ASCII tabel we just have characters up to 127 defined,if the number passes 127,the system automatically subtracts 256
				from it and print the result,so we should check if the code we have,after add 256 to it is a valid ASCII,then this would be
				the result we wanted*/
	if (((k+count)%2)==0)
	{

		mod=q%2;
		q=q>>1;
		ptr[i]=mod*MOVE+q;

	}
	else
	{
		mod=q/MOVE;
		q=q<<1;
		ptr[i]=q+mod;
	}
	ptr[i]=ptr[i] ^ k;
	return 0 ;
}
int print_file_sequence( Storage *strg, char* file_name) {
	record *current_rec;
	Sector* curr_file;
	int flag=FALSE;
	if(strg==NULL || strlen(file_name)>255|| strg->rec==NULL) //checking for validity
		return -1;
	for(current_rec=strg->rec->next;current_rec!=NULL;current_rec=current_rec->next)
	{
		if(current_rec->name==file_name)
		{
			flag=TRUE;
			break;
		}
	}
	if(flag==FALSE)
		return -2;
	for(curr_file=current_rec->head->next;curr_file!=NULL;curr_file=curr_file->next) //the number of each file of the list should be printed
	{
		printf("%d ",curr_file->number);
	}
	printf("\n");
	return 0;
}
int delete_file( Storage *strg, char* file_name)
{
	record *current_rec;
	Sector *curr_file;
	Sector *head;
	Sector* temp;
	int flag=FALSE;
	int count=0,i;
	if(strg==NULL||strlen(file_name)>255)
		return -1;
	if(strg->rec==NULL)
		return -2;
	for(current_rec=strg->rec->next;current_rec!=NULL;current_rec=current_rec->next)
	{

		if(strcmp(current_rec->name,file_name) == 0) 
		{  flag=TRUE;
		break;
		}
	}
	if (flag==FALSE) //in case the file doesnt exit
		return -2;

	head=current_rec->head;
	for(curr_file=current_rec->head;curr_file!=NULL;curr_file=curr_file->next)
		count++;
	for(i=0;i<count;i++)
	{
		temp=head;  //gets them from full files
		head=head->next;
		temp->next=strg->empty; //stores them in the empty-linked list
		strg->empty=temp;
	}

	free(curr_file);
	free(current_rec);
	return count;
}
int avail_space( Storage* strg )
{ 
	Sector* current;
	int count=0;
	if(strg==NULL)
		return -1;

	for(current=strg->empty->next;current!=NULL;current=current->next) //counts the empty nodes,without coounting the dummy tail !
		count++;
	return count*(int)strg->sector_size;
}
int list_dir( Storage* strg, char* path)
{ 
	record* current;

	if(strg==NULL||path!=NULL)
		return -1;
	if(strg->rec==NULL)
		return -1;
	for(current=strg->rec->next;current!=NULL;current=current->next) //it should just print the record name
	{
		printf("file name:%s \n",current->name);
	}
	return 0;
}
void free_storage( Storage *strg){
	Sector* del_it;
	record *Del;
	Sector* DEL;
	record* dlt;
	if( strg==NULL || strg->empty==NULL || strg->rec==NULL )
		return ;
	while(strg->empty!=NULL)
	{
		del_it=strg->empty; //frees each node of the empty linked list
		strg->empty=strg->empty->next;
		free(del_it); 
	}
	dlt=strg->rec;//just for starting from the nest...
	strg->rec=strg->rec->next;
	free(dlt);
	while(strg->rec!=NULL)
	{  

		while(strg->rec->head!=NULL)
		{
			Sector* tmp = strg->rec->head->next; //frees each file of each record
			free(strg->rec->head);
			strg->rec->head = tmp;
		}

		Del=strg->rec; //frees each record which the content has been deleted
		strg->rec=strg->rec->next;
		free(Del);
	}
	free (strg->ptr);
}
int dump_storage( Storage *strg, char* dname) {
	record *curr_rec;
	Sector *curr_file;
	Sector* curr_sector;
	FILE *output;
	if(strg==NULL||strlen(dname)>255) //validity check
		return -1;
	output=fopen(dname,"wb");
	if(output==NULL)
		return -1;
	if(strg->rec==NULL || strg->empty==NULL)
		return -1;
	
	fwrite(strg->ptr,sizeof(char),strg->disk_size,output);
	fclose(output);
	return 0;
}
