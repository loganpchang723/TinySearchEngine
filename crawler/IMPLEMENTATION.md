# CS50 TSE

## Crawler Implementation

### API

#### crawler
Starts at seedURL and loops over all pages to be explored, saving them to appropriate files in pageDirectory
```c 
static void crawler (char* seedURL, char* pageDirectory, const int maxDepth)
```
#### page_scanner
Extracts URLs from the page, adds unseen URLs to pageBag to be crawled
```c 
static void page_scanner (webpage_t* page, bag_t* pageBag, hashtable_t* seen)
```

### Pseudocode

For `int main`:
* Read in and validate the command-line arguments (ie. make sure there are 3 arguments, `seedURL` is internal, `maxDepth` is an integer, `pageDirectory` is a writable directory)
* Call `crawler` with the `seedURL`, `maxDepth`, and `pageDirectory` arguments

For `void crawler`:
Arguments: `seedURL` (a string), `pageDirectory` (a string), `maxDepth` (an int)
* Initialize the `bag` of webpages to be saved and `hashtable` of seen URLs. Assert these structs were initialized without error
* Create a `webpage` of the `seedURL` at depth 0 and insert it into the `bag` of webpages to be saved and insert `seedURL` into the `hashtable` of seen URLs
* While there are webpages in the `bag` of webpages to be saved:
    * Extract a webapge from the `bag`
    * Attempt to fetch the HTML of the webpage
    * If the current page's depth is less than `maxDepth`, scan the current page for embedded URLs using `page_scanner`
    * Delete the current webpage struct from memory
* Delete the `bag` of webpages and `hashtable` of seen URLs from memory

For `void page_scanner`:
Arguments: `page` (a webpage struct pointer), `pageBag` (a bag struct pointer of webpages to be saved), `seen` (a hashtable struct pointer of seen URLs)
* While the `page` has URLs:
    * Extract the next URL on the page, normalize it, and check if it is internal
    * If it is internal, try to insert the URL into `seen`
    * If the insert is succesful, the current URL has not been seen before, so create a new webpage struct for this URL
    * Insert this new webpage struct into `pageBag`

### Data Structures
Three helper modules provide data structures:
* `bag` of webpage (URL, depth, HTML) structures
* `hashtable` of (URL, "") pairs where each URL has already been encountered by the crawler
* `set` of URLs (indirectly used by hashtable)