<h1> README </h1>

(soln.exe) OR ([soln.cpp](soln.cpp) can be run on an IDE like Visual Studio code with some extensions : C/C++ , Code Runner) in the same folder containing flights.csv , canceled.csv, passengers.csv (preferably offline).

<hr>

My first thought after reading the problem statement was to use network flow algorithms to solve the problems.

In search of more hints to the problem, I dug deeper into the python generator codes :


* Flights.py provided huge information that the number of airports is limited to 35 

with this and the fact that at maximum only 3 layoffs are possible : we get only 39270 (35*34*33) maximum possible routes - making something related to BFS (Edmond Carp's Algorithm) able to run in sufficent time.

* Passengers.py revealed that all these flights are somewhere between 50% - 100% filled. (therefore on avg 25% empty)

suggesting filling the void capacities to be a multi phased process. Starting off with careful greedy approach and then to particular heuristics.

* Cancelled.py tell that upto 5 flights can be cancelled at max.

This was probably the biggest hint, at these five nodes (airports) we may add the source capacities and model this problem as a multi-sourced maximum network flow problem.


The following illustration shows how to solve a multi-sourced network flow :

<img src="photo.jpg">

<hr>

From these I started working out my solution : 
Now the question comes from where to start in the flow. If we take the greedy approach, we start with the node having highest source capacity and then satisfy nodes in this order till we exhaust all options and move to the next highest source.

Counter : one can argue that if one seat can be taken by a passenger in one layoff and another passenger in two layoffs, as both are satisfying the first criteria we should satisfy the second criteria and allot the seat to the passenger going through only one layoff.
But here's the problem : suppose through that path, passenger A can just complete their trip in three layoffs and some passenger B is going through one layoff - If there exists a possibility where B can get some alternative route and finish their trip (within time constraints as well) in <= 3 trips ; then first priority is better fulfilled.

Therefore, Blind Greedy Solving is Rejected!

We can introduce a new vector 'collisions' which accounts for the same flight being used in multiple

Firstly, from Flights.csv, we make a 1 D table by Flight ID's containing all other information.
Then from passengers.csv - one by one reduce the flight capacities to get final residual capacities.
After that, in one 2 d array, we note all passengers who got their flights cancelled and also make capacity of all these flights = 0;

<hr>

For the five cancelled flights :
Make a vector vector int s(5);

Make three vectors accounting for the three path lengths possible;

Make another array containing flight IDS and demands.
The above three vectors for paths can be made via individual BFS for each cancelled flight source while considering the time parameter between layoffs as well.

As the flights are ~ 25% empty on average, run through these three vectors for each cancelled flight in order [length 1 for all then length (1 then 2) for all then (1 then 2 then 3) for all...] and first assign passengers with FIDS having 0 collisions (one may ask this might compromise on 2nd criteria but think of it this way : all FIDS with collision in lower path lengths will carry a passenger anyway)

While running through, if one source manages to get all its volume satisfied - then reduce all its remaining flight collisions and set a bool flag to true to repeat the process with new collision values. 

After running through the vectors as described above, only the paths with some collision will be remaining.

From here onwards, we can split our algorithm so to lot of experimental heuristics :
1) Satisfy the one with lower no. of capacity requirement (maybe it can free up spaces for other cancelled flight).

2) Satisfy the one with least number of (s1+s2+s3) routes possible in the order of lower number of flights.

I could not implement these ideas on time and have presented a solution which just allots the flights greedily for now.

<hr>

[Soln Explaination](explaination.md)
