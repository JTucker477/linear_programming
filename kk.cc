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
using namespace std;

// Returns std array that has a 100 instances of 100 random numbers to input into our algorithms 
array<array <unsigned long long , 100>,100> rand_array(){

    int size = 100;

    array <array <unsigned long long, 100>,100> final;

    // https://stackoverflow.com/questions/37396278/how-to-generate-very-large-random-number-in-c/37397139

    /* Seed */
    std::random_device rd;

    /* Random number generator */
    std::default_random_engine generator(rd());

    /* Distribution on which to apply the generator */
    std::uniform_int_distribution<long long unsigned> distribution(0,0xFFFFFFFFFFFFFFFF);


    for (int j = 0; j < size; j++ ){
        array <unsigned long long, 100> temp;

        for (int i = 0; i < size; i++) {
            temp[i] = distribution(generator) ;
        }
        final[j] = temp;
    }
  
 
    return final;
}
array <unsigned long long , 100> karmarkar(array<array <unsigned long long , 100>,100> input){



    array <unsigned long long , 100> final_residues ;

   

    // Example: residue should be 2. 
    // input[0][0] = 9;
    // input[0][1] = 5;
    // input[0][2] = 3;
    // input[0][3] = 15;
    
    // For each of the 100 instances 
    for (int i = 0 ; i < 100 ; i++){


        unsigned long long current_distance;
        int numby = 0;
        while(true){
            numby += 1; 
        
            array <unsigned long long , 100>::iterator first_max;
            array <unsigned long long , 100>::iterator second_max;

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

// Returns two random subsets of the inputed array. STILL TO DO
// need to return two sets (return two things!)
// initialize 
// each array is at most 100 but should be 
// test
pair<vector <unsigned long long >, vector <unsigned long long> > create_random_set_division(array <unsigned long long , 100> input){
    // array <unsigned long long , 100> s1;
    // array <unsigned long long , 100> s2;
    vector <unsigned long long> s1;
    vector <unsigned long long> s2;

    // add elements to random subset
    for (int i = 0; i < 100; i++)
    {
        if (rand()%2 == 0)
        {
            s1.push_back(input[i]);
        }
        // if not in random subset, add to another subset
        else {
            s2.push_back(input[i]);
        }
    }
    return make_pair(s1,s2);
}


    // for (int i = 0; i < 100 ; i++){
    // for (int i = 0; i < 100; i++){
    //     if (rand()%2 == 0){

    //     }
    //     else{
    //         s1
    //     }
    // }
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
vector a;
for (int i = 0; i < 100; i++)
{
    a.push_back(rand())
}

array <unsigned long long , 100> repeated_random(){
    // Create random solution
}


// hill climbing

//  simmulated annealing


int main(){
    array<array <unsigned long long , 100>,100> chicken = rand_array();
    karmarkar(chicken);
    // for (int i = 0; i < 11; i++){
    //     printf("number: %llu\n", chicken[0][i]); 
    // }
    printf("try her out: %d\n", abs(-9));
    array <unsigned long long , 100>::iterator first_max;
    first_max = max_element(chicken[0].begin(), chicken[0].end());
    // printf("max: %llu\n", (long long unsigned) *max_element(chicken[0].begin(), chicken[0].end()));
    
} 