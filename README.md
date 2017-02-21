# tunnel-solver

 * @author Dragoslav Rakić (dragoslav_rakic@outlook.com)
 * @date 15, February, 2017
 * @brief Program with algorithm for calculating the most quicker way for a number of people to pass through a tunnel.
 *
 * @bug No known bugs.
 *
 * This program solves the issue, which is the fastest way passing through a narrow dark tunnel for a number of people
 * with having only one flashlight. The tunnel can pass only two people at the same time and somebody has to go back
 * in order to return flashlight. Everyone has a certain speed, two people travel at the speed of those person that has
 * a lowest speed. Person that is returning back travel at his own speed.
 * Although it seems that the solution for this problem can be easily solved with a recursive function, this program
 * represents a new way approach for this problem. Although the program uses a recursive function, it is used only to
 * summarize all the results of the solutions in vector of paths.
 * Program uses class solutionsGenerator for creating a matrix (two-dimensional array), and all possible solutions for
 * each way of the path, forward (two persons going), backward(one person returning). Program shows how we can transform
 * this problem in binary operations. For example if we have 3 persons that is on one side, lets say at beginning on tunnel
 * we can represent this with 111, if first two people cross on other side we can represent this as 110.
 * Program can do in theory bit shifting and bit operations to solve this problem, but for know we will simulate bits like
 * characters.
 * We will do same operation on bits with another program version.
 *
 * Let's see some examples:
 * 111 FO (beginning of the tunnel) forward->backward
 * 110 B0 (end of the tunnel) backward->forward
 * 101 F1 (beginning of the tunnel) forward->backward
 * 111 B1 (end of the tunnel)
 *
 * Scenario of this example - In F0 we see all people is on beginning of the tunnel, on B0 first two people cross the tunnel,
 *                            on F1 first person is returned, on B1 first and last person move to the end of tunnel.
 *
 * On B0 we have tree solutions: 101, 110, 011. The same solutions we have on F1. So we can make a 4 matrices of solutions:
 *
 *             B0[0] 101    F1[0] 101
 * F0[0] 111   B0[1] 110    F1[1] 110    B1[0] 111
 *             B0[2] 011    F1[2] 011
 *
 *             Path possibilities: F0[0],B[0],F1[1],B1[0]
 *                                 F0[0],B[0],F1[2],B1[0]
 *                                 F0[0],B[1],F1[0],B1[0]
 *                                 F0[0],B[1],F1[2],B1[0]
 *                                 F0[0],B[2],F1[0],B1[0]
 *                                 F0[0],B[2],F1[1],B1[0]
 *
 * Rules:
 * - forward->backward is a path operation that check out binary match. If we had on beginning of the tunnel 4 persons with
 * representation 0111, and on the end 0111 after forward->backward operation we see that there is something wrong.
 * Why? If we have on beginning 0111, then we now that on the end is 1000. After forward->backward operation from
 * 0111 on the end only can be: 1011, 1101 and 1110 because two people are returning and the first is already on the end.
 * - same is for backward->forward operation, here one person is returning so if we have on end 1010 we cannot have on the
 * beginning 1110. Why? If we have on end 1010, we know that on the beginning is 0101, so possible solutions are: 1101,0111.
 *
 *
