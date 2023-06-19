
/**
 * String structure used to keep track of the size
 * of the allocated space.  Used by the curl library
 * as a buffer.
 */
struct string {
  char *ptr;
  size_t len;
};

/**
 * Initializes a string structure
 */
void init_string(struct string *s);

/**
 * Callback used by the curl function so that we capture the curl
 * stream to a buffer.  This function automatically expands the
 * buffer via realloc (exiting on failure).
 */
size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s);

/**
 * Function returns a string containing the content of the given
 * url.
 */
char *getContent(const char *url);
