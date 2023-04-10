
#include <stdio.h>
#include <unistd.h>

void convert(char* s){
	for(int i = 0; i < strlen(s); i ++) {
		if (s[i] >= 'A' && s[i] <= 'Z') {
            s[i] = s[i] - 'A' + 'a';
        }
        else if (s[i] >= 'a' && s[i] <= 'z') {
            s[i] = s[i] - 'a' + 'A';
        }
	}  
}

int main(int argc, char const *argv[])
{
    char * message = argv[1];
	
	int p[2], pid;

	if (pipe(p) < 0)
		exit(0);

	/* continued */
	if ((pid = fork()) > 0) {
		write(p[1], message, strlen(message));
		wait(NULL);
	}

	else {
        char buffer[1024];
        read(p[0],buffer,1024);
        convert(buffer);
        printf("%s\n",buffer);
        exit(0);
		
	}
	return 0;
}
