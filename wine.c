#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/wait.h>

int user_sharedwine=0,a;

int arg_install=0,arg_name=0,args_left;

void launch_wine(char *argv[], char *envp[])
{
  execve("/usr/bin/wine.orig",argv,envp);
  printf("Could not launch wine....?\n");
  exit(-1);
}

int main (int argc, char *argv[], char *envp[])
{
  char *prefix=getenv("WINEPREFIX");
  if (prefix==0 || strncmp(prefix,"/sharedwine/",12)!=0) launch_wine(argv,envp);
  
  char name[2000];
  strcpy(name,&prefix[12]);
  unsigned int l=strlen(name);
  if (l>=2000) launch_wine(argv,envp); 
  if (l>0 && name[l-1]=='/') name[--l]=0;
  if (l==0) launch_wine(argv,envp);
  while (l--) if (name[l]=='/') launch_wine(argv,envp); 
  
  fprintf(stderr,"shared wine %s\n",name);
  
  if (getuid()==0 || geteuid()==0)
  {
    fprintf(stderr,"sharedwine cannot be run as root\n");
    exit(-1);    
  }
   
  struct passwd *p=getpwuid(geteuid());
  if (p!=0 && strcmp(p->pw_name,"sharedwine")==0) user_sharedwine=1;
 
  char shareddir[2000],userdir[2000],tmp[2000];
   
  sprintf(shareddir,"/sharedwine/%s",name);
  sprintf(userdir,"%s/.local/share/sharedwine/%s",getenv("HOME"),name);

  
  
  if (user_sharedwine==1)
  {
  fprintf(stderr,"running as sharedwine\n");
    pid_t cpid, w;
    cpid = fork();
    if (cpid == -1)
    {
      perror("error fork");
      exit(EXIT_FAILURE);
    }

    if (cpid != 0)
    {                    /* Code executed by parent */
	int status;
	w = waitpid(cpid,&status,0); //, &status, WUNTRACED | WCONTINUED);
	system("chmod a+rx -R /sharedwine/.local /sharedwine/.config");
	if (w == -1)
	{
	  perror("waitpid");
	  exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
    }
  }
  
  
  if (user_sharedwine==1)
      setenv("WINEPREFIX",shareddir,1);
  else
      setenv("WINEPREFIX",userdir,1);

  
  DIR *dirp = opendir(shareddir);
  if ( dirp == NULL)
  {
    if (user_sharedwine==1)
    {
//      setenv("XDG_CONFIG_HOME","/sharedwine/config",1);
//      setenv("XDG_DATA_HOME","/sharedwine/share",1);
      
//      system("wine.orig regedit /sharedwine/path.reg");
      
//      system("wineserver -w");
 //     sprintf(tmp,"mv \"%s/drive_c/windows/temp\" \"%s/drive_c/\"",shareddir,shareddir);
  //    system(tmp);
     // sprintf(tmp,"rm \"%s/user.reg\"",shareddir);
  //    system(tmp);
    }
    else
    {
      printf ("dir '%s' does not exsist\n",shareddir);
      exit(-1);
    }
  }
  else
  {
    closedir(dirp);
  }
  
  if (user_sharedwine==0)
  {
    sprintf(tmp,"mkdir -p \"%s/.local/share/sharedwine\"",getenv("HOME"));
    system(tmp);

    if ( (dirp = opendir(userdir)) == NULL)
    {
      system("wineboot");
      system("wineserver -w");
//      sprintf(tmp,"rm \"%s/user.reg\"",userdir);
 //     system(tmp);

        sprintf(tmp,"mv \"%s/drive_c/users\" \"%s/\" && rm -R \"%s/drive_c\" \"%s/users/Public\" && mkdir \"%s/drive_c\" && mv \"%s/users\" \"%s/drive_c\"",userdir,userdir,userdir,userdir,userdir,userdir,userdir);
      system(tmp);

      
	
//          sprintf(tmp,"rm -R \"%s/drive_c/Program\" \"%s/drive_c/Program Files\" \"%s/drive_c/Program Files (x86)\" \"%s/drive_c/windows\" \"%s/drive_c/users/Public\"",userdir,userdir,userdir,userdir,userdir);
//      system(tmp);
     
//      sprintf(tmp,"mkdir \"%s/drive_c/temp\"",userdir);
//      system(tmp);
    }

    sprintf(tmp,"ln -sf \"%s/drive_c/\"* \"%s/drive_c\"",shareddir,userdir);
    system(tmp);
    sprintf(tmp,"ln -sf \"%s/drive_c/users/\"* \"%s/drive_c/users\"",shareddir,userdir);
    system(tmp);
    sprintf(tmp,"ln -sf \"%s/system.reg\" \"%s/userdef.reg\" \"%s\"",shareddir,shareddir,userdir);
    system(tmp);
  }

  launch_wine(argv,envp);
}
