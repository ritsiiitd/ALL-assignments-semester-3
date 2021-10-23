#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
/*Credits: https://www.geeksforgeeks.org/converting-strings-numbers-cc/
           https://www.geeksforgeeks.org/strtok-strtok_r-functions-c-examples/
           https://www.geeksforgeeks.org/input-output-system-calls-c-create-open-close-read-write/
           */
int main(int argc, char const *argv[])
{
    /* code */
    int pid=fork();
    
    if(pid==-1){//fork failed
        perror("fork");
        return EXIT_FAILURE;
    }

    
    //child process
    int f=open("student_record.csv",O_RDONLY); //open system call to open the file

    if(f==-1){
        perror("open");
        return EXIT_FAILURE;
    }

    char data[1024];//to store data of csv file as string
    
    int size=read(f,data,1024);//reading the file using 'read' systemcall
    if(size==-1){
        perror("read");
        return EXIT_FAILURE;
    }

    char *d=data;//d variable used for splitting

    int i=0;//to keep track of rows 
    
    int ass1=0,ass2=0,ass3=0,ass4=0,ass5=0,ass6=0;
    int count=0;

    char *one_row = strtok_r(d,"\n",&d);//picks up first row of the csv file, as rows are "newline" seperated
    char section='\0';
    while(one_row!=NULL){ //till we don't reach EOF
        
        // if(i!=1)
        // printf("%s\n",one_row);
    
        char *row = one_row;//used in strtok_r for splitting
        int j=0;//to keep track of columns of a row

        char *rd[8];//array to store contents of a row
    
     //now 'one_row' contains a row from csv file
     //splitting ',' to get each item of row

        char *one_column=strtok_r(row,",",&row);//first column
        
        while(one_column!=NULL){

            //printf("%s\n",one_column);
            char *oc=malloc(strlen(one_column)+1);//making a shallow copy to store in array of row
            strcpy(oc,one_column);

            rd[j]=one_column;// storing data in an array

            one_column=strtok_r(row,",",&row);//moving to next column
            j++;

        }
        
        if(pid==0){
            section='A';
        }
        else if(pid>0){
            section='B';
        }

        if(rd[1][0]==section && strlen(one_row)>1){//calculating sum of marks of assignments for a section 
            count++;
            ass1 += atoi(rd[2]);
            ass2 += atoi(rd[3]);
            ass3 += atoi(rd[4]);
            ass4 += atoi(rd[5]);
            ass5 += atoi(rd[6]);
            ass6 += atoi(rd[7]);    
        }
        
        one_row = strtok_r(d,"\n",&d);//moving to next row
        i++;
    }
    if(pid>0){
        int status;
        pid_t p=waitpid(pid,&status,0);
        if(p<-1){
            perror("wait");
            return EXIT_FAILURE;
        }
        printf("Child process exited with status [%d]",status);
    }
    
    printf("\nPrinting average marks for section %c :\n",section);
    printf("Number of students in section %c = %d\n",section,count);
    printf("Average marks of Assignment 1 %.7f\n",(ass1*1.0)/count);
    printf("Average marks of Assignment 2 %.7f\n",(ass2*1.0)/count);
    printf("Average marks of Assignment 3 %.7f\n",(ass3*1.0)/count);
    printf("Average marks of Assignment 4 %.7f\n",(ass4*1.0)/count);
    printf("Average marks of Assignment 5 %.7f\n",(ass5*1.0)/count);
    printf("Average marks of Assignment 6 %.7f\n",(ass6*1.0)/count);
    if(pid==0){
        printf("Child is exiting\n");
        exit(1);
    }
    return 0;
}