//----------------------------------------------------------------------------//
// Template for reading portable gray map files (*.pgm)                       //
//                                                                            //
//                                                RickWare                    //
//                                                August 23, 2018             //
//                                                                            //
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
//         Prototype declarations                                             //
//----------------------------------------------------------------------------//

void heading (void);      /* Displays program heading */
void openfiles (void);    /* Opens input and output files in binary */
void userdefined (void);  /* Here, the user will implement his routine */
void readhdr (void);      /* Read header from input image file */
void addhdr (void);       /* Add header to output image file */
void closefiles (void);   /* Close all files used in program */

//----------------------------------------------------------------------------//
//         Include declarations                                               //
//----------------------------------------------------------------------------//

# include <stdio.h>
# include <math.h>
# include <stdlib.h>
# include <string.h>

//----------------------------------------------------------------------------//
//         Variable declarations                                              //
//----------------------------------------------------------------------------//

int    MRows, NCols;                 /* Number of Rows and columns in image */
FILE   *infptr, *outfptr, *outfptrh; /* Input and output file pointers */
char   infile[40];                   /* Name of input file */
char   outfile[40];                  /* Name of output file */

//----------------------------------------------------------------------------//
//         Main program                                                       //
//----------------------------------------------------------------------------//

main ()
{
     // Display heading
     heading ();
     // Open Input and Output Image Files
     openfiles ();
     // Read header from image file
     readhdr();
     // Add header to output file
     addhdr();
     // Execute userdefined procedure
     userdefined ();
     // Close all image files
     closefiles ();
     
     printf("\n Bye! Bye!\n");
     system("PAUSE");
     return(1);
} // end main ();

//----------------------------------------------------------------------------//
//         Headings                                                           //
//----------------------------------------------------------------------------//

void heading ()
{
     int i;
     for (i=0; i<16; i++)
       printf("                                    +                       \n");
       printf("                      Digital Image Processing in C         \n");
       printf("                          Date: August 23, 2018             \n");
       printf("                            Author: RickWare                \n");
     for (i=0; i<3; i++)
       printf("                                    +                       \n");
       return;
} // end heading ()

//----------------------------------------------------------------------------//
//         Openfiles                                                          //
//----------------------------------------------------------------------------//

void openfiles ()
{
     printf("\n Opening Input and Output image files\n");
     printf(" Enter name of *.pgm INPUT image file (example: lena.pgm) ");
     scanf("%s",&infile);
     
     //Check if input file exists
     if ((infptr = fopen(infile, "rb")) == NULL)
     {
       printf(" Cannot open input image file: %s\n",infile);
       printf(" Exiting program...\n");
       system("PAUSE");
       exit(1);
       }
     
     printf(" Enter name of *.pgm OUTPUT image file (example: lenaout.pgm) ");
     scanf("%s",&outfile);
     
     //Check if output file was created succesfully
     if ((outfptr = fopen(outfile, "wb")) == NULL)
     {
       printf(" Cannot create output image file: %s\n",outfile);
       printf(" Exiting program...\n");
       system("PAUSE");
       exit(1);
       }
       
     // If this point is reached, file are OK
     printf(" File opened and created OK! \n\n");
     
}  //end openfiles ()

//----------------------------------------------------------------------------//
//         Read Image File Header                                             //
//----------------------------------------------------------------------------//

void readhdr ()
{
     int   i=0, MaxRGB;
     char  c, buffer[128];
     
     //Read header of PGM file
     printf("\n\n File <%s> Header: \n",infile);
     
     do
     {
       c = fgetc(infptr);
       buffer[i] = c;
       i++; 
     } while (c != '\n');
     
     //Check if file is P5 (pgm) format
     if (buffer[1] == '5')
       printf("\n Input file is pgm, OK\n");
     else
     {
       printf("\n Input file is NOT pgm, Exiting program...\n");
       system("PAUSE");
       exit(0);
     }
     
     //File is pgm, read rest of header
     fscanf(infptr,"%d %d %d",&NCols, &MRows, &MaxRGB);
     //Make file pointer point to the first byte of the image
     c = fgetc(infptr);
     //Image info
     printf("%d ",NCols);
     printf("%d   <---- Width x Height) \n",MRows);
     printf("%d   <---- Max. Grayscale level) \n\n",MaxRGB);
}  // end readhdr ()

//----------------------------------------------------------------------------//
//         Add Image Header to output file                                    //
//----------------------------------------------------------------------------//

void addhdr ()
{
     fprintf(outfptr, "P5\n%d %d\n%d\n",NCols,MRows,255);
} //addhdr ()

//----------------------------------------------------------------------------//
//         User defined section                                               //        
//         Your preprocessors/algorithms will be defined here                 //
//----------------------------------------------------------------------------//

void userdefined ()
{
	// Define local variables:
	unsigned char Matrix[NCols][MRows];
	int Column, Row, i, Num;
	unsigned char Pixel[9];
	
	
	// Import the original image an store it in Matrix:
	for( Column = 0; Column < NCols; Column++ ){
		for( Row = 0; Row < MRows; Row++ ){
			Matrix[Column][Row] = fgetc( infptr );
		}
	}
	
	// Read pixels:
	for( Column = 0; Column < NCols; Column++ ){
		for( Row = 0; Row < MRows; Row++ ){
			//Pixel1 = Matrix[Column-1][Row-1];
			//Pixel2 = Matrix[Column][Row-1];
			//Pixel3 = Matrix[Column+1][Row-1];
			//Pixel4 = Matrix[Column-1][Row];
			//Pixel5 = Matrix[Column][Row];
			//Pixel6 = Matrix[Column+1][Row];
			//Pixel7 = Matrix[Column-1][Row+1];
			//Pixel8 = Matrix[Column][Row+1];
			//Pixel9 = Matrix[Column+1][Row+1];
			
			// Cheking for border pixels:
			if(Column == 0 && Row == 0){
				fputc(Matrix[Column][Row], outfptr);
			}
			else if (Column == 0 && Row == 512){
				fputc(Matrix[Column][Row], outfptr);
			}
			else if (Column == 512 && Row == 0){
				fputc(Matrix[Column][Row], outfptr);
			}
			else if (Column == 512 && Row == 512){
				fputc(Matrix[Column][Row], outfptr);
			}
			else{
				// Getting the pixels values:
				Pixel[0] = Matrix[Column-1][Row-1];
				Pixel[1] = Matrix[Column][Row-1];
				Pixel[2] = Matrix[Column+1][Row-1];
				Pixel[3] = Matrix[Column-1][Row];
				Pixel[4] = Matrix[Column][Row];			// Central Pixel
				Pixel[5] = Matrix[Column+1][Row];
				Pixel[6] = Matrix[Column-1][Row+1];
				Pixel[7] = Matrix[Column][Row+1];
				Pixel[8] = Matrix[Column+1][Row+1];
			}
			
			// Making the order and selecting the median:
			for( i = 0; i < 9; i++ ){
				if(Pixel[i] > Pixel[i+1]){
					Num = Pixel[i];
					Pixel[i] = Pixel[i+1];
					Pixel[i+1] = Num;
				}
			}
			
			// Saving the output image:
			fputc( Pixel[4], outfptr ); 
			
		}
	}
	
	
	
	

}  // end userdefined ()

//----------------------------------------------------------------------------//
//         Close Files                                                        //
//----------------------------------------------------------------------------//

void closefiles ()
{
     fclose(infptr);
     fclose(outfptr);
}  // end closefiles ()
