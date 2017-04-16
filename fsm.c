
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

// define fsm parser with structs:

struct fsm;
struct stateList;
struct transNode;

typedef struct fsm {
	struct stateList *state;
	struct transNode *trans;
} fsm; 

typedef struct stateList {
	char *start;
	char *end;
} stateList;

typedef struct transNode {
	char *key;
	struct transNode *val;
	struct transNode *next;
	struct transNode *next_state;	
} transNode;


stateList* create_stateList() {
	stateList *ret = (stateList*) malloc(sizeof(stateList));
	ret->start = "";
	ret->end = "";
	return ret;
}

transNode* create_transNode() {
	transNode *ret = (transNode*) malloc(sizeof(transNode));
	ret->key = "";
	ret->val = NULL;
	ret->next = NULL;
	return ret; 
}

// a function to create (allocate an fsm, with above structs)

fsm* create_fsm() {
	fsm *ret = (fsm*) malloc(sizeof(fsm));
	ret->state = create_stateList();
	ret->trans = create_transNode();
	return ret;
}

// a function to remove spaces and tabs

void rmSp(char* source)
{
  char* i = source;
  char* j = source;
  while(*j != 0)
  {
    *i = *j++;
    if(*i != ' ' && *i != '\t')
      i++;
  }
  *i = 0;
}

// a function to remove newlines

void rmNl(char* source)
{
  char* i = source;
  char* j = source;
  while(*j != 0)
  {
    *i = *j++;
    if(*i != '\n')
      i++;
  }
  *i = 0;
}

int main(int argc, char **argv) {

if(argc != 3) {
	printf("Incorrect number of args! \n Usage: fsmfilefile, input_string\n");
	exit(0);
}

int i;
int bytes_read = 0;
int count = 0;
int count2 = 0;
int count3= 0;

size_t size = 1023;

bool first;
bool found;
bool loopedback= false;

char *input = argv[2];
char *buf = (char *) malloc(1024);
char *token;
char *token2;
char *token3;
char *token4;
char *helper;
char *transitions = (char *) malloc(sizeof(char) *1024);


FILE *fsm_file = fopen(argv[1], "r");
fsm *machine = create_fsm();

transNode *state1 = (transNode*) malloc(sizeof(transNode));
transNode *state2 = (transNode*) malloc(sizeof(transNode));



do {   // while not [transNodes], parse [states]

bytes_read = getline(&buf,&size,fsm_file);
while(bytes_read !=-1 && strlen(buf)==1){
	bytes_read = getline(&buf,&size,fsm_file);
}
	if(bytes_read == -1) {
		printf("Error in parsing string!\n");
		printf("buf = %s...\n",buf);
		exit(1);

	} else {
		if (count == 0) { //consume [states] aka do nothing;
		}

		rmSp(buf); // remove spaces
		rmNl(buf);
		if(count == 1) {

			token = strtok(strdup(buf), ":");
			machine->trans->key = strtok(NULL, ",");
			state1 = machine->trans;

			while((token4 = strtok(NULL, ",")) != NULL) {
				state1->next_state = (transNode*) malloc (sizeof(transNode));
				state1->next_state->next = NULL;
				state1->next_state->next_state = NULL;
				state1->next_state->val = NULL;


				if(first) {
					machine->trans = state1;  // establish hard link on first time thru
					first = false; 
				}

				state1->next_state->key = token4;
				assert(state1->next_state != NULL);
				state1 = state1->next_state;
				count2++;

			}

		state2 = machine->trans;
		while(state2 != NULL) {
			state2 = state2->next_state;
		}

		} else if (count == 2 ) {
			token = strtok(strdup(buf), ":");
			token = strtok(NULL, ":");
			machine->state->start = token;

		} else if (count == 3 ) {
			token = strtok(strdup(buf), ":");
			token = strtok(NULL, ":");
			machine->state->end = token;
		}
		count++;
	}

} while(strcmp(buf, "[transitions]"));


fread(transitions, 1, 1023, fsm_file);
rmSp(transitions);


first = true;

token = strtok_r(transitions, "\n", &helper);
state1 = machine->trans;
transNode *start = machine->trans;

// PARSE FSM FILE TRANSITIONS

do {
	char *helper2;
	char *helper3;
	token2 = strtok_r(strdup(token), ":", &helper2);  // stuff before colon (start, 1, etc)
	

	state2 = start;

	while((token2 = strtok_r(NULL, ",", &helper2)) != NULL ) {    // a given pair


		state1->next = (transNode*) malloc (sizeof(transNode));
		state1->next->next = NULL;
		state1->next->next_state = NULL;
		state1->next->val = NULL;

		token3 = strtok_r(strdup(token2), ":", &helper3);  // fst in pair
		state1->next->key = token3;


		token4 = strtok_r(NULL, ":", &helper3);    // snd in pair

		while(state2 != NULL && strcmp(state2->key,token4)) {
			state2 = state2->next_state;
		}

		state1->next->val = state2;

			state1 = state1->next;

	}

	state1 = start->next_state;
	start = start->next_state;

	token = strtok_r(NULL, "\n", &helper);

} while(token != NULL);

transNode *cur_state = machine->trans; 
char *end_state = machine->state->end;
transNode *inner;
transNode *state3;

// PARSE INPUT

for(i = 0; i < strlen(input); i++) {
	found = false; 
	char c = input[i];
	char d[2] = {c,'\0'};
	loopedback = false;

LOOP:

	if(cur_state != NULL) {
		if(loopedback) {
			inner = cur_state->next->val;
		}
		
		inner = cur_state;

		while(inner != NULL) {
			if(!(strcmp(inner->key, d))) {
				cur_state = inner->val;

				found = true;
				break;
			}
				inner = inner->next;
		}
		
		if(!found && !loopedback) {
		inner = cur_state->next;

			while(inner != NULL) {
				if(!(strcmp(inner->key, "*"))) {
					cur_state = inner->val;
					if(cur_state == NULL)
						cur_state = machine->trans;

					found = true;
					break;
				}

			inner = inner->next;


			}
			if(!found) {
				cur_state = machine->trans;
				loopedback = true;
				goto LOOP;
			}

		}
	}

	if(loopedback && inner != NULL) {

	if(!found) {
		printf("Error: Non-exhaustive transition function!\n");
		exit(-1);
	}
	
	}

}

if(!(strcmp(cur_state->key,end_state))) 
	printf("Accept.\n");

else printf("Reject. Ended in state %s\n", cur_state->key);


if(machine != NULL) free(machine);

if(transitions != NULL) free (transitions);
if(cur_state != NULL) free (cur_state);
if(inner != NULL) free (inner);

if(buf != NULL) free(buf);



exit(0);

}
