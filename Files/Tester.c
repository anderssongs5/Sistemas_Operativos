#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_CHARS_PER_LINE  50
#define NAME                "TESTER"
#define OUTPUT_NAME 		"Output.txt"

struct test {
    char *children;
    char *times;
    char *accounts;
    char *money;
    int loops;
};

char* programName;
char* fileName;
char* params;
char temp[20];
char b[80];

int extractParameters(int argc, char* argv[]);

int checkBalance(struct test t, float value);

void tokenValues(char *line, struct test *t);

void buildSystemCommands(struct test t);

int main(int argc, char* argv[]) {
    if (extractParameters(argc, argv) == 0) {
        fprintf(stdout, "%s: The params amount is not valid. Amount: %d.\n",
            NAME, argc);

        return (EXIT_FAILURE);
    }
    
    FILE* input = fopen(fileName, "r");
    FILE* output = fopen(OUTPUT_NAME, "a+");
    if (input == NULL) {
        fprintf(stdout, "%s: The file: \"%s\" can not be opened.\n", NAME,
            fileName);

        return (EXIT_FAILURE);
    }
    
    if (output == NULL) {
        fprintf(stdout, "%s: The file: \"%s\" can not be opened.\n", NAME,
            OUTPUT_NAME);

        return (EXIT_FAILURE);
    }

    char line[MAX_CHARS_PER_LINE];
    struct test t;
    int counter;
    int oe;
    float value;
    char text[20];
    while (fgets(line, MAX_CHARS_PER_LINE, input) != NULL) {
		tokenValues(line, &t);
             
        buildSystemCommands(t);
        params = b;
		
        for(counter = 1; counter <= t.loops; counter++) {
			if(system(params) == (-1)){
				fprintf(stderr, "%s: Error utilizing \'system\' call function\n",
						NAME);
			} else {
				fscanf(output, "%s %f", text, &value);
				if(checkBalance(t, value) == 1){
					fprintf(output, "\t%s\n", "OK");
				} else {
					fprintf(output, "\t%s\n", "WRONG");
				}
				
				fflush(output);
			}
			
			printf("\n\n");
		}
    }
	
	fprintf(stdout, "Finalizado.\n");
	
    fclose(input);
    fclose(output);

    return (EXIT_SUCCESS);
}

int extractParameters(int argc, char* argv[]) {
    if (argc == 3) {
		strcpy(temp, "./");
		strcat(temp, argv[1]);
        programName = temp;
        fileName = argv[2];
		
        return (1);
    }

    return (0);
}

void buildSystemCommands(struct test t){
	strcpy(b, programName);
	strcat(b, " ");
	strcat(b, t.children);
	strcat(b, " ");
	strcat(b, t.times);
	strcat(b, " ");
	strcat(b, t.accounts);
	strcat(b, " ");
	strcat(b, t.money);
}

void tokenValues(char* line, struct test *t) {
    char *token = strtok(line, " ");

	t->children = token;

    token = strtok(NULL, " ");
    t->times = token;

    token = strtok(NULL, " ");
    t->accounts = token;

    token = strtok(NULL, " ");
    t->money = token;

    token = strtok(NULL, " ");
    t->loops = atoi(token);
}

int checkBalance(struct test t, float value){
	int accs = atoi(t.accounts);
	float mon = atof(t.money);

	return ((accs * mon) == value);
}
