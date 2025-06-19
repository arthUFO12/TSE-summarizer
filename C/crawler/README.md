[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/phOqX7q9)
# CS50 TSE
## Arthur Ufongene (arthUFO12)

The assignment and Specs are in a [public repo](https://github.com/CS50DartmouthSP25/labs/tse).
Do not clone that repo; view it on GitHub.
Watch there for any commits that may represent updates to the assignment or specs.

Add here any assumptions you made while writing the crawler, any ways in which your implementation differs from the three Specs, or any ways in which you know your implementation fails to work.

- I assumed that already visited websites should not be added to the bag of websites to visit. 
- I assumed that a delete function isn't needed to clean up the hashtable and bag because the bag will be empty by the completion of the while loop and I used constants for the item in the hashtable.
- I assumed the seedURL did not need to be freed manually since it will be freed when its webpage is deleted
- I also assume that my own implementation of the hashtable module is used, which uses mem_calloc_assert to allocate memory and guarantees that memory will be allocated for the data structure, otherwise throwing an error.

- Please ignore the page_load function in the pagedir module as that is for the next lab.
