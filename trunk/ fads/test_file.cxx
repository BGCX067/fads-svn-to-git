#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>

int next_user(char *user_output,int num)
{
    FILE *fp;
    char ch;
    char user[256];
    int user_num=0;
    fp=fopen("users.ini","r");
    if(fp==NULL)
    {
      printf(" file users.ini not found");
    }
     
    while(ch!=EOF)
    {
       int i=0;
       while(1)
       {
    	ch=fgetc(fp);
    	if(ch=='\n' || ch==EOF)
    	{
    	  user[i++]='\0';
    	  user_num++;
    	  break;
    	 }
    
    	  user[i++]=ch;
    	}
    	if(user_num==num)
    	{
    	  strcpy(user_output,user);
    	  return 1;
        }
     }


}
////////////////////////////////////////////////////////////////////////////////
int find_user_no()
{
    FILE *fp;
    int num;
    fp=fopen("data.ini","r+");
    if(fp==NULL)
    {
      //fl_alert(" file data.ini not found");
      return 0;
    }

    fscanf(fp,"%d",&num);
    fclose(fp);
    return num;
}

////////////////////////////////////////////////////////////////////////////////
int main()
{
    int i,n;
	char user[256];
    clrscr();
    n=find_user_no();
	for(i=1;i<=n;i++)
	{
	  next_user(user,i);
	  printf("%s\n",user);
    } 
    getch();
    return 0;
}
