
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_UW 54 // maximum untranslated word length
#define MAX_TW 25 // maximum translated word length

typedef struct Node {
	char *str;
	struct Node *next;
} Node;

typedef struct LinkedList {
	Node *head;
	Node *tail;
} LinkedList;

typedef struct Tuple {
	int i; // index of occurrence
	char *us; // untranslated string;
	char *ts; // translated string;
} Tuple;


void addNode(LinkedList *LL, char* str) {
	Node *newNode = (Node*) malloc(sizeof(Node));
	LL->tail->next = newNode;
	LL->tail = newNode;
	newNode->next = NULL;
	newNode->str = (char*) malloc(MAX_UW * sizeof(char));
	strcpy(newNode->str, str);
}

LinkedList *fgetWords(char *dir) {
	FILE *fileIn = fopen(dir, "r");
	if (fileIn == NULL) {
		printf("COMP2510ERROR: Input File Not Found");
		exit(1);
	}


	// init LL with placeholder head
	LinkedList *newList = (LinkedList*) malloc(sizeof(LinkedList));
	newList->head = NULL;
	newList->tail = NULL;
	Node *newWord = (Node*) malloc(sizeof(Node));
	newList->head = newWord;
	newList->tail = newWord;


	char *curWord;
	curWord = (char*) malloc(MAX_UW + 5);
	int curC = fgetc(fileIn);
	while (fgetc(fileIn) != EOF) {
		fseek(fileIn, -1, SEEK_CUR);
		if (curC == ' ') {
			if (strlen(curWord) > 0) {
				if (strlen(curWord) > MAX_UW) {
					printf("COMP2510ERROR: Input word is too long.");
					exit(1);
				}
				addNode(newList, curWord);
				curWord = (char*) malloc(MAX_UW + 5);
			}

			curC = fgetc(fileIn);
			continue;
		}
		if (curC == '\n') {
			if (strlen(curWord) > 0) {
				if (strlen(curWord) > MAX_UW) {
					printf("COMP2510ERROR: Input word is too long.");
					exit(1);
				}
				addNode(newList, curWord);
				curWord = (char*) malloc(MAX_UW + 5);
			}

			addNode(newList, "\n");

			curC = fgetc(fileIn);
			continue;
		}

		strcat(curWord, (char*)&curC);
		curC = fgetc(fileIn);
	
	}
	if (strlen(curWord) > 0) {
		if (strlen(curWord) > MAX_UW) {
			printf("COMP2510ERROR: Input word is too long.");
			exit(1);
		}
		addNode(newList, curWord);
		curWord = (char*) malloc(MAX_UW + 5);
	}

	// remove placeholder head
	newList->head = newList->head->next;
	return newList;
}

int numOcc(const char *haystack, const char *needle){
    int count = 0;
    const char *tmp = haystack;
    while(tmp = strstr(tmp, needle))
    {
        count++;
        tmp++;
    }
    return count;
}

Tuple **occurrences(char *src, char *ustr, char* tstr, int numOcc) {
	Tuple **list = malloc((numOcc) * sizeof(Tuple*));
	Tuple *newTuple;


	int offset = 0;
	int i = 0;

	offset = strstr(src, ustr) - src;
	//printf("%d\n", offset);
	newTuple = (Tuple*) malloc(sizeof(Tuple));
	newTuple->i = offset;
	newTuple->us = ustr;
	newTuple->ts = tstr;
	list[i] = newTuple;


	for (i = 1; i < numOcc; i++) {
		offset = strstr(src + offset + 1, ustr) - src;
		//printf("%d\n", offset);
		newTuple = (Tuple*) malloc(sizeof(Tuple));
		newTuple->i = offset;
		newTuple->us = ustr;
		newTuple->ts = tstr;
		list[i] = newTuple;
	}

	return list;
}

char *transSingle(char* src, char* ustr, char* tstr, int diff) {
	int numOccurrences = numOcc(src, ustr);
	if (numOccurrences == 0) {
		return src;
	}
	//printf("numOcc: <%d>\n", numOccurrences);
	int newLen = numOccurrences * diff + strlen(src);
	char *new = malloc(newLen * sizeof(char));
	Tuple **occs = occurrences(src, ustr, tstr, numOccurrences);

	int i;
	strncat(new, src, occs[0]->i);
	for (i = 0; i < numOccurrences - 1; i++) {
		strcat(new, occs[i]->ts);
		int startIndex = occs[i]->i+strlen(occs[i]->us);
		int endIndex = (occs[i+1]->i-1);
		int run = endIndex - startIndex + 1;
		strncat(new, &src[startIndex], run);
	}
	strcat(new, occs[i]->ts);
	strncat(new, &src[occs[i]->i+strlen(occs[i]->us)], strlen(src) - occs[i]->i);
	/*
	*/

	i;
	for (i = 0; i < numOccurrences; i++) {
		//printf("%d\n", occs[i]->i);
	}
	//printf("\n\n");
	/*
	*/
	
	return new;
}

char *transWord(char* str) {
	char *new = transSingle(str, "er", "erre", 2);
	new = transSingle(new, "I", "Aei", 2);
	new = transSingle(new, ".", "[.]", 2);
	new = transSingle(new, "is", "iizz", 2);
	new = transSingle(new, "student", "worker", -1);
	new = transSingle(new, "ing", "eeng", 1);
	new = transSingle(new, "class", "ccamp", 0);
	new = transSingle(new, "skytrain", "sloth", -3);
	new = transSingle(new, "professor", "king", -5);
	new = transSingle(new, "friend", "stranger", 2);
	new = transSingle(new, "tion", "ertioner", 4);
	return new;

}

LinkedList *translateWords(LinkedList *src) {
	// init LL with placeholder head
	LinkedList *newList = (LinkedList*) malloc(sizeof(LinkedList));
	newList->head = NULL;
	newList->tail = NULL;
	Node *newWord = (Node*) malloc(sizeof(Node));
	newList->head = newWord;
	newList->tail = newWord;

	
	char *translatedWord;
	Node *curSrcNode = src->head;
	while (curSrcNode != NULL) {
		//printf("<%s>", curSrcNode->str);
		translatedWord = transWord(curSrcNode->str);
		addNode(newList, translatedWord);
		curSrcNode = curSrcNode->next;
	}



	// remove placeholder head
	newList->head = newList->head->next;
	return newList;
}

void writeToOutput(int spots, int spaces, Node *head, char *dir) {
  FILE *outputFile = fopen(dir, "a+");
  if (outputFile == NULL) {
    printf("COMP2510ERROR: Output File Not Found");
    exit(1);
  }

  /*
   * check if all words + spaces are greater than 25
  if (strlen(head->)) {
  
  }
  */

  fseek(outputFile, 0, SEEK_END);

  char *result = (char*) malloc(25);

  Node *curr = head;

  if (spots == 0) {

    fprintf(outputFile, "%s\n", head->str);

    fclose(outputFile);

    return;

  }

  int minSpaces = floor(spaces/spots);
  int additionalSpaces = spaces % spots;

  strcpy(result, curr->str);

  for (int i = 1; i <= spots; i++) {

    int spacesToAdd = (i <= additionalSpaces) ? minSpaces + 1 : minSpaces;

    for (int i = 0; i < spacesToAdd; i++) {

      strcat(result, " ");

    }

    strcat(result, curr->next->str);

    curr = curr->next;

  }

  fprintf(outputFile, "%s\n", result);

  fclose(outputFile);

}

void insertNewLine(Node *left, Node *right) {

  Node *newLine = (Node*) malloc(sizeof(Node));
  newLine->str= "\n";
  left->next = newLine;
  newLine->next = right;

}

void wrapText(LinkedList *ll, int maxLettersPerLine, char *dir) {

  Node *curr = ll->head;

 if (curr->next == NULL) {
	 if (strlen(curr->str) > maxLettersPerLine) {
	      printf("COMP2510ERROR: Ouput Word Length Exeeds 25 Characters");
	      exit(1);
	 }
    writeToOutput(0, 0, curr, dir);
 }

  while (curr->next != NULL) {
	 if (strlen(curr->next->str) > maxLettersPerLine || strlen(curr->str) > maxLettersPerLine) {
	      printf("COMP2510ERROR: Ouput Word Length Exeeds 25 Characters");
	      exit(1);
	 }

    if (curr->next->next == NULL) {
      break;
    }

    if (strcmp(curr->next->str, "\n") == 0) {
      curr = curr->next->next;
    }

    Node *lineHead = curr;

    int minSpaces = 0;

    int lineLength = strlen(curr->str);

    while (curr->next != NULL && strcmp(curr->next->str, "\n") != 0) {


      if (lineLength + strlen(curr->next->str) + minSpaces + 1 <= maxLettersPerLine) {

	lineLength += strlen(curr->next->str);
	curr = curr->next;
	minSpaces++;

      } else {

	insertNewLine(curr, curr->next);

	if (minSpaces == 0) {
	  lineHead = curr;
	}

      }

    }
    writeToOutput(minSpaces, maxLettersPerLine - lineLength, lineHead, dir);

  }

}

int main(int argc, char** argv) {
	/*
	char *inDir = "./sample_files/input1.txt";
	char *outDir = "./out1.txt";
	*/

	if (argv[1] == NULL || argv[2] == NULL) {
		printf("COMP2510ERROR: Incorrect Number Of Command-Line Arguments");
		exit(1);
	}
	char *inDir = argv[1];
	char *outDir = argv[2];
	
	// check and handle empty input file
	FILE *fileInn = fopen(inDir, "r");
	if (fileInn == NULL) {
		printf("COMP2510ERROR: Input File Not Found");
		exit(1);
	}
	fseek(fileInn, 0, SEEK_END);
	int size = ftell(fileInn);
	if (size == 0) {
		  FILE *outputFile = fopen(outDir, "w");
		  if (outputFile == NULL) {
		    printf("COMP2510ERROR: Output File Not Found");
		    exit(1);
		  }
		  return 1;
	}
	fseek(fileInn, 0, SEEK_SET);


	// reset file if it exists
	FILE *arbitrary = fopen(outDir, "w");
	fclose(arbitrary);
	//printf("%s", outDir);

	LinkedList *words = fgetWords(inDir);
	LinkedList *translated = translateWords(words);

	wrapText(translated, 25, outDir);
	

	Node *cur;

	/*
	cur = words->head;
	while (cur != NULL) {
		printf("<%s>", cur->str);
		cur = cur->next;
	}
	*/


	/*
	cur = translated->head;
	while (cur != NULL) {
		printf("<%s>", cur->str);
		cur = cur->next;
	}
	*/

	return 1;
}
