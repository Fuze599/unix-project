#include "utils.h"
#include "utils_v1.h"
#include <stdlib.h>

int livreComptes[1000] = {0};

int main(int argc, char **argv) {
	//if there is not all the args
	if(argc != 3){
		exit(1);
	}
	// convert args to integers
	int account = atoi(argv[1]);
	int sumMoney = atoi(argv[2]);
	livreComptes[account] += sumMoney;
	printf("%i\n", account);
	printf("%i\n", sumMoney);  
	printf("%i\n", livreComptes[account]);
}
