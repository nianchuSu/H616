#include <stdio.h>
#include <sqlite3.h>

int fun(void *user_data, int num_fields, char **field_values, char **field_names)
{
	int i;
	printf("%s\n",(char*)user_data);
	for(i = 0; i < num_fields; i++){
		printf("%s = %s\t",field_names[i],field_values[i]);
	}
	printf("\n");
	return 0;	// You must return 0, or you'll get an error
}

int main(int argc,char **argv)
{
	if(argc < 2){
		printf("Missing necessary parameters.\n");
		return -1;
	}
	int ret;
	sqlite3 *db;

	ret = sqlite3_open(argv[1], &db);
	if(ret != 0){
		printf("error:%s\n",sqlite3_errmsg(db));
		return -1;
	}
	
	printf("open %s successful.\n",argv[1]);

	char *message = NULL;
	ret = sqlite3_exec(db,"insert into Class01(name) values('wangwu',17);",fun,"data:",&message);
	printf("%d,%s\n",ret,message);
	if(ret != 0){
		printf("SQL error %s.\n",message);
		return -1;
	}
	


	ret = sqlite3_exec(db,"select * from Class01;",fun,"data:",&message);
	sqlite3_close(db);	// Closing the database
	
	printf("end.\n");
	return 0;
}
