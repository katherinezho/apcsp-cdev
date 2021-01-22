//
// student.c
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "student.h"
#include "encrypt.h"


const char* STUFILE = "studentdata.txt";

// keeps track of the number of students in the students array
int numStudents = 0;

// holds the student pointer array
Student* students[100] = {0};


void createStudent(char* fname, char* lname, int age, int id)
{
  // createStudent dynamically creates a Student on the heap and adds that
  // student to the student array
  //  - the firstName and lastName strings should be dynamically created
  //    based on the size of the fname and lname args

  // Allocate space for first name.
  char *firstName   = (char *)malloc(strlen(fname));

  // Copy first name into allocated space.
  strcpy(firstName,fname);

  // Allocate space for last name.
  char *lastName   = (char *)malloc(strlen(lname));

  // Copy last name into allocated space
  strcpy(lastName, lname);


  // Allocate space for student data structure.
  Student *student = (Student *) malloc(sizeof(Student));

  // Assign student names.
  student->firstName = firstName;
  student->lastName  = lastName;
  student->age       = age;
  student->id        = id;

  // Put student in list of students.
  students[numStudents] = student;
  numStudents++;  // Increment student count.
}


void deleteStudent(Student* student)
{
  free(student->firstName);
  free(student->lastName);
  free(student);
}


void deleteStudents()
{
  for(int i=0; i < numStudents; i++){
    deleteStudent(students[i]);
  }

  numStudents = 0;
}


void saveStudents(int key)
{
  // save all students in the student array to a file 'studentdata.txt' overwriting
  // any existing file
  //   - the format of the file is one line per student as follows fname lname age id:
  //       tom thumb 15 1234 
  //       james dean 21 2345 
  //       katy jones 18 4532
  // the best way to do this is to convert the student data to a string using sprintf and then
  // (optionally) encrypt the whole string before writing it to disk (see cdemo/fileio3.c)
  FILE* fp;
  fp = fopen("studentdata.txt","w");
  char age[16];
  char id[32];
  
  if(fp){
    for(int i=0; i < numStudents; i++){
      caesarEncrypt(students[i]->firstName, key);
      caesarEncrypt(students[i]->lastName, key);
      sprintf(age, "%d", students[i]->age);
      caesarEncrypt(age,key);
      sprintf(id, "%ld", students[i]->id);
      caesarEncrypt(id, key);
      fprintf(fp, "%s %s %s %s \n", students[i]->firstName, students[i]->lastName, age, id);
      printf("Saving %s %s %s %s \n", students[i]->firstName, students[i]->lastName, age, id);
    }
    printf("Saved %d students\n", numStudents);
    fclose(fp);
  }
  else{
    printf("no file found\n");
  }
}


void loadStudents(int key)
{
  // load the students from the data file overwriting all exisiting students in memory
  //  - make sure you first deleteStudents before loading new ones
  //  - when loading it is best to load data into four strings and then (optionally) decrypt the
  //    strings
  //  - call createStudent to correctly create the students
  deleteStudents();
  FILE* fp;
  fp = fopen("studentdata.txt","r");

  if(fp){
    while (1){
       char fname[256]; 
       char lname[256];
       char age[256];
       char id[256];
       int  age_int;
       long id_long;

       if( fscanf(fp, "%s %s %s %s \n", fname, lname, age, id) == 4){
         caesarEncrypt(fname, -key);
         caesarEncrypt(lname, -key);
         caesarEncrypt(age, -key);
         caesarEncrypt(id, -key);
         sscanf(age, "%d", &age_int);
         sscanf(id, "%ld", &id_long);
	 createStudent(fname, lname, age_int, id_long);
       }
       else{
	 break;
       }
    }
           
    printf("Loaded %d students\n", numStudents);
    fclose(fp);
  }
  else {
    printf("no file found\n");
  }  
}


void printStudent(Student* student)
{
  printf("  Student: %s %s\n", student->firstName, student->lastName);
  printf("    age: %d\n", student->age);
  printf("    id: %ld\n", student->id); 
}


void printStudents()
{
  for (int i = 0; i < numStudents; i++) {
    printf("\n----- student %d ------\n", i);
    printStudent(students[i]);
  }
}




