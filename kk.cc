// TODO
// - [o] pre-partition
//      - [X] random solution
//      - [X] random neighbor
//      - [o] calc residue
// - [x] file input
// - [o] collect data
//      - [o] time
//      - [o] (do this in Google sheets)take the average of final 100 residue instances
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

    array <ll , 100> solution;
    if (pp){
        solution = random_solution_PP();
    }
    else{
        solution = random_solution();
    }
    ll lowest_residue;

    if (pp){
        lowest_residue = calc_residue_PP(input, solution);
    }
    else{
        lowest_residue = calc_residue(input, solution);
    }
 
    for (int j = 0; j < max_iter ; j ++ ){

        // Try a new random solution
        array <ll , 100> new_sol;
        if (pp){
            new_sol = random_solution_PP();
        }
        else{
            new_sol  = random_solution();
        } 
        ll current_residue; 
        if (pp){
            current_residue = calc_residue_PP(input, solution);
        }
        else{
            current_residue = calc_residue(input, solution);
        }


        if (current_residue < lowest_residue){

            lowest_residue = current_residue;
        }
    }
    return lowest_residue; 
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

    // For each of the 100 instances of the problem
    // ?? is the transform i's different?
    array <ll , 100> solution;
    if(pp){
        solution = random_solution();
    }
    else{
        solution = random_solution_PP();
    }
    ll lowest_residue;
    if(pp){
        lowest_residue = calc_residue_PP(input, solution);
    }
    else{
        lowest_residue = calc_residue(input, solution);
    } 

    for (int j = 0; j < max_iter ; j ++ ){
        // Try a new random solution
        array <ll , 100> new_sol;
        if (pp){
            new_sol = random_neighbor_pp(solution);
        }
        else{
            new_sol = random_neighbor(solution);
        }
         

        ll current_residue;
        if (pp){
            current_residue = calc_residue_PP(input, new_sol);
        }
        else{
            current_residue = calc_residue(input, new_sol);
        }
        ;

        if (current_residue < lowest_residue){
            lowest_residue = current_residue;
        }
    }

    return lowest_residue; 
}

//  simmulated annealing (pseudocode -- rn similar to hill climb except the improve with reesidues not always better)
ll sim_anneal (array <ll , 100> input, bool pp){
    //Start with a random solution S
   
    array <ll , 100> solution;
    if (pp){
        solution = random_solution_PP();
    }
    else{
        solution = random_solution();
    } 

    ll lowest_residue;
    if (pp){
        lowest_residue = calc_residue_PP(input, solution);
    } 
    else{
        lowest_residue = calc_residue(input, solution);
    }

    for (int j = 0; j < max_iter ; j ++ ){
        // Try a new random solution
        array <ll , 100> new_sol;
        if (pp){
            new_sol = random_neighbor_pp(solution);
        }
        else{
            new_sol = random_neighbor(solution);
        } 

        ll current_residue;
        if (pp){
            current_residue = calc_residue_PP(input, new_sol);
        }
        else{
            current_residue = calc_residue(input, new_sol);
        } 

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

    return lowest_residue; 
}


// COLLECT DATA - got this format from our strassen
//      - [o] time
//      - [o] (we can do this easily in Sheets instead) take the average of final 100 residue instances
// void collect_data(){
//     array<array <ll , 100>,100> chicken = rand_array();
//     std::ofstream file;
//     file.open("results.csv", std::ios::app);

//     auto start = high_resolution_clock::now();

//     array <ll , 100> final1 = repeated_random(chicken);
//     // have to do individual timings for these too
//     // array <ll , 100> final2 = hill_climb(chicken);
//     // array <ll , 100> final3 = sim_anneal(chicken);
//     // array <ll , 100> kk = karmarkar(chicken);

//     auto stop = high_resolution_clock::now();
//     auto duration = duration_cast<nanoseconds>(stop - start);
//     float time = duration.count();
//     // file << d << ", " << i <<  ", " << time << "\n";
//     file.close();
// }


int main(int argc, char *argv[]){

    srand(time(NULL));


    // array< array<ll, 100>, 100> chicken = rand_array();
    // ll answer1 = repeated_random(chicken[0]);
    // ll answer2 = hill_climb(chicken[0]);
    // ll answer3 = sim_anneal(chicken[0]);
    // printf("repeated random: %lld, hill climb: %lld %lld\n", answer1, answer2, answer3);

    // if (argc != 2) {
    //     cerr << "Usage: ./kk inputfile\n";
    //     exit(1);
    // }

    // // initialize array
    // array <ll , 100> input;
    // // read file
    // // input file is a file of 100 integers, one on each line
    // ifstream f;
    // f.open(argv[1],ios::in);
    // string entry;

    // for (int i = 0; i < 100; i++)
    // {
    //     getline(f, entry);
    //     input[i] = stoi(entry);
    // }
    
    // f.close();

    

    // array <ll , 100> chicken  = {10,8,7,6,5};
    // array <ll , 100> P = {1,2,2,4,5};
    // ll answer = calc_residue(chicken, P);
    // printf("answer: %lld\n", answer);

    array <ll , 100> chicken  = {10,8,7,6,5};
    array <ll , 100> P = {1,-1,1,-1,1};
    // 10 - 8 + 7 - 6 + 5
    ll answer = calc_residue(chicken, P);
    printf("answer: %lld\n", answer);

    // array <ll , 100> final1 = repeated_random(chicken);
    // array <ll , 100> final2 = hill_climb(chicken);
    // array <ll , 100> final3 = sim_anneal(chicken);
    // array <ll , 100> kk = karmarkar(chicken);
    // printf("result1: %lld %lld %lld \n", final1[0], final1[1], final1[2]);
    // printf("result2: %lld %lld %lld\n", final2[0], final2[1], final2[2]);
    // printf("result3: %lld %lld %lld\n", final3[0], final3[1], final3[2]);
    // printf("kk: %lld %lld %lld\n", kk[0], kk[1], kk[2]);


} 
