#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <list>
using namespace std;
int n, k, a[1000000], dis;//n for cities, k for range, a to store list of city, dis for range cover
int current = 0; // initializing the starting city, after that should be the current point

int main() {
    
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */   
    
    //scanning value
    scanf("%d %d", &n, &k);
    for(int i=0; i<n; i++){
        scanf("%d", &a[i]);
    }
    /*
    for(int i=0; i<n; i++){
        cout<<a[i];
    } //test case for list 
   */
    
    dis = k - 1;
    int count =0;
    while (current < n) { //while current point not over the last city
        int temp = dis;  //temp varibale for cover range in list. NOT euqal to "k", the range for tower.
        if ((current+temp) > n) // go over range then set position to the last
            temp = n - current - 1;   
        while ((current+temp)>=0 && a[current+temp]==0) //while the city has no tower, go back. Won't out of the list
            temp--;
        if ( (current+temp) < 0 || (current+k+temp) == current && a[current+temp+1] == 0) { // failure case. 
            // (current+temp) < 0 for test case where all city has no tower
            // (current+k+temp) == current && a[current+temp+1] == 0) for test case where if it goest back and point to the last tower which has swith on and the next city has no tower such as  8 2 0 1 0 0 0 0 0 0. 
            count = -1;
            break;
        } else {
            count++;
            current += temp + k;
        }        
    }
    printf("%d", count);
}
