


int find_user(char *user_input)
{
    FILE *fp;
    char ch;
    char user[256];
    int flag=0;
    fp=fopen("users.ini","r");
    if(fp==NULL)
    {
      fl_alert(" file users.ini not found");
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
	  break;
	 }

	  user[i++]=ch;
	}
	if(!strcmp(user,user_input))
	{
	  fclose(fp);
	  return 1;
    }
	else
	  flag=0;
    }
		fclose(fp);
       return flag;

}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int add_user(char *user_input)
{
    FILE *fp;
     int num=0;
    fp=fopen("users.ini","a");

    if(fp==NULL)
    {
      fl_alert(" file users.ini not found");
      return 0;
    }
    fseek(fp,0,SEEK_END);
    fputs("\n",fp);
    fputs(user_input,fp);
    fclose(fp);

    fp=fopen("data.ini","r+");
    if(fp==NULL)
    {
      fl_alert(" file data.ini not found");
      return 0;
    }

    fscanf(fp,"%d",&num);
    num++;
    fclose(fp);
    fp=fopen("data.ini","w");
    fprintf(fp,"%d",num);
    fclose(fp);

    return 1;

}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int next_user(char *user_output,int num)
{
    FILE *fp;
    char ch;
    char user[256];
    int flag=0;
    fp=fopen("users.ini","r");
    if(fp==NULL)
    {
      fl_alert(" file users.ini not found");
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
////////////////////////////////////////////////////////////////////////////////
int find_user_no()
{
    fp=fopen("data.ini","r+");
    if(fp==NULL)
    {
      fl_alert(" file data.ini not found");
      return 0;
    }

    fscanf(fp,"%d",&num);
    return num;
}
