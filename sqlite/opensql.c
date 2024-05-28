#include <stdio.h>
#include <sqlite3.h>

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

	sqlite3_close(db);	// Closing the database
	
	printf("end.");
	return 0;
}
