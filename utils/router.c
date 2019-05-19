// pattern type path 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "router.h"
#define max 100

static struct Map *xmlan = NULL;
static int map_len;

void xml_ana()
{
	FILE *fp = NULL;
	//pattern type path
	char pattern[255];
	char type[255];
	char path[255];
	
	//open purpose file
	fp = fopen("./web.xml","r");
	
	if(fp == NULL){
		printf("Cannot open the file!\n");
		exit(0);
	}
	
	//read and test the file
	printf("pattern\ttype\tpath\n");
	int i = 0;
	while(!feof(fp)){
		
		fscanf(fp,"%s%s%s", pattern, type, path);
//		printf("%s\t%s\t%s\n",pattern,type,path);
		
		//save to struct
//		string s = pattern;
		strcpy(xmlan[i].pattern,pattern);
		strcpy(xmlan[i].type,type);
		strcpy(xmlan[i].path,path);
		i++;
	}
	map_len = i;
	//print
	int j;
	for(j = 0;j < i;j++){
		printf("%s\t%s\t%s\n",xmlan[j].pattern,xmlan[j].type,xmlan[j].path);
	}
	
	// printf("\n");
	fclose(fp);
}

const struct Map* getRoute(const char *uri){
	int i;
	if(xmlan == NULL)
	{
		xmlan = (struct Map*)malloc(sizeof(struct Map) * max);
		if(xmlan == NULL)
		{
			printf("getRoute: malloc error\n");
			exit(0);
		}
		xml_ana();
	}
	for(i = 0 ; i < map_len ; ++i) {
		if(strcmp(uri, xmlan[i].pattern) == 0) {
			printf("pattern: %s\n",xmlan[i].pattern);
			printf("type: %s\n",xmlan[i].type);
			printf("path: %s\n",xmlan[i].path);
			printf("\n");
			return &xmlan[i];
		}
	}
	return NULL;
}

// int main()
// {
// //	str xmlan[100];
// //   	FILE *fp = NULL;
// //   	char buff[255];
// //   	
// //	fp = fopen("C:\\a.txt", "rb");
   	
// //   fscanf(fp, "%s", buff);
// //   printf("1: %s\n", buff );
// //   
// //   fscanf(fp, "%s", buff);
// //   printf("2: %s\n", buff );
// //   
// //   fscanf(fp, "%s", buff);
// //   printf("3: %s\n", buff );
// //   fgets(buff, 255, (FILE*)fp);
// //   printf("2: %s\n", buff );
// //   
// //   fgets(buff, 255, (FILE*)fp);
// //   printf("3: %s\n", buff );

// 	//open and read
// 	//save to struct
// 	xml_ana();
// 	char uri[] = "/bsd";
// 	struct str * st = get_st(uri);
// 	if(st==NULL){
// 		printf("Error!");
// 		exit(0);
// 	}
// 	printf("pattern: %s\n",st->pattern);
// 	printf("type: %s\n",st->type);
// 	printf("path: %s\n",st->path);
	
// 	return 0;
 
// }
