/*-------------------------------------------
Program 2: Food web analysis with dynamic memory
Course: CS 211, Fall 2023, UIC
System: Visual Studio Code
Author: Zaheer Safi
------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Org_struct 
{
    char name[20];
    int* prey;   
    int numPrey;
} Org;

// function to reallocate memeory for web dynamicaly allocated arry which stores the structs org
void buildWeb(Org* web, int numOrg, int predInd, int preyInd) 
{
    // if the web array has no elemets just reallocate for one space
    if (web[predInd].prey == NULL)
    {
        int size =  web[predInd].numPrey;
        web[predInd].prey = (int *)malloc(sizeof(int));
        web[predInd].prey[0] = preyInd;
        web[predInd].numPrey = web[predInd].numPrey + 1;
    }
    
    // otherwise also reallocate the array but in a different way to hold all the previous values
    else
    {
        int size =  web[predInd].numPrey; // store the size of prey array at the given index
        int * arr = (int *)malloc((size + 1) * sizeof(int)); // make a new dynamically allocated memeory
        
        // transfer all the elements
        for (int i = 0; i < size; i++)
        {
            arr[i] = web[predInd].prey[i];
        }
        // free the prey array at the given index
        free(web[predInd].prey);
        // now make the prey array at the given index equal to the new allocated memeory
        web[predInd].prey = arr;
        // change the last element in the prey array
        web[predInd].prey[size] = preyInd;
        // increment the numPrey which accounts for the size of prey array
        web[predInd].numPrey = web[predInd].numPrey + 1;
    }    
}


// function to handle all the changes after the user asks for an extinction of predator
void extinction(Org** web, int* numOrgs, int index) 
{
    // if web is pointing to null just return
    if (*web == NULL)
    {
        return;
    }
    
    // if web has only one element and the user wants to extinct it just make the web pointing to null and return
    else if (*numOrgs == 1 && index == 0)
    {
        free((*web)[index].prey);
        *web = NULL;
        (*numOrgs)--;
        return;
    }
    
    // otherwise if web has more then one element
    
    // free all the space that the prey array occupies at the given index
    free((*web)[index].prey);
    Org* new_web = (Org*)malloc((*numOrgs - 1) * sizeof(Org)); // make a new web array
    
    // transfer all the elements upto the one we want to remove
    for (int i = 0; i < index; i++)
    {
        new_web[i] = (*web)[i];  
    }
    
    // tranfer all the elements after the extenction 
    for (int i = index; i < *numOrgs - 1; i++)
    { 
        new_web[i] = (*web)[i + 1];  
    }
    
    // do the same steps as build index function to complete the reallocation
    free(*web);
    *web = new_web;
    (*numOrgs)--;


    // now that the index is removed from the web array go to each prey array in web and also remove the element from it
    for (int i = 0; i < *numOrgs; i++)
    {
        for (int iterations = 0; iterations < (*web)[i].numPrey; iterations++)
        {
            // if the an element is array at web[i] is equal to index remove it and reallocate the arry by removing an element
            if ((*web)[i].prey[iterations] == index)
            {
                int *new_prey = (int *)malloc(((*web)[i].numPrey - 1) * sizeof(int));
                for (int j = 0; j < iterations; j++)
                { 
                    new_prey[j] = (*web)[i].prey[j];   
                }
                for (int p = iterations; p < (*web)[i].numPrey - 1; p++)
                { 
                    new_prey[p] = (*web)[i].prey[p + 1];
                    
                }
                
                free((*web)[i].prey);
                (*web)[i].prey = new_prey;
                (*web)[i].numPrey = (*web)[i].numPrey - 1;
            }
        }
    }
    
    // now every element that is greater than the index subtract one from it to account for the reallocation
    for (int i = 0; i < *numOrgs; i++)
    {
        for (int iterations = 0; iterations < (*web)[i].numPrey; iterations++)
        {
            if ((*web)[i].prey[iterations] > index)
            {
                (*web)[i].prey[iterations] = (*web)[i].prey[iterations] - 1;
            }
        }
    }
    return;
}


int main(void) {

    int numOrgs;
    printf("Welcome to the Food Web Application\n");
    printf("--------------------------------\n");
    printf("Enter number of organisms:\n");
    scanf("%d",&numOrgs);

    Org* web = NULL;
    if(numOrgs > 0) { //Do not malloc an empty array, leave it pointing to NULL
        web = (Org*)malloc(numOrgs*sizeof(Org));
    }
    
    printf("Enter names for %d organisms:\n", numOrgs);
    for (int i = 0; i < numOrgs; ++i) {
        scanf("%s",web[i].name);
        web[i].prey = NULL;
        web[i].numPrey = 0;
    }

    printf("Enter number of predator/prey relations:\n");
    int numRels;
    scanf("%d",&numRels);

    printf("Enter the pair of indices for the %d predator/prey relations\n",numRels);
    printf("the format is [predator index] [prey index]:\n");
    
    int predInd, preyInd;
    for (int i = 0; i < numRels; ++i) {
        scanf("%d %d",&predInd, &preyInd);
        buildWeb(web,numOrgs,predInd,preyInd);
    }
    printf("--------------------------------\n\n");

    printf("Food Web Predators & Prey:\n");

    // print out what preadtor eats what according to the user input or from file reading
    int arrNum = 0; // to store the value of prey arry element
    for (int i = 0; i < numOrgs; i++)
    {
        // if prey array is not empty
        if (web[i].prey != NULL)
        {
            printf("  %s eats", web[i].name); // print out the name of predator
            
            // print out whatever it eats
            for (int j = 0; j < web[i].numPrey; j++)
            {
                arrNum = web[i].prey[j];
                if (j == 0)
                {
                    printf(" %s", web[arrNum].name);
                }
                else
                {
                    printf(", %s", web[arrNum].name);
                }
            }
            
            
        }
        
        // if the prey arry is empty it means it does not eat anything simply just print out the name
        else if (web[i].prey == NULL)
        {
            printf("  %s", web[i].name);
        }
        printf("\n");
    }

    printf("\n");

    printf("Apex Predators:\n");
    
    // print out predators that are not eaten by anyone
    for (int i = 0; i < numOrgs; i++) 
    {
        bool isApex = true;
        
        // go through each element in prey arry and if it is  not eaten by any predator make isApex equal to true
        for (int j = 0; j < numOrgs; j++) 
        {
            if (i != j) 
            {
                for (int iterations = 0; iterations < web[j].numPrey; iterations++) 
                {
                    if (web[j].prey[iterations] == i) 
                    {
                        isApex = false;
                        break;
                    }
                }
                if (!isApex) 
                {
                    break;
                }
            }
        }
        
        // print out the names
        if (isApex) 
        {
            printf("  %s\n", web[i].name);
        }
    }
    printf("\n");

    printf("Producers:\n");

    //print out th elements that does not eat anything like plants
    for (int i = 0; i < numOrgs; i++)
    {
        // print the ones with empty prey array which means they dont eat any other predator
        if (web[i].prey == NULL)
        {
            printf("  %s\n", web[i].name);
        }
    }
    printf("\n");

    printf("Most Flexible Eaters:\n");

    // predator in the web array that eats the maximum amount of other predators
    int max = web[0].numPrey;
    
    // go through each prey size and find the highest one store it in max
    for (int i = 0; i < numOrgs; i++)
    {
       if (max < web[i].numPrey)
       {
            max = web[i].numPrey;
       }

    }
    
    // every prey arry that has size max print it out
    for (int i = 0; i < numOrgs; i++)
    {
       if (web[i].numPrey == max)
       {
            printf("  %s\n", web[i].name);
       }

    }
    printf("\n");

    printf("Tastiest Food:\n");
    // predators that are eaten by the highest amount of other predators
    
    int* preyCount = (int*)malloc(numOrgs * sizeof(int)); // make a new dynamically allocated array

    // intialize it with zeros
    for (int i = 0; i < numOrgs; i++) 
    {
        preyCount[i] = 0;
    }

    for (int i = 0; i < numOrgs; i++) 
    {
        for (int j = 0; j < web[i].numPrey; j++) 
        {
            preyCount[web[i].prey[j]]++;
        }
    }

    int maxPreyEaten = -1;

    // find out the element whihc is eaten by highest amount of other predators and store it in maxpreyeaten
    for (int i = 0; i < numOrgs; i++) 
    {
        if (preyCount[i] > maxPreyEaten) 
        {
            maxPreyEaten = preyCount[i];
        }
    }

    // go through preycount and print the one with maxPreyeatn value
    for (int i = 0; i < numOrgs; i++) 
    {
        if (preyCount[i] == maxPreyEaten) 
        {
            printf("  %s\n", web[i].name);
        }
    }

    free(preyCount); 
    printf("\n");

    printf("Food Web Heights:\n");
    
    bool changesMade; // A flag to keep track of whether changes were made in the loop
    int* heights = (int*)malloc(numOrgs * sizeof(int)); // An array to store heights of organisms in the food web

    // Initialize heights array to 0 for all organisms
    for (int i = 0; i < numOrgs; ++i) 
    {
        heights[i] = 0; 
    }

    int iterations = 0; 

    // Perform iterations until no more changes are made or the number of iterations reaches numOrgs
    do {
        changesMade = false; 

        // Loop through all organisms in the food web
        for (int i = 0; i < numOrgs; ++i) 
        {
            int maxPreyHeight = -1; 

            // Loop through the prey of the current organism
            for (int j = 0; j < web[i].numPrey; ++j) 
            {
                int preyIndex = web[i].prey[j];
                
                // Find the maximum height among the prey
                if (heights[preyIndex] > maxPreyHeight) 
                {
                    maxPreyHeight = heights[preyIndex];
                }
            }

            // If the height of the current organism is not one more than the maximum prey height
            if (heights[i] != maxPreyHeight + 1) 
            {
                heights[i] = maxPreyHeight + 1; 
                changesMade = true; 
            }
        }
        iterations++;
    } while (changesMade && iterations != numOrgs); // Continue while changes were made and not all organisms have been checked

    // Print the heights of organisms in the food web
    for (int i = 0; i < numOrgs; ++i) 
    {
        printf("  %s: %d\n", web[i].name, heights[i]);
    }
    printf("\n");

    printf("Vore Types:\n");

    // print out the producers as above
    printf("  Producers:\n");
    for (int i = 0; i < numOrgs; i++)
    {
        if (web[i].prey == NULL)
        {
            printf("    %s\n", web[i].name);
        }
    }
    
    // print out herbivores only eat producers (i.e. only plants) using heights array
    printf("  Herbivores:\n");
    for (int i = 0; i < numOrgs; i++)
    {
        if (heights[i] == 1)
        {
            printf("    %s\n", web[i].name);
        }
    }
    
    // print out omnivores eats producers and non-producers using heights array again
    printf("  Omnivores:\n");
    
    int numArr1 = 0;
    for (int i = 0; i < numOrgs; i++)
    {
        // check that only thing that eats are producers
        bool produce = false;
        for (int j = 0; j < web[i].numPrey; j++)
        {
            numArr1 = web[i].prey[j];
            if (heights[numArr1] == 0)
            {
                produce = true;
            }
        }
        if (heights[i] > 1 && produce)
        {
            printf("    %s\n", web[i].name);
        }
    }
    
    printf("  Carnivores:\n");
    
    // print carniovers using height array it eats more than one thing and not only producers
    for (int i = 0; i < numOrgs; i++)
    {
        bool produce = false;
        for (int j = 0; j < web[i].numPrey; j++)
        {
            numArr1 = web[i].prey[j];
            if (heights[numArr1] == 0)
            {
                produce = true;
            }
        }
        if (heights[i] > 1 && !produce)
        {
            printf("    %s\n", web[i].name);
        }
    }


    printf("\n");
    free(heights);

    printf("--------------------------------\n");
    int extInd;
    printf("Enter extinct species index:\n");
    scanf("%d",&extInd);
    printf("Species Extinction: %s\n", web[extInd].name);
    extinction(&web,&numOrgs,extInd);
    printf("--------------------------------\n\n");

    
    // printout all the results again after the extension of a predator the sameway just with different veriable names
    
    
    printf("UPDATED Food Web Predators & Prey:\n");
    for (int i = 0; i < numOrgs; i++)
    {
        if (web[i].prey != NULL)
        {
            printf("  %s eats", web[i].name);
            
            for (int j = 0; j < web[i].numPrey; j++)
            {
                int run = web[i].prey[j];
                if (j == 0)
                {
                    printf(" %s", web[run].name);
                }
                else
                {
                    printf(", %s", web[run].name);
                }
            }    
            
        }

        else if (web[i].prey == NULL)
        {
            printf("  %s", web[i].name);
        }
        printf("\n");
    }


    printf("\n");

    printf("UPDATED Apex Predators:\n");
    for (int i = 0; i < numOrgs; i++) 
    {
        bool isApex = true;
        for (int j = 0; j < numOrgs; j++) 
        {
            if (i != j) 
            {
                for (int iterations = 0; iterations < web[j].numPrey; iterations++) 
                {
                    if (web[j].prey[iterations] == i) 
                    {
                        isApex = false;
                        break;
                    }
                }
                if (!isApex) 
                {
                    break;
                }
            }
        }
        if (isApex) 
        {
            printf("  %s\n", web[i].name);
        }
    }
    printf("\n");

    printf("UPDATED Producers:\n");
    for (int i = 0; i < numOrgs; i++)
    {
        if (web[i].prey == NULL)
        {
            printf("  %s\n", web[i].name);
        }
    }
    printf("\n");

    printf("UPDATED Most Flexible Eaters:\n");
    int max1 = web[0].numPrey;
    for (int i = 0; i < numOrgs; i++)
    {
       if (max1 < web[i].numPrey)
       {
            max1 = web[i].numPrey;
       }

    }
    for (int i = 0; i < numOrgs; i++)
    {
       if (web[i].numPrey == max1)
       {
            printf("  %s\n", web[i].name);
       }

    }
    printf("\n");

    printf("UPDATED Tastiest Food:\n");
    
    int* preyCount1 = (int*)malloc(numOrgs * sizeof(int));

    for (int i = 0; i < numOrgs; i++) 
    {
        preyCount1[i] = 0;
    }

    for (int i = 0; i < numOrgs; i++) 
    {
        for (int j = 0; j < web[i].numPrey; j++) 
        {
            preyCount1[web[i].prey[j]]++;
        }
    }

    int maxPreyEaten1 = -1;

    for (int i = 0; i < numOrgs; i++) 
    {
        if (preyCount1[i] > maxPreyEaten1) 
        {
            maxPreyEaten1 = preyCount1[i];
        }
    }

    for (int i = 0; i < numOrgs; i++) {
        if (preyCount1[i] == maxPreyEaten1) 
        {
            printf("  %s\n", web[i].name);
        }
    }

    free(preyCount1);
    printf("\n");

    printf("UPDATED Food Web Heights:\n");
    bool changesMade1;
    int* heights_updated = (int*)malloc(numOrgs * sizeof(int));

    for (int i = 0; i < numOrgs; ++i) 
    {
        heights_updated[i] = 0; 
    }
    int iterations1 = 0;
    do {
        changesMade1 = false;

        for (int i = 0; i < numOrgs; ++i) 
        {
            int maxPreyHeight2 = -1; 

            for (int j = 0; j < web[i].numPrey; ++j) 
            {
                int preyIndex = web[i].prey[j];
                if (heights_updated[preyIndex] > maxPreyHeight2) {
                    maxPreyHeight2 = heights_updated[preyIndex];
                }
            }

            if (heights_updated[i] != maxPreyHeight2 + 1) 
            {
                heights_updated[i] = maxPreyHeight2 + 1;
                changesMade1 = true;
            }
        }
        iterations1++;
    
    } while (changesMade1 && iterations1 != numOrgs);

    for (int i = 0; i < numOrgs; ++i) 
    {
        printf("  %s: %d\n", web[i].name, heights_updated[i]);
    }
    printf("\n");

    printf("UPDATED Vore Types:\n");
    
    printf("  Producers:\n");
    for (int i = 0; i < numOrgs; i++)
    {
        if (web[i].prey == NULL)
        {
            printf("    %s\n", web[i].name);
        }
    }
    
    printf("  Herbivores:\n");
    for (int i = 0; i < numOrgs; i++)
    {
        if (heights_updated[i] == 1)
        {
            printf("    %s\n", web[i].name);
        }
    }
    
    bool produce1 = false;
    printf("  Omnivores:\n");
    for (int i = 0; i < numOrgs; i++)
    {
        produce1 = false;
        for (int j = 0; j < web[i].numPrey; j++)
        {
            int num4 = web[i].prey[j];
            if (heights_updated[num4] == 0)
            {
                produce1 = true;
            }
        }
        if (heights_updated[i] > 1 && produce1)
        {
            printf("    %s\n", web[i].name);
        }
    }
    
    bool produce2 = false;
    printf("  Carnivores:\n");
    for (int i = 0; i < numOrgs; i++)
    {
        produce2 = false;
        for (int j = 0; j < web[i].numPrey; j++)
        {
            int num5 = web[i].prey[j];
            if (heights_updated[num5] == 0)
            {
                produce2 = true;
            }
        }
        if (heights_updated[i] > 1 && !produce2)
        {
            printf("    %s\n", web[i].name);
        }
    }

    printf("\n");
    free(heights_updated);
    printf("--------------------------------\n");

    // free all the allocated memory
    for (int i = 0; i < numOrgs; i++) 
    {
        free(web[i].prey);
    }
    free(web);

    return 0;
}
