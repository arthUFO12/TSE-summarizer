/*
 * pagedir.c - CS50 'pagedir' module
 *
 * See pagedir.h for more information
 *
 * Arthur Ufongene, May 2025
 */

#include <string.h>
#include "pagedir.h"
#include "mem.h"
#include "file.h"

/*********** pagedir_init ***********/
/* see pagedir.h for more information */
bool pagedir_init(const char* pageDirectory)
{
  char pathName[strlen(pageDirectory) + strlen(".crawler") + 2]; // create string buffer
  strcpy(pathName,pageDirectory);
  strcpy(pathName + strlen(pageDirectory) ,"/");                 // copy pageDirectory/.crawler to pathName
  strcpy(pathName+strlen(pageDirectory)+1, ".crawler");

  FILE* crawlerFile;
  if ((crawlerFile = fopen(pathName,"w")) == NULL) {             // try to open file
    fputs("Could not open crawler file", stderr);                // if unsuccessful, print error and return false
    return false;
  }
  fclose(crawlerFile);                                           // else, close file and return true
  return true;
}


/**************** pagedir_save ****************/
/* See pagedir.h for more information */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID)
{
  char docIDBuffer[10];
  sprintf(docIDBuffer, "%d", docID);               // print docID to buffer
  char pathName[strlen(pageDirectory) + strlen(docIDBuffer) + 2];
  
  strcpy(pathName, pageDirectory);
  strcpy(pathName + strlen(pageDirectory), "/");   // copy pageDirectory/docID to pathName
  strcpy(pathName + strlen(pageDirectory) + 1, docIDBuffer);
  

  FILE* docFile;
  if ((docFile = fopen(pathName,"w")) == NULL) {   // Try to open file, if unsuccessful, exit non-zero
    fputs("Could not open save file\n", stderr);
    exit(-1);
  }
  
  fprintf(docFile, "%s\n", webpage_getURL(page));  // print the URL on the first line
  fprintf(docFile, "%d\n", webpage_getDepth(page));                 // print the docID on the second line
  fprintf(docFile, "%s", webpage_getHTML(page));   // print the webpage HTML on subsequent lines
  
  fclose(docFile);
  return;
}

/**************** pagedir_validate ****************/
/* See pagedir.h for more information */
bool pagedir_validate(const char* pageDirectory)
{
  char pathName[strlen(pageDirectory) + strlen(".crawler") + 2]; // create string buffer
  strcpy(pathName,pageDirectory);
  strcpy(pathName + strlen(pageDirectory) ,"/");                 // copy pageDirectory/.crawler to pathName
  strcpy(pathName+strlen(pageDirectory)+1, ".crawler");

  FILE* crawlerFile;
  if ((crawlerFile = fopen(pathName,"r")) == NULL) {             // try to open file                 
    return false;                                                // return false
  }
  fclose(crawlerFile);                                           // else, close file and return true
  return true;
}

/*************** pagedir_validateWriteFile ***************/
/* see pagedir.h for more information */
bool pagedir_validateWriteFile(const char* filePath)
{
  FILE* writeFile;
  if ((writeFile = fopen(filePath,"w")) == NULL) {            // try to open file in w mode           
    return false;                                             // if unsuccessful return false
  }
  fclose(writeFile);
  return true;
}

/*************** pagedir_validateReadFile ***************/
/* see pagedir.h for more information */
bool pagedir_validateReadFile(const char* filePath)
{
  FILE* readFile;
  if ((readFile = fopen(filePath,"r")) == NULL) {            // try to open file in r mode           
    return false;                                             // if unsuccessful return false
  }
  fclose(readFile);
  return true;
}

/**************** pagedir_load ****************/
/* See pagedir.h for more information */
webpage_t* pagedir_load(const char* pageDirectory, const int docID)
{
  char docIDBuffer[10];
  sprintf(docIDBuffer, "%d", docID);               // print docID to buffer
  char pathName[strlen(pageDirectory) + strlen(docIDBuffer) + 2];
  
  strcpy(pathName, pageDirectory);
  strcpy(pathName + strlen(pageDirectory), "/");   // copy pageDirectory/docID to pathName
  strcpy(pathName + strlen(pageDirectory) + 1, docIDBuffer);

  FILE* docFile;
  if ((docFile = fopen(pathName,"r")) == NULL) {   // Try to open file, if unsuccessful, return null
    return NULL;
  }

  char* URL = file_readLine(docFile);              // read URL and depth from file

  int depth;
  fscanf(docFile, "%d\n", &depth);
  
  char* HTML = file_readFile(docFile);             // read the rest of the file which is HTML
  
  fclose(docFile);
  
  return webpage_new(URL,depth,HTML);              // return a new webpage with the URL, depth, and HTML

}

/************* pagedir_getURL ****************/
/* see pagedir.h for more info */
char* pagedir_getURL(const char* pageDirectory, int docID)
{
  char docIDBuffer[10];
  sprintf(docIDBuffer, "%d", docID);               // print docID to buffer
  char pathName[strlen(pageDirectory) + strlen(docIDBuffer) + 2];
  
  strcpy(pathName, pageDirectory);
  strcpy(pathName + strlen(pageDirectory), "/");   // copy pageDirectory/docID to pathName
  strcpy(pathName + strlen(pageDirectory) + 1, docIDBuffer);

  FILE* docFile;
  if ((docFile = fopen(pathName,"r")) == NULL) {   // Try to open file, if unsuccessful, return null
    return NULL;
  }

  char* URL = file_readLine(docFile);              // obtain URL from first line of file

  fclose(docFile);
  return URL;
}
