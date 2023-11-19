/*Cameron Atkins
 *CompSci 2750-02
 *Project 04 Due Monday, November 21, 2022 by 11:59 PM
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

//key to be used by parent and child
#define SHMKEY1 63108
#define SHMKEY2 63112

//allocate enough memory for 2 integers
//#define BUFF_SZ 2 * sizeof (int)

void help(){
	printf("This program takes in 2 inputs\n");
	printf("A filename, followed by a series of numbers\n");
	printf("If 2 of the numbers sum to 19, 2 of those numbers will be written to shared memory\n");
}

int main(int argc, char* argv[]){
	
	//Check if user entered -h in the argument list
	int option;
  	while ((option = getopt(argc, argv, "h")) != -1) {
   		switch(option) {
    			case 'h':
     			help();
			exit(0);
			break;
			default:
			help();
			break;
			exit(0);	
		}//end of switch
	}//end of while statement
	
	
	#define BUFF_SZ sizeof (int)
	
	//allocation of the memory for the 2 integers
	int shmid01 = shmget (SHMKEY1, BUFF_SZ, 0777 | IPC_CREAT);
	if (shmid01 == -1){
		printf("Could not allocate memory");
		return EXIT_FAILURE;
	}
	
	int shmid02 = shmget (SHMKEY2, BUFF_SZ, 0777 | IPC_CREAT);
	if (shmid02 == -1){
		printf("Could not allocate memory");
		return EXIT_FAILURE;
	}

	
	char * paddr01 = (char *) (shmat (shmid01, 0, 0));
	int * pointer1 = (int *) (paddr01);

	char *paddr02 = (char *) (shmat (shmid02, 0, 0));
	int * pointer2 = (int *) (paddr02);
	
	//The 2 shared memory locations are set to -2
	*pointer1 = -2;
	*pointer2 = -2;
	
	pid_t childPid = fork();

	if (childPid == 0){
		//pointer for dynamically created array	
		int * nums = 0;
		int i, j, x, y, z = 19;
		
		//allocates enough memory for an array of integers
		nums = malloc(sizeof(int) * (argc-2));		
		
		//Array size will be determined by the number of argumnets minus 2
		int size = (argc - 2);

		//Convert the array of characters into integers
		for (i = 0; i < size; i++)
			nums[i] = atoi(argv[i+2]);

		//Test if any 2 integers sum to 19
		//If 2 integers are found to sum 19, those 2 integers are written to shared memory
		//Else, -1 and -1 are written to shared memory
		
		for(x = 0; x < (size-1); x++){
			for (y = (x+1); y < size; y++){
				//if 2 numbers are found to sum to 19, they are written to shared memory
				if(nums[x] + nums[y] == z){
					*pointer1 = nums[x];
					*pointer2 = nums[y];
				//	break;	
				} //end of if statement
				
			//If no 2 integers are found to sum to 19, -1 is written to shared memory
				if(*pointer1 == -2 && *pointer2 == -2){
					*pointer1 = -1;
					*pointer2 = -1;
				}  
			} // end of inner for-loop
		} // end of outer for-loop
				
		//free the dynamically allocated memory
		free(nums);
		nums = 0;
	
	}//end of child


	//beginning of parent process     
	else{
		int stat;
		
		wait(&stat);
		
		//Prints 2 integers if they are found to sum to 19.
		if (*pointer1 != -1 && *pointer2 != -1){
			printf("%s %d %d", "Pair found by child: ", *pointer1, *pointer2);
			printf("\n");
		}
		
		//Prints message if no 2 digits sum to 19
		else{
			printf("No 2 digits entered sum to 19\n");
		}
	
	
	}//end of parent process
	
	//detach from shared memory
	shmdt(pointer1);
	shmdt(pointer2);
	
	//delete shared memory
	shmctl(shmid01, IPC_RMID, NULL);
	shmctl(shmid02, IPC_RMID, NULL);
	return EXIT_SUCCESS;



}//end of main 
