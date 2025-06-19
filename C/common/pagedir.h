/* 
 * pagedir.h - header file for CS50 'pagedir' module
 * 
 * This module initializes and saves pages to a 
 * specified page directory
 *
 * Arthur Ufongene, May 2025
 */

#include "webpage.h"

/*********** pagedir_init ***********/
/* Initialize the specified page directory
 * Opens a .crawler file in the specified 
 * Directory and return true if successful and false if not
 * 
 * Caller provides:
 *   A valid char* pointer representing directory
 * We return:
 *   True or false depending on successful initialization
 * 
*/
bool pagedir_init(const char* pageDirectory);

/**************** pagedir_save ****************/
/* Save the given webpage to the page directory
 *
 * Caller provides:
 *   valid pointer to page, page directory string, and docID
 * We do:
 *   Save the webpage at the location pageDirectory/docID
 *   With the webpage URL on the first line, docID
 *   on the second line, and HTML in the rest of the 
 *   document
 * 
 */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);

/**************** pagedir_load ****************/
/* Loads a webpage from a saved file in the page directory
 *
 * Caller provides:
 *   Pathname to page directory and an integer docID
 * We return:
 *   If the file pageDirectory/docID is readable;
 *   A webpage that has the same URL, depth, and HTML attributes
 *   as the saved originial.
 *   else; nothing
 * Note:
 *   We assume that the file is in the format: URL first line,
 *   depth second line, HTML subsequent lines.
 */
webpage_t* pagedir_load(const char* pageDirectory, const int docID);

/*********** pagedir_validate ***********/
/* Validates that a given directory is a crawler directory
 * checks if there is a a .crawler file in the specified 
 * directory
 * 
 * Caller provides:
 *   A valid char* pointer representing directory
 * We return:
 *   True or false depending on successful if directory has crawler file
 * 
*/
bool pagedir_validate(const char* pageDirectory);

/*********** pagedir_validateWriteFile ***********/
/* Validates that a given file can be written to
 * Checks if the file can be opened in write mode
 * 
 * Caller provides:
 *   A valid string representing the file path
 * We return:
 *   True or false depending on successful if the file
 *   can be opened in write mode
 */
bool pagedir_validateWriteFile(const char* filePath);


/************* pagedir_getURL ****************/
/* Retrieves the URL from the first line of a page in a page directory
 * 
 * Caller provides:
 *   Page directory and doc ID
 * We return:
 *   A URL containing the string
 * Caller is responsible for:
 *   Later freeing the string
 */
char* pagedir_getURL(const char* pageDirectory, int id);

/*********** pagedir_validateReadFile ***********/
/* Validates that a given file can be read
 * Checks if the file can be opened in read mode
 * 
 * Caller provides:
 *   A valid string representing the file path
 * We return:
 *   True or false depending on successful if the file
 *   can be opened in read mode
 */
bool pagedir_validateReadFile(const char* filePath);
