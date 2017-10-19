

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<time.h>
#include<dirent.h>

int main(int args,char *argv[],char *envp[])
{
	DIR *direct;
	struct dirent *rdir;
	struct stat buffer;
	char mtimes[50];
	char ctimes[50];
	char filename[100];
	long int filsiz;
//	fprintf(stderr,"Argument %s\n",argv[1]);
	direct = opendir(argv[1]);				//opening the directory.
	if(direct == NULL)
	 {
		perror("Open directory failed");
		exit(1);
	 }

	fprintf(stderr,"st_mode\t\tUser id\tsize\tFilename\t\tst_ctime\n");
	while((rdir=readdir(direct))!=NULL)			//reading te directory.
	{
	strcpy(filename,rdir->d_name);
	if((stat(filename,&buffer))<0)				//fecthing the data from directory using stat function.
		perror("Unable to fetch the details of the file");

	strcpy(ctimes,ctime(&buffer.st_ctime));
//	strcpy(mtimes,ctime(&buffer.st_mtime));
	filsiz = buffer.st_size;
//	fprintf(stderr,"%o\t",buffer.st_mode);
	fprintf(stderr,(S_ISDIR(buffer.st_mode)) ? "d":"-");
	fprintf(stderr,(buffer.st_mode & S_IRUSR) ? "r":"-");
	fprintf(stderr,(buffer.st_mode & S_IWUSR) ? "w":"-");
	fprintf(stderr,(buffer.st_mode & S_IXUSR) ? "x":"-");
	fprintf(stderr,(buffer.st_mode & S_IRGRP) ? "r":"-");
	fprintf(stderr,(buffer.st_mode & S_IWGRP) ? "w":"-");
	fprintf(stderr,(buffer.st_mode & S_IXGRP) ? "x":"-");
	fprintf(stderr,(buffer.st_mode & S_IROTH) ? "r":"-");
	fprintf(stderr,(buffer.st_mode & S_IWOTH) ? "w":"-");
	fprintf(stderr,(buffer.st_mode & S_IXOTH) ? "x":"-");
	fprintf(stderr,"\t");

	fprintf(stderr,"%o\t",buffer.st_uid);
	fprintf(stderr,"%ld\t",filsiz);
	fprintf(stderr,"%s\t\t",filename);
	fprintf(stderr,"%s",ctimes);
//	fprintf(stderr,"%s",mtimes);
	}
return 0;
}
