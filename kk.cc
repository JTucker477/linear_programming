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
using namespace std;
int max_iter = 1; 
typedef long long ll;

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

}

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



array <ll, 100 > random_solution(){
    // array <ll , 100> s1;
    // array <ll , 100> s2;
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

array <ll , 100> repeated_random (array<array <ll , 100>,100> input){
    array <ll , 100> final; 
    
    // For each of the 100 instances of the problem
    for (int i = 0; i < 1; i++){
        array <ll , 100> solution;
        array <ll , 100> multiply_result;
        solution = random_solution();
        multiply_result = solution; 
        ll lowest_residue;
        // element wise multiplication of input Result is stored in multiply_result. input is unchanged
        transform( input[i].begin(), input[i].end(),
                    multiply_result.begin(), multiply_result.end(),  
                    std::multiplies<int>() );
        lowest_residue = accumulate(begin(multiply_result), end(multiply_result),0);

        // The actual iteration 
        for (int i = 0; i < max_iter ; i ++ ){
            
           
            // Try a new random solution
            array <ll , 100> new_sol = random_solution();
            array <ll , 100> temp_multiply_result;
            temp_multiply_result = new_sol;
            // Find residue result 
            transform( input[i].begin(), input[i].end(),
                    temp_multiply_result.begin(), temp_multiply_result.end(),  
                    std::multiplies<int>() );

            printf("temp_multiply_result[0] %lld\n",temp_multiply_result[0] );
            
            ll current_residue = accumulate(begin(temp_multiply_result), end(temp_multiply_result),0);

            if (current_residue < lowest_residue){
                lowest_residue = current_residue;
            }
        }

        final[i] = lowest_residue;
    }
    
    return final; 
}

// hill climbing

//  simmulated annealing


int main(){
    srand(time(NULL));
    array<array <ll , 100>,100> chicken = rand_array();
    // karmarkar(chicken);
    // // for (int i = 0; i < 11; i++){
    // //     printf("number: %llu\n", chicken[0][i]); 
    // // }
    // printf("try her out: %d\n", abs(-9));
    // array <ll , 100>::iterator first_max;
    // first_max = max_element(chicken[0].begin(), chicken[0].end());
    // printf("max: %llu\n", (long long unsigned) *max_element(chicken[0].begin(), chicken[0].end()));
    array <ll, 100> final;
    printf("hihihihihi\n");
    final = repeated_random(chicken);

    printf("hey girl\n");
    printf("result: %lld \n", final[0]);
} 
















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
