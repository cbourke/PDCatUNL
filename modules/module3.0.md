# PDC@UNL
## Module 3.0
**[School of Computing](https://computing.unl.edu/)**  
**[College of Engineering](https://engineering.unl.edu/)**  
**[University of Nebraska-Lincoln](https://unl.edu)**  

In this module, you will observe a PDC pattern known as the consumer-producer
pattern which is a way of organizing, coordinating, and handling asynchronous
"requests" for work.

# Problem Statement

A common scenario in computing is when there are a collection of independent
asynchronous "entities" that make requests for work that require computational
resources.  For example, thousands or millions of users make a request to a
webserver for an image or webpage.  There needs to be a mechanism by which each
request is received, organized and handled so that the computational resources
can be coordinated and shared.

Consider the real-world example of $n$ lunch customers at a restaurant that
has $m$ order kiosks.  Each customer needs access to a kiosk to make their
order and only one customer can use the kiosk at once.  If two customers
approach the same kiosk at the same time, we need a way to ensure that they
each get a turn.

These scenarios can be modeled as a *producer-consumer* pattern in which
a collection of independent *producers* generate requests and a collection
of *consumers* "handle" the requests.  To organize and coordinate this process
producers submit their requests to a *blocking queue*.  A queue is simply a
"line" (like a line at the restaurant) in which requests wait until a consumer
takes the next one.  

The queue coordinates this by *blocking* consumers and/or
producers in the following manner.  If the queue has a maximum capacity and
is full, producers are blocked from generating any more requests.  If the
queue is empty (there are no waiting requests) and a consumer tries to handle
the next request it is blocked and told to wait.  The queue notifies the
consumer when a new request becomes available.  This scenario is visualized
below.

<img src="../web/assets/img/producerConsumer.png"/>

## Visualization

To give you an idea of how a producer-consumer pattern works we've provided
a simple visualization simulating a generic request system.  

TODO

## Demonstration & Exercises - Java version

We have written a producer-consumer simulation in Java that is available
in the same Eclipse project available at https://github.com/cbourke/PDCatUNL

We have provided several classes (in `unl.soc.producer_consumer`) that
simulate a producer-consumer scenario:

* `RequestGenerator` is the producer class.  It runs continuously in its own
thread and generates a new request at random intervals.
* `RequestHandler` is the consumer class.  It also runs continuously in its
own thread and handles requests.
* `HttpDelayRequest` is the actual request class that is generated and handled.
It simply makes a dummy request to a webserver that "simulates" work by making
an artificial delay between 2 and 5 seconds (randomly)
* `RequestManager` coordinates the simulation by managing all the threads and
keeping track of all requests in a blocking queue.  
* `RequestMonitor` is a special class that monitors the queue and reports how
many requests are awaiting execution (it reports every 5 seconds).

### Instructions

1. Open the `Simulation` class and observe the parameters.  You can specify
how many `RequestGenerator`s and `RequestHandler`s to create in the simulation
and an interval that specifies how often each `RequestGenerator` generates
a request (initially it is between 2 and 5 seconds).  
2. Run the simulation by running this class with the provided parameters and
observe the output.  The `RequestGenerator`s and `RequestHandler`s report on
each action and the `RequestMonitor` periodically reports how many tasks are
waiting in the queue.  The simulation runs continuously; to stop it you'll need
to kill the program manually (the stop button in Eclipse).  
3. Run and rerun the simulation with the following parameters and observe and
note the results.

TODO: table

4. Answer the following questions:

* TODO
