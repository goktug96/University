#include <stdio.h>
#include <string.h>

/* Part 1: You need to modify this function */
int get_uid() {
    char buffer[2];
    int * ret;
    ret = buffer + 10;  //2bytes for buffer + 8 bytes for ebp
    *ret += 8;  //jump
	return 0;
}

/****************************************
*
* DO NOT EDIT ANYTHING BELOW THIS LINE
*
****************************************/

/* Part 2: Find the best input for this function */
int IsPwOk(char * pw, int size_of_pw) {
    char password [8];
	memcpy(password, pw, size_of_pw/2);
	return 0 == strcmp(password, "1234");
}

/* This function converts hex string to ascii string
** Example: input=32334142 output=23AB
** Warning: Do not give any string that contains 00
*/
void hex_to_ascii(char * passInHex){
    char temp [3];
    char ch;
    int num;
    int i;
	for(i = 0; i < strlen(passInHex); i = i + 2) {
		temp[0] = passInHex[i];
		temp[1] = passInHex[i+1];
		temp[2] = '\0';
		sscanf(temp,"%x", &num);
		ch = num;
        passInHex[i/2] = ch;
    }
    passInHex[i/2] = '\0';
}

int main(int argc, char **argv) {
	int admin_priv = 0;
	int admin_pw_check = 0;
	int size_of_argv = strlen(argv[1]);
    char username [10];
    char root_name [] = "root";
    char guest_name [] = "guest";
    int default_uid = 1000;
    int uid = 23;
    uid = get_uid();
    uid = default_uid; // this line should not be executed
    if (uid == 0) {
		hex_to_ascii(argv[1]);
		printf("Logging in as Admin\n");
		strncpy(username, root_name, strlen(root_name));
		// IsPwOk should return 0 and the program should jump to else statement
		admin_pw_check = IsPwOk(argv[1], size_of_argv);
		if (!admin_pw_check) {
			printf("Admin password is not accepted\n");
		} else {
	      	printf("Admin password is accepted\n");
			admin_priv = 1;
        }
    } else {
      	printf("Logging in as Guest\n");
        strncpy(username, guest_name, strlen(guest_name));
        admin_priv = 0;
    }

	if(admin_priv == 0 && strncmp(username, guest_name, strlen(guest_name)) == 0) {
		printf("Logged in as Guest\n");
		printf("Both buffer overflows failed!\n");
	} else if(admin_priv == 0 && strncmp(username, guest_name, strlen(guest_name)) != 0) {
		printf("Logged in as Guest\n");
		printf("First buffer overflow succeeded, but second failed!\n");
	} else if(admin_priv == 1 && admin_pw_check == 1) {
		printf("Logged in as Admin, but using Admin Password\n");
		printf("First buffer overflow succeeded, but second failed!\n");
	} else if(admin_priv == 1 && admin_pw_check == 0) {
		printf("Logged in as Admin\n");
		printf("Both buffer overflows succeeded! YAY!\n");
 	} else {
		printf("Something went wrong!\n");
	}

        return 0;
}


