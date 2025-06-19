/*
 * crawler.c - CS50 'crawler' module
 *
 * This module 'crawls' across interconnected webpages
 * starting from a given seed URL up to a given depth 
 * and saves the HTML of those webpages to a given directory
 * 
 * Arthur Ufongene, May 2025
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "webpage.h"
#include "mem.h"
#include "bag.h"
#include "hashtable.h"
#include "pagedir.h"

// Function prototypes
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);

/************** parseArgs *************/
/* Parses and validates command line arguments
 * and stores them in the given variables
 *
 * seedURL: pointer to the seedURL string
 * pageDirectory: pointer to the pageDirectory string
 * maxDepth: pointer to maxDepth integer
 *
 */
static void parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, int* maxDepth)
{
  if (argc != 4) { // ensure the correct number of arguments given to command line
    fprintf(stderr, "Incorrect number of arguments\n");
    exit(-1);
  }

                   // Allocate space for the seed URL and copy the first argument to the allocated space
  *seedURL = mem_calloc_assert(strlen(argv[1]) + 1, sizeof(char), "Couldn't assign space for seedURL\n");
  strcpy(*seedURL, argv[1]);

                   // Normalize URL and then ensure it is internal
  char* normalized = normalizeURL(*seedURL);
  if (!isInternalURL(normalized)) {
    fputs("Seed URL is not internal\n", stderr);
    exit(-1);     
  }
  free(normalized); // Free the normalized copy to prevent memory leak

                    // Copy page directory
  *pageDirectory = mem_calloc_assert(strlen(argv[2]) + 1, sizeof(char), "Couldn't assign space for pageDirectory\n");
  strcpy(*pageDirectory, argv[2]);

                    // Initialize the directory
  if (!pagedir_init(*pageDirectory)) {
    fprintf(stderr, "Could not initialize page directory\n");
    exit(-1);
  }

                    // Parse maxDepth and ensure it is in the correct range
  int i;
  
  if (1 != sscanf(argv[3], "%d%n", maxDepth, &i) || *maxDepth < 0 || *maxDepth > 10) {
    fprintf(stderr, "Max depth is invalid or out of range\n");
    exit(-1);   
                    // make sure integer is followed by new line
  } else if (!isspace(argv[3][i]) && argv[3][i] != '\0') { 
    fprintf(stderr, "Invalid max depth\n");
    exit(-1);
  }
}

/************* main *************/
/* Calls parseArgs and calls crawl */
int main(const int argc, char* argv[]) 
{
  char* seedURL;
  char* pageDirectory;
  int maxDepth;

  // Parse command-line arguments
  parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);

  // Begin crawling
  crawl(seedURL, pageDirectory, maxDepth);

  // Free memory used for page directory path
  free(pageDirectory);
  exit(0);
}

/*************** crawl ***************/
/* Crawls through webpages to a certain depth
 * starting from the seedURL page and saves 
 * the HTML of the pages to the specified directory
 * each with a unique id as their file name
 * 
 * seedURL: web address of webpage to start at
 * pageDirectory: location to save files to
 * maxDepth: maximum distance from the seedURL that we will visit
 */
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth)
{
        // ID to number saved files
  int id = 1; 

        // Hashtable to keep track of seen URLs
  hashtable_t* visitedPages = hashtable_new(200);

        // Bag to manage the queue of pages to crawl
  bag_t* pagesToVisit; // check to make sure it is allocated
  if ((pagesToVisit = bag_new()) == NULL) {
    fprintf(stderr, "Couldn't allocate space for bag\n");
    exit(-1);
  }
        // Add seed URL to hashtable and bag
  hashtable_insert(visitedPages, seedURL, "");
  bag_insert(pagesToVisit, webpage_new(seedURL, 0, NULL));

  webpage_t* currWebpage;

        // Keep extracting until bag is empty
  while ((currWebpage = bag_extract(pagesToVisit)) != NULL) {
    
    char* url = webpage_getURL(currWebpage);

        // Attempt to fetch page content
    if (webpage_fetch(currWebpage)) {
      printf("Fetched: %s\n", url);

        // Save the page to the specified directory with the current id, then increment id
      if (webpage_getHTML(currWebpage) != NULL) {
        pagedir_save(currWebpage, pageDirectory, id);
        id++;

        // If the depth is less than the maxDepth, scan for new URLs
        if (webpage_getDepth(currWebpage) < maxDepth) {
          printf("Scanning: %s\n", url);
          pageScan(currWebpage, pagesToVisit, visitedPages);
        }
      }
    } else {
      printf("Failed to fetch: %s\n", url);
    }

        // Free the current webpage object
    webpage_delete(currWebpage);
  }

        // Cleanup
  hashtable_delete(visitedPages, NULL);
  bag_delete(pagesToVisit, NULL);
}

/**************** pageScan *****************/
/* Extracts all URLs from a webpage and adds their
 * webpage to the bag of pages to crawl
 *
 * page: The webpage from which to extract URLs
 * pagesToCrawl: the bag to add the new webpages to
 * pagesSeen: a hashtable that keeps track of pages we have visited
 */
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen)
{
  int pos = 0;
  char* currURL;

    // Extract all URLs found in the webpage
  while ((currURL = webpage_getNextURL(page, &pos)) != NULL) {
    printf("Found: %s\n", currURL);

    // Normalize URL before checking if it is internal
    char* normalized = normalizeURL(currURL);

    if (isInternalURL(normalized)) {
      
    // Try to insert the URL into the pagesSeen hashtable
      if (hashtable_insert(pagesSeen, currURL, "")) {

    // If successful, make a webpage for that URL at current depth + 1 and add it to the bag
        webpage_t* nextPage;
        if ((nextPage = webpage_new(currURL, webpage_getDepth(page) + 1, NULL)) == NULL) {
          fprintf(stderr, "Couldn't create webpage\n");
          exit(-1);
        }
        bag_insert(pagesToCrawl, nextPage);
        printf("Added: %s\n", currURL);
      } else {
    // Already visited
        printf("Already visited: %s\n", currURL);
        free(currURL);
      }
    } else {
    // External URL
      printf("External: %s\n", currURL);
      free(currURL);
    }

    // Clean up normalized string
    free(normalized);
  }
}
