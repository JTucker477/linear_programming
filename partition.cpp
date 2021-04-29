
//NOW
// FAILS
    // repeated random standard: 11
        // These numbers seem high
    // hill climb standard: 2
        // These numbers look way too low.
    // sim anneal standard: 3
        // These numbers seem high.

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
#include <fstream>
using namespace std;
#include <chrono>
using namespace std::chrono;
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

ll karmarkar(array <ll , 100> input){

    ll final_residue = -1;
    
    ll current_distance;
    int numby = 0;
    while(true){
        numby += 1; 
    
        array <ll , 100>::iterator first_max;
        array <ll , 100>::iterator second_max;

        // Find Largest number
        first_max = max_element(input.begin(), input.end());
        long long unsigned first_max_value = *first_max;
        int first_max_index = distance(input.begin(), first_max); 
        // I set it to zero so that we can find the second largest number next
        input[first_max_index] = 0;

        // Find second largest number
        second_max = max_element(input.begin(), input.end());
        long long unsigned second_max_value = *second_max;
        int second_max_index = distance(input.begin(), second_max); 
        input[second_max_index] = 0;

        // Assign distance to largest element
        current_distance = first_max_value - second_max_value;
        input[first_max_index] = current_distance;

        // If the second max value is 0, we know there's only one nonzero element left
        if (current_distance == first_max_value ){
            break;
        }

        // This is just to check for bugs (distance should always be positive)
        long long distance = first_max_value - second_max_value;
        if (distance < 0){
            printf("\n\n\n\n ERROR \n\n\n");
        }
        final_residue = current_distance;

    }
    return final_residue;

}   


//RESIDUE FUNCTION STANDARD
ll calc_residue(array<ll , 100> input, array<ll , 100> solution){

    array<ll , 100> multiply_result = solution; 

    transform(input.begin(), input.end(),
            solution.begin(), multiply_result.begin(),  
            std::multiplies<ll>() );
        
    return abs(accumulate(begin(multiply_result), end(multiply_result),0));
}

ll calc_residue_PP(array<ll , 100> A, array<ll , 100> P){

    array<ll , 100> final;

    for (int i = 0; i< 100; i++){
        ll target = P[i];
        ll sum = A[i]; 
        for (int j = i + 1; j < 100; j++){
            if (target == P[j]){
                sum += A[j];
                // So we will not account for P[j] any more
                P[j] = 200; 
                A[j] = 0;
            }
        }
        final[i] = sum; 
    }

    return karmarkar(final);
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
ll repeated_random (array <ll , 100> input, bool pp){

    // start with a random solution S
    array <ll , 100> S;
    if (pp){
        S = random_solution_PP();
    }
    else{
        S = random_solution();
    }
    ll residueS;
 
    for (int j = 0; j < max_iter ; j ++ ){
        // residue(S)
        if (pp){
            residueS = calc_residue_PP(input, S);
        }
        else{
            residueS = calc_residue(input, S);
        }
        // Try a new random solution S'
        array <ll , 100> S1;
        if (pp){
            S1 = random_solution_PP();
        }
        else{
            S1  = random_solution();
        } 
        // residue(S')
        ll residueS1; 
        if (pp){
            residueS1 = calc_residue_PP(input, S1);
        }
        else{
            residueS1 = calc_residue(input, S1);
        }

        // if residue(S') < residue(S) then s=S'
        if (residueS1 < residueS){

            // lowest_residue = current_residue;
            S = S1;
        }
    }
    if (pp){
        residueS = calc_residue_PP(input, S);
    }
    else{
        residueS = calc_residue(input, S);
    }
    return residueS; 
    
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
    int rand_i = rand() % 100; // exclusive?
    int rand_j = rand() % 100;
    // makes sure i != j

    while(rand_i == rand_j){
        rand_j = rand() % 100;
    }

    // perform a random move on the current solution to get a random neighbor
    // set s_i to - s_i
    rand_neighbor[rand_i] = -sol[rand_i];

    // with probablity 1/2, set s_j to -s-j
    if (rand()%2 == 0){
        rand_neighbor[rand_j] = -sol[rand_j];
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
// pp = true if pp
ll hill_climb (array <ll , 100> input, bool pp){
    //Start with a random solution S

    array <ll , 100> S;
    if(pp){
        S = random_solution_PP();
    }
    else{
        S = random_solution();
    }

    ll residueS;
    for (int j = 0; j < max_iter ; j ++ ){
        // residue(S)
        if(pp){
            residueS = calc_residue_PP(input, S);
        }
        else{
            residueS = calc_residue(input, S);
        } 
        // Try a new random neighbor S'
        array <ll , 100> S1;
        if (pp){
            S1 = random_neighbor_pp(S);
        }
        else{
            S1 = random_neighbor(S);
        }
        // residue(S')
        ll residueS1;
        if (pp){
            residueS1 = calc_residue_PP(input, S1);
        }
        else{
            residueS1 = calc_residue(input, S1);
        }
        // if residue(S') < residue(S) then S=S'
        if (residueS1 < residueS){
            S = S1; 
            // lowest_residue = current_residue;
        }
    }
    if(pp){
        residueS = calc_residue_PP(input, S);
    }
    else{
        residueS = calc_residue(input, S);
    } 
    return residueS; 
}

//  simmulated annealing (pseudocode -- rn similar to hill climb except the improve with reesidues not always better)
ll sim_anneal (array <ll , 100> input, bool pp){
    
    //Start with a random solution S
    array <ll , 100> S;
    if (pp){
        S = random_solution_PP();
    }
    else{
        S = random_solution();
    } 

    // S'' = S
    array <ll , 100> S2 = S;
    ll residueS;
    ll residueS2;
    for (int j = 0; j < max_iter ; j ++ ){
        // residue(S)
        if (pp){
            residueS = calc_residue_PP(input, S);
        } 
        else{
            residueS = calc_residue(input, S);
        }
        // Try a new random neighbor S'
        array <ll , 100>S1;
        if (pp){
            S1 = random_neighbor_pp(S);
        }
        else{
            S1 = random_neighbor(S);
        } 
        // residue(S')
        ll residueS1;
        if (pp){
            residueS1 = calc_residue_PP(input, S1);
        }
        else{
            residueS1 = calc_residue(input, S1);
        } 

        // if residue(s') < residue(s) then S=S'
        if (residueS1 < residueS){
            // lowest_residue = residue1;
            S = S1;
        }
        //else S=S' with prob  ... 
        else {
            if (pp){
                residueS = calc_residue_PP(input, S);
            }
            else{
                residueS = calc_residue(input, S);
            }
            float p = exp(-(residueS1 - residueS)/ (pow(10,10) * pow((0.8),floor(j / 300))));

            // this isn't TRULY random but close enough?
            if (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) <= p) // https://stackoverflow.com/questions/686353/random-float-number-generation
            {
                // lowest_residue = residue1;
                S = S1;
            }
        }
        // residue (S'')
        
        if (pp){
            residueS2 = calc_residue_PP(input, S2);
        }
        else{
            residueS2 = calc_residue(input, S2);
        } 
        if (pp){
            residueS = calc_residue_PP(input, S);
        }
        else{
            residueS = calc_residue(input, S);
        }
        // if residue(S) < residue(S′′) then S′′ = S
        if (residueS < residueS2){
            // residue2 = lowest_residue;
            S2 = S;
        }
    }
    if (pp){
        residueS2 = calc_residue_PP(input, S2);
    }
    else{
        residueS2 = calc_residue(input, S2);
    } 
    return residueS2; 
}


int main(int argc, char *argv[]){

    srand(time(NULL));

    if (argc != 4) {
        cerr << "Usage: ./kk flag algorithm inputfile\n";
        exit(1);
    }

    // initialize array
    array <ll , 100> input;
    // read file
    // input file is a file of 100 integers, one on each line
    ifstream f;
    f.open(argv[3],ios::in);
    string entry;

    for (int i = 0; i < 100; i++)
    {
        getline(f, entry);
        input[i] = stoull(entry);
    }
    
    f.close();

    int alg = atoi(argv[2]);
    if (alg == 0)
    {
        ll output = karmarkar(input);
        cout << output;
    }
    else if (alg == 1)
    {
        ll output = repeated_random(input, false);
        cout << output;
    }
    else if (alg == 2)
    {
        ll output = hill_climb(input, false);
        cout << output;
    }
    else if (alg == 3)
    {
        ll output = sim_anneal(input, false);
        cout << output;
    }
    else if (alg == 11)
    {
        ll output = repeated_random(input, true);
        cout << output;
    }
    else if (alg == 12)
    {
        ll output = hill_climb(input, true);
        cout << output;
    }
    else if (alg == 13)
    {
        ll output = sim_anneal(input, true);
        cout << output;
    }

} 
