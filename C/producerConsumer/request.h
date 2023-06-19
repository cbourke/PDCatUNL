
typedef struct {
  int id;
  double delay;
} Request;

/**
 * Executes an HTTPS request to a delay script to simulate a certain
 * amount of synchronous work.
 */
double execute(Request req);
