#include <vector>


using namespace std;


void ocean (vector<int>& grid, int xdim, int ydim, int timesteps)
{
    /********************* the red-black algortihm (start)************************/
    /*
    In odd timesteps, calculate indeces with - and in even timesteps, calculate indeces with * 
    See the example of 6x6 matrix, A represents the corner elements. 
        A A A A A A
        A - * - * A
        A * - * - A
        A - * - * A
        A * - * - A
        A A A A A A 
    */

    

    for (int ts=0; ts<timesteps; ts++) {
        for(int i=1; i<ydim-1; i++) {
            int offset = (i+ts)%2;
            for(int j=1+offset; j<xdim-1; j+=2) {
                grid.at(i * ydim + j) = (grid.at(i * ydim + j) + grid.at((i-1)*ydim + j) + grid.at((i+1) * ydim + j)
                                + grid.at(i * ydim + (j-1)) + grid.at(i * ydim + (j + 1)))/5;
            }
        }
    }



    /////////////////////// the red-black algorithm (end) ///////////////////////////
}