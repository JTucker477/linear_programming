// TODO
// - [o] pre-partition
//      - [X] random solution
//      - [X] random neighbor
//      - [o] calc residue
// - [o] file input
// - [o] collect data
//      - [o] time
//      - [o] take the average of final 100 residue instances
// - [o] writeup

#include <stdio.h>
#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>
#include <string>
#include <random>
#include <cmath> 
#include <cstdlib>
#include <tuple>
#include <numeric>
#include <functional>
using namespace std;
int max_iter = 25000; 
typedef long long ll;

// You will run experiments on sets of 100 integers, with each integer being a random number chosen uniformly from the range [1,1012].


// Returns std array that has a 100 instances of 100 random numbers to input into our algorithms 

array<array <ll , 100>,100> rand_array(){
    array<array <ll , 100>,100>  final;
    random_device rand_dev;
    mt19937 generator(rand_dev());
    std::uniform_int_distribution<long> distribution(1, pow(10,12));

    for (int j = 0; j < 100; j++ ){
        array <ll, 100> temp;

        for (int i = 0; i < 100; i++) {
            temp[i] = distribution(generator) ;
        }
        final[j] = temp;
    }
    return final;

}

array <ll , 100> karmarkar(array<array <ll , 100>,100> input){

    array <ll , 100> final_residues ;
    
    // For each of the 100 instances 
    for (int i = 0 ; i < 100 ; i++){


        ll current_distance;
        int numby = 0;
        while(true){
            numby += 1; 
        
            array <ll , 100>::iterator first_max;
            array <ll , 100>::iterator second_max;

            // Find Largest number
            first_max = max_element(input[i].begin(), input[i].end());
            long long unsigned first_max_value = *first_max;
            int first_max_index = distance(input[i].begin(), first_max); 
            // I set it to zero so that we can find the second largest number next
            input[i][first_max_index] = 0;

            // Find second largest number
            second_max = max_element(input[i].begin(), input[i].end());
            long long unsigned second_max_value = *second_max;
            int second_max_index = distance(input[i].begin(), second_max); 
            input[i][second_max_index] = 0;

            // Assign distance to largest element
            current_distance = first_max_value - second_max_value;
            input[i][first_max_index] = current_distance;

            // If the second max value is 0, we know there's only one nonzero element left
            if (current_distance == first_max_value ){
                break;
            }

            // This is just to check for bugs (distance should always be positive)
            long long distance = first_max_value - second_max_value;
            if (distance < 0){
                printf("\n\n\n\n ERROR \n\n\n");
            }
        }


        final_residues[i] = current_distance;

    }

    return final_residues;

}   


//RESIDUE FUNCTION STANDARD
ll calc_residue(array<ll , 100> input, array<ll , 100> solution){

    array<ll , 100> multiply_result = solution; 

    transform(input.begin(), input.end(),
            solution.begin(), multiply_result.begin(),  
            std::multiplies<ll>() );
        
    return abs(accumulate(begin(multiply_result), end(multiply_result),0));
}

//RESIDUE FUNCTION PREPARTITION
// ll calc_residue(array<ll , 100> input, array<ll , 100> solution){

// }

// RANDOM SOLUTION STANDARD
array <ll, 100 > random_solution(){

    array <ll, 100> sol;
    // add elements to random subset
    for (int i = 0; i < 100; i++)
    {
        if (rand()%2 == 0)
        {
            sol[i] = -1;
        }
        // if not in random subset, add to another subset
        else {
            sol[i] = 1;
        }
    }
    return sol;
}

//RANDOM SOLUTION PREPARTITION
// A random solution can be obtained by generating a sequence of n values in the range [1,n] 
// and using this for P. 
array <ll, 100 > random_solution_PP(){
    array <ll, 100> sol;

    for (int i = 0; i < 100; i++)
    {
        sol[i] = rand() % 100; // random number [0,100] (double check to see if this is inculsive?)
    }
    return sol;
}


// REPEATED RANDOM
array <ll , 100> repeated_random (array<array <ll , 100>,100> input){
    array <ll , 100> final; 
    
    // For each of the 100 instances of the problem
    for (int i = 0; i < 100; i++){

        array <ll , 100> solution = random_solution();

        ll lowest_residue = calc_residue(input[i], solution);

 
        for (int j = 0; j < max_iter ; j ++ ){

            // Try a new random solution
            array <ll , 100> new_sol = random_solution();
        
            ll current_residue = calc_residue(input[i], new_sol);
 
            if (current_residue < lowest_residue){
    
                lowest_residue = current_residue;
            }
        }

        final[i] = lowest_residue;
    }
    
    return final; 
}

// RANDOM NEIGHBOR GENERATION STANDARD
    // A random neighbor 
    // Recall: a neighbor of a solution S is the one such that differs from S in 
        // one or two places.
        // Aka, if A1 represents +1 and A2 -1 values,
        // then moving from S to a neighbor is accomplished either by
            // moving one or two elements from A1 to A2, or moving one or two elements from A2 to A1
array <ll, 100 > random_neighbor(array <ll , 100> sol){

    array <ll , 100> rand_neighbor = sol;
    int rand_i = rand() % (rand_neighbor.size() - 1);
    int rand_j = rand() % (rand_neighbor.size() - 1);
    // makes sure i != j

    while(rand_i == rand_j){
        rand_j = rand() % (rand_neighbor.size() - 1);
    }

    // perform a random move on the current solution to get a random neighbor
    // set s_i to - s_i
    rand_neighbor[rand_i] = -sol[rand_i];
    // with probablity 1/2, set s_j to -s-j
    if (rand()%2 == 0){
        rand_neighbor[rand_j] = -1;
    }
    else{
        rand_neighbor[rand_j] = 1;
    }
    return rand_neighbor;
}

// RANDOM NEIGHBOR - PREPARTITION
// Thinking of all possible solutions as a state space, 
// a natural way to define neighbors of a solution P is as the set of all solutions 
// that differ from P in just one place. The interpretation is that we change the 
// prepartitioning by changing which partition one element lies in.

// A random move on this state space can be defined as follows. 
// Choose two random indices i and j from [1,n] with p_i  != j and set p_i to j.
array <ll, 100 > random_neighbor_pp(array <ll , 100> sol){

    array <ll , 100> rand_neighbor = sol;
    // two random indices i and j from [1,n]
    // make sure p_i != j
    // set p_i to j
    int rand_i = rand() % 100;
    int rand_j = rand() % 100;

    // makes sure p_i != j
    while(rand_neighbor[rand_i] == rand_j){
        rand_j = rand() % 100;
    }

    // perform a random move on the current solution to get a random neighbor
    // set p_i to j
    rand_neighbor[rand_i] = rand_j;

    return rand_neighbor;
}


// HILL CLIMBING
 // (from section )
    // Pick a starting point x
    // While there is a neighbor y in N(x) with f(y) < f(x), set x to y and continue
    // return final solution
    // how to prevent getting stuck in local optima?

array <ll , 100> hill_climb (array<array <ll , 100>,100> input){
    //Start with a random solution S
   
    array <ll , 100> final; 
    
    // For each of the 100 instances of the problem
    // ?? is the transform i's different?
    for (int i = 0; i < 100; i++){
        array <ll , 100> solution = random_solution();

        ll lowest_residue = calc_residue(input[i], solution);
    
        for (int j = 0; j < max_iter ; j ++ ){
            // Try a new random solution
            array <ll , 100> new_sol = random_neighbor(solution);

            ll current_residue = calc_residue(input[i], new_sol);

            if (current_residue < lowest_residue){
        
                lowest_residue = current_residue;
            }
        }

        final[i] = lowest_residue;
    }
    return final; 
}

//  simmulated annealing (pseudocode -- rn similar to hill climb except the improve with reesidues not always better)
array <ll , 100> sim_anneal (array<array <ll , 100>,100> input){
    //Start with a random solution S
   
    array <ll , 100> final; 
    
    // For each of the 100 instances of the problem
    for (int i = 0; i < 100; i++){
        array <ll , 100> solution = random_solution();

        ll lowest_residue = calc_residue(input[i], solution);
    
        for (int j = 0; j < max_iter ; j ++ ){
            // Try a new random solution
            array <ll , 100> new_sol = random_neighbor(solution);

            ll current_residue = calc_residue(input[i], new_sol);

            if (current_residue < lowest_residue){
                lowest_residue = current_residue;
            }
            else {
                float p = exp(-(current_residue - lowest_residue)/ (pow(10,10) * pow((0.8),floor(j / 300))));

                // this isn't TRULY random but close enough?
                if (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) <= p) // https://stackoverflow.com/questions/686353/random-float-number-generation
                {
                    lowest_residue = current_residue;
                }
            }
            // if residue(S) < residue(S′′) then S′′ = S
            if (current_residue < lowest_residue){
                lowest_residue = current_residue;
            }
        }
        final[i] = lowest_residue;
    }
    return final; 
}


int main(){
    srand(time(NULL));

    array<array <ll , 100>,100> chicken = rand_array();

    array <ll , 100> final1 = repeated_random(chicken);
    array <ll , 100> final2 = hill_climb(chicken);
    array <ll , 100> final3 = sim_anneal(chicken);
    array <ll , 100> kk = karmarkar(chicken);
    printf("result1: %lld %lld %lld \n", final1[0], final1[1], final1[2]);
    printf("result2: %lld %lld %lld\n", final2[0], final2[1], final2[2]);
    printf("result3: %lld %lld %lld\n", final3[0], final3[1], final3[2]);
    printf("kk: %lld %lld %lld\n", kk[0], kk[1], kk[2]);
} 




// pluf A' into our alg to find the smallest residue

// we have our standard representation A
// reset a_i to be thee sum of all values j with p_j = i
// for example using:
//      A' = (0,0,...,0)     // initalize to 0
//      for j = 1 to n
//          a'_pj = a' pj + aj

// given P
// array <ll , 100> A_new;
// for (int j = 1; j < 100; i++)
// {
//     A_new[i] = 0;
// }

// best partition to give the lowest reesidue
// kk is also another alg that approximates the best partition/lowest calc_residuebut kk also has a diff role:
    // calculate the residue from a prepartition
    // 

// calc residue would be using kk
// random neighbor : pick 2 indices then put them together


// currently, standard solution: A = [+1,+1,+1,+1,-1,-1]
// need prepartition, A': A' = []

// radom solution partitionfunction
// each element is 
// every element in P is between 1 and n randomly, where n is 100


// change calc residue
// change random neighbor

// ARCHIVE



// array <int, 3> v1 = {1,2,3};
//     array <int, 3> v2 = {2,3,4}; 
//     array <int, 3> v3 = v1;
//     v2[0] = 0; 
//     std::transform( v1.begin(), v1.end(),
//                 v2.begin(), v2.begin(),  // assumes v1,v2 of same size > 1, 
//                                           //       v one element smaller
//                 std::multiplies<int>() );
//     printf("hi %d\n", v1[0]);
//     printf("hi %d\n", v1[1]);
//     printf("hi %d\n", v1[2]);


// Returns two random subsets of the inputed array. STILL TO DO
// need to return two sets (return two things!)
// initialize 
// each array is at most 100 but should be 
// test
// pair<vector <ll >, vector <ll> > two_random_set(array <ll , 100> input){
//     // array <ll , 100> s1;
//     // array <ll , 100> s2;
//     vector <ll> s1;
//     vector <ll> s2;

//     // add elements to random subset
//     for (int i = 0; i < 100; i++)
//     {
//         if (rand()%2 == 0)
//         {
//             s1.push_back(input[i]);
//         }
//         // if not in random subset, add to another subset
//         else {
//             s2.push_back(input[i]);
//         }
//     }
//     return make_pair(s1,s2);
// }

// Heuristics


// Repeated random: repeatedly generate random solutions to the problem, as determined by the representation.
// Start with a random solution S for iter = 1 to max iter
    // S′ = a random solution
    // if residue(S′) < residue(S) then S = S′ 
// return S

// repeated random
// Maybe create two random subsets within

//pseudocode
// vector a;
// for (int i = 0; i < 100; i++)
// {
//     a.push_back(rand())
// }

// array <ll , 100> repeated_random(){
//     // Create random solution
// }

// John's repeated random:

// pair<vector <ll >, vector <ll> > random_neighbor(pair<vector <ll >, vector <ll> >){


// }



// array<array <ll , 100>,100> rand_array(){

//     int size = 100;

//     array <array <ll, 100>,100> final;

//     // https://stackoverflow.com/questions/37396278/how-to-generate-very-large-random-number-in-c/37397139

//     /* Seed */
//     random_device rd;

//     /* Random number generator */
//     // std::default_random_engine generator(rd())

//     /* Distribution on which to apply the generator */
//     // std::uniform_int_distribution<long long > distribution(1,pow(10,12));



  
 
//     return final;
// }