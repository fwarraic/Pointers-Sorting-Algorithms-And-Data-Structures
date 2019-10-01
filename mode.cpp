#include <iostream>
#include <fstream>
using namespace std;

// Here I've left the code as I found it, to show you
// a contrasting (and in my opinion, nowhere near as clear)
// style.  Many C++ coders have this style.

void merge(long *array, int l, int m, int r) {
   int i, j, k, nl, nr;

   //size of left and right sub-arrays
   nl = m-l+1; nr = r-m;
   long larr[nl], rarr[nr];

   //fill left and right sub-arrays
   for(i = 0; i<nl; i++)
      larr[i] = array[l+i];
   for(j = 0; j<nr; j++)
      rarr[j] = array[m+1+j];
   i = 0; j = 0; k = l;

   //merge temp arrays to real array
   while(i < nl && j<nr) {
      if(larr[i] <= rarr[j]) {
         array[k] = larr[i];
         i++;
      }else{
         array[k] = rarr[j];
         j++;
      }
      k++;
   }

   while(i<nl) {       //extra element in left array
      array[k] = larr[i];
      i++; k++;
   }

   while(j<nr) {     //extra element in right array
      array[k] = rarr[j];
      j++; k++;
   }
}
void mergeSort(long *array, int l, int r) {
   if(l < r) {
      int m = l+(r-l)/2;

      // Sort first and second arrays
      mergeSort(array, l, m);
      mergeSort(array, m+1, r);
      merge(array, l, m, r);
   }
}


// Find the mode of an array.
//
// Parameters:
//  arr is an integer array with n elements
// Return value:
//  return the mode
//
long mode (long *arr, int n) {
    // TO DO:: Fill in this function.

	mergeSort(arr, 0, n - 1);

    // Return nothing if array is empty.
	if (n == 0) {
		return -1;
	}

	// Return the array[0] if only one element.
	if (n == 1) {
		return arr[0];
	}
	
	long finalMode;
	int finalCount = 0;
    for (int i = 0; i < n; ++i) {
		
		int currentCount = 0;
    	for (int j = 0; j < n; ++j) {
    		if (arr[i] == arr[j]) {
    			currentCount++;
    		}
    	}

    	if (currentCount >= finalCount) {
    		finalCount = currentCount;
    		finalMode = arr[i];
    	}
    }
    return finalMode;
}


// Input is the number of elements, followed by that many integers.
// Writes mode of elements to file specified as first command-line argument.
int main (int argc,  char* argv[]) {

	// ofstream for writing result.
	ofstream outputfile;

	// Parse command-line arguments.
	if (argc != 2) {
		// Note that the program name is the first argument, so argc==1 if there are no additional arguments.
		cerr << "Expected one argument." << endl;
		cerr << "  Usage: " << argv[0] << " output_filename" << endl;
		return 1;
	} else {
		// Open the filename specified for input.
		outputfile.open (argv[1]);
	}


	// Read a set of elements into an array.
	int n;
	long *arr;

	// Get the number of elements
	cout << "Enter the number of elements:" << endl;
	cin >> n;

	// Create array in dynamic memory.
	arr = new long[n];

	for (int i=0; i<n; i++) {
		cout << "Enter a number:" << endl;
		cin >> arr[i];
	}

	long m = mode(arr,n);

	cout << "The mode is " << m << endl;

	outputfile << m << endl;
	outputfile.close();

	// Free up memory.  Note use of delete [], since this is an array.
	delete [] arr;

	return 0;
}
