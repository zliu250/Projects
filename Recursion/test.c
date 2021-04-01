#include <stdio.h>
#include <stdlib.h>

// MACRO for printing a newline
#define NEWLINE printf("\n")

// This function is to perform a recursion fill, which check if the index should be filled
//	or not and ultimately it will end searching if the index has reaches the boundary or
//	out of bounds
void boundary_fill(int **image, int boundary, int fill, int row, int col, int row_idx, int col_idx){

	if( row_idx < 0 || row_idx >= row || col_idx < 0 || col_idx >= col ){
		return;
	}

	// check on the index's NSWE direction recursively if the index contain
	// a non boundary number nor a fill number
	if(image[row_idx][col_idx] != fill && image[row_idx][col_idx] != boundary){
		image[row_idx][col_idx] = fill;
		boundary_fill(image, boundary, fill, row, col, row_idx+1, col_idx);
		boundary_fill(image, boundary, fill, row, col, row_idx-1, col_idx);
		boundary_fill(image, boundary, fill, row, col, row_idx, col_idx+1);
		boundary_fill(image, boundary, fill, row, col, row_idx, col_idx-1);
	}
	return;
}


int main(void) {
    int rows, cols, polygon, fill, row_idx, col_idx = 0; // initialize variables
    
    scanf("%d %d\n",&rows,&cols);

	// dynamically allocate a 2d array to store the image
    int ** image = (int **)calloc(rows , sizeof(int *));
    for(int i = 0;i < rows;i++){
        image[i] = (int *)calloc(cols , sizeof(int));
    }

    scanf("%d\n",&polygon);
    scanf("%d\n",&fill);
    scanf("%d %d\n",&row_idx,&col_idx);

	// fill the array
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            scanf("%4d",&image[i][j]);
        }
    }

	printf("input: \n\n");
	for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            printf("%4d",image[i][j]);
        }
        NEWLINE;
    }

	boundary_fill(image, polygon, fill, rows, cols, row_idx, col_idx);

    NEWLINE;
    printf("output: \n\n");

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            printf("%4d",image[i][j]);
        }
        NEWLINE;
    }

	// destructor for 2d array
	for(int i = 0; i < rows; i ++){
		free(image[i]);
	}
	free(image);
	image = NULL;
	
    return 0;
}
