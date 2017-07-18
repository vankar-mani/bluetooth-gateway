#include<stdio.h>
#include<mysql/mysql.h>
#include<stdlib.h>

void location(void);
void intime(void);
void outtime(void);
void effec_time(void);
void no_employs(void);



int main(void)
{
	int choice;
	printf("Enter your option:\n\t1. last/current location of employee by name\n\t2. Intime of employee by name\n\t 3. Outtime of employee by name\n\t4. Effective working time of employee in minutes\n\t5. Number of employee's present at gateway\n");
	scanf("%d",&choice);

	switch(choice){
 		
		case 1:
			location();
			break;
		case 2:
			intime();
			break;
		case 3:
			outtime();
			break;
		case 4:
			effec_time();
			break;
		case 5:
			no_employs();
			break;
                default:
			printf("invalid Query \n");


	}
	return 0;
}
