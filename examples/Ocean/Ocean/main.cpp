
#include <iostream>

#include <vector>
#include <amp.h>

using namespace std;
using namespace concurrency;

/* Implement this function in ocean.cpp */
extern void ocean (vector<int>& grid, int xdim, int ydim, int timesteps);

void printGrid(const vector<int>& grid, int xdim, int ydim);


inline double ElapsedTime(const LARGE_INTEGER& start, const LARGE_INTEGER& end)
{
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    return (double(end.QuadPart) - double(start.QuadPart)) * 1000.0 / double(freq.QuadPart);
}

int main(int argc, char* argv[])
{
    int xdim,ydim,timesteps;

	LARGE_INTEGER start, end;
	

    /********************Get the arguments  (start) **************************/
    /*
    Three input Arguments to the program
    1. X Dimension of the grid
    2. Y dimension of the grid
    3. number of timesteps the algorithm is to be performed
    */

    if (argc!=4) {
        printf("The Arguments you entered are wrong.\n");
        printf("./serial_ocean <x-dim> <y-dim> <timesteps>\n");
        return EXIT_FAILURE;
    } else {
        xdim = atoi(argv[1]);
        ydim = atoi(argv[2]);
        timesteps = atoi(argv[3]);
    }
    ///////////////////////Get the arguments  (end) //////////////////////////


    /*********************create the grid (start) ************************/
    vector<int> gridv(xdim * ydim);    
   
	for (int i=0; i<ydim; i++) {
        for (int j=0; j<xdim; j++) {
            //grid[i][j] = rand();
            if (i == 0 || j == 0 || i == ydim-1 || j == xdim-1) {
             gridv.at(i * ydim + j) = 1000;
            } else {
             gridv.at(i * ydim + j) = 0;
            }
        }
    }

	cout << "Grid starting state:" << endl;
	printGrid(gridv, xdim, ydim);
	cout << endl << "Running simulation...";

    ///////////////////////create the grid (end) //////////////////////////

    // Start the time measurment here before the algorithm starts
    QueryPerformanceCounter(&start);

    ocean(gridv, xdim, ydim, timesteps);

    QueryPerformanceCounter(&end);  

    double elapsedTime = ElapsedTime(start, end);

	cout << " completed." << endl;

    printGrid(gridv, xdim, ydim);  

	cout << "   Total time:  " << elapsedTime << " ms" << endl;

	cout << "Press enter to exit...";
	string line;
	getline(cin, line);
    return EXIT_SUCCESS;
}

void printGrid(const vector<int>& grid, int xdim, int ydim)
{
    for (int i=0; i<ydim; i++) {
        for (int j=0; j<xdim; j++) {
            printf("%05d  ", grid.at(i * ydim + j));
        }
        printf("\n");
    }
}
