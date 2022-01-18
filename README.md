# Producer consumer problem using semphores

## Synchronization of threads using semaphores

The goal of this task is implementation of program in `C` which will allow simultaneous task
of producing and consuming ingredients to make 🥟 at the same time by different consumers
(there are 4 types of ingredient producers and 3 types of consumers who will make them 🥟).
The pastry is ingredient which will be used by all consumers so we will have to be carefoul there.

We will have to use semaphores to prevent producers to produce at the same time, as well as to
prevent conusmers from sending new orders before finish previous ones. The order of blocking is very important,
every consumer needs to have it's basic ingredient before requesting pastry, this prevents from taking resource
which otherwise could be very well used by diiferent consumer.

With 🥟 made with ❤️