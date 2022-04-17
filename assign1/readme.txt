File: readme.txt
Assignment: assign1
Author: Xiaokun Chen
Pronouns: He/him
----------------------

ARIANE-5 CASE STUDY
1)The cause was the program on Ariane 5 trying to fill 16-bit format varible with 64-bit format varible. And that caused overflow which leads to the system error. They did not detect it because the engineer never thought that Ariane 5 was much faster than Ariane 4. Plus, they did not do the simulation.
2)If the velocity is big enough, bigger than the range that a 32-bit int could represent, the system would experience an overflow. I would collect the simulation velocity value of the ship before launching. Run the test set just as we did on the sat problem and consider all the boundary conditons.

