/*
 * rwedoffHW4P7.c
 * Author: Ryan Wedoff
 *
 * This program simulates a paging scheme using the aging algorithm.  Please compile with -std=c99
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef int bool;
#define true 1
#define false 0


typedef struct Pages{
    long age;
    int pageNum;
    bool fault;
}Page;
void agingAlgor(int numPages, int bits, int *list, int listLength);
void printList(Page list[], int length);


void agingAlgor(int numPages, int bits, int *list, int listLength) {
    /*
     * Initalizes the list the simulates the aging algorithm
     */
    //Create
    // array of pages
    Page pageList[numPages];
    //Result = 2^n
    long result = 1;
    int base = 2;
    while (bits)
    {
        if (bits & 1)
            result *= base;
        bits >>= 1;
        base *= base;
    }

    //Initalization of pages
    int pIndex = 0, lIndex = 0;
    while(pIndex < numPages){
        bool pageIn = false;
        if(lIndex > numPages){
            printf("Page is already in list\n");

        }
        //Checks if the page is already in the list
        for(int k = 0; k < pIndex; k++){
            if(pageList[k].pageNum == list[lIndex]){
                pageIn = true;
            }
        }
        //Doesn't allow a page that is already in array to be added to the list again
        if(pageIn){
            lIndex++;
        } else{
            pageList[pIndex].age = result;
            pageList[pIndex].pageNum = list[lIndex];
            pageList[pIndex].fault = false;
            pIndex++;
            lIndex++;
        }
    }

    //Print off inital list
        printf("\n");
        printList(pageList,numPages);

    //Cycle through the list, and simluate aging algorithm
    bool inList = false;
    int i = lIndex;
    int temp = 0;

    while(i < listLength){
        //Checks to see if the page is in the list
        for(int k = 0; k < numPages; k++){
            if(list[i] == pageList[k].pageNum){
                temp = k;
                inList = true;
            }
        }

        if(inList){
            //Ages all of the pages except the page that was referenced
            for(int j = 0 ; j < numPages; j++){
                if(j != temp){
                    pageList[j].age = pageList[j].age / 2;
                }
            }
            printList(pageList, numPages);
        } else{
            //Ages all pages
            for(int j = 0 ; j < numPages; j++){
                    pageList[j].age = pageList[j].age / 2;
            }
            //Selects the min page to be removed
            long min = result+1;
            int pageListIndex = 0;
            for(int a = 0; a < numPages; a++){
                if(pageList[a].age <= min){
                    min = pageList[a].age;
                    pageListIndex = a;
                }
            }
            pageList[pageListIndex].fault = true;

            printList(pageList, numPages);

            //Replaces the min aged page
            pageList[pageListIndex].pageNum = list[i];
            pageList[pageListIndex].age = result;
            pageList[pageListIndex].fault = false;

        }
        inList = false;
        i++;
    }
}

void printList(Page pageList[], int length){
    for(int i = 0; i < length; i++){
        printf("mem %d %lu\t", pageList[i].pageNum, pageList[i].age);
        printf("\tfault %d\n", pageList[i].fault);
    }
    printf("\n\n");
}



int main(int argc, char *argv[]) {
    if (argc == 4) {
        //printf("%s,%s\n", argv[1], argv[2]);
        int bitLength = atoi(argv[1]);
        int numPages = atoi(argv[2]);
        printf("Bits allow: %d\n", bitLength);
        printf("Pages Allowed %d\n", numPages);
        FILE * fp;
        char * line = NULL;
        size_t len = 0;
        fp = fopen(argv[3], "r");
        //Checks if file is valid
        if (fp == NULL){
            printf("File not found");
            exit(EXIT_FAILURE);
        }
        //Reads file line
        getline(&line, &len, fp);
        fclose(fp);
        const char delims[] = ",";
        const char *s = line;
        int *list = malloc(sizeof(int));
        int counter = 0;
        do {
            char buf [10];
            size_t field_len = strcspn(s, delims);
            sprintf(buf,"%.*s", (int)field_len, s);
            s += field_len;
            list[counter] = atoi(buf);
            counter++;
        } while (*s++);

        if (line)
            free(line);

        agingAlgor(numPages, bitLength,list, counter);
    }
    return 0;
}