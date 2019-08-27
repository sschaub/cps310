#include <amp.h>
#include <vector>


using namespace concurrency;

#define VER_SER

void ocean (std::vector<int>& gridv, int xdim, int ydim, int timesteps)
{
    /********************* the red-black algorithm (start)************************/
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

    array_view<int, 2> grid(xdim, ydim, gridv);

#ifdef VER_SER
    for (int ts=0; ts<timesteps; ts++) {
        for(int i=1; i<ydim-1; i++) {
            int offset = (i+ts)%2;
            for(int j=1+offset; j<xdim-1; j+=2) {
                grid[i][j] = (grid[i][j] + grid[i-1][j] + grid[i+1][j]
                                + grid[i][j-1] + grid[i][j+1])/5;
            }
        }
    }
#endif

#ifdef VER_SIMPLE_AMP

	

	// PUT YOUR CODE HERE
    
#endif


    /////////////////////// the red-black algorithm (end) ///////////////////////////
}