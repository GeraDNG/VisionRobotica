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
     printf(" Enter name of *.ppm INPUT image file (example: lena.ppm) ");
     scanf("%s",&infile);
     
     //Check if input file exists
     if ((infptr = fopen(infile, "rb")) == NULL)
     {
       printf(" Cannot open input image file: %s\n",infile);
       printf(" Exiting program...\n");
       system("PAUSE");
       exit(1);
       }
     
     printf(" Enter name of *.ppm OUTPUT image file (example: lenaout.ppm) ");
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
     
     //Check if file is P6 (ppm) format
     if (buffer[1] == '6')															// Changed to 6 because we are using PPM files
       printf("\n Input file is ppm, OK\n");
     else
     {
       printf("\n Input file is NOT ppm, Exiting program...\n");
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
     fprintf(outfptr, "P6\n%d %d\n%d\n",NCols,MRows,255);							// Changed to P6 because we are using PPM files and change 255 to 512
} //addhdr ()

//----------------------------------------------------------------------------//
//         User defined section                                               //        
//         Your preprocessors/algorithms will be defined here                 //
//----------------------------------------------------------------------------//

void userdefined ()
{
	// Define the local variables:
	unsigned char Matrix[NCols][MRows];
	unsigned char Red,Green,Blue;
	unsigned char OldPixelValue;
	unsigned char NewPixelValue;
	int Column, Row;
	
	// Import the original image an store it in Matrix:
	for( Column = 0; Column < NCols; Column++ ){
		for( Row = 0; Row < MRows; Row++ ){
			Matrix[Column][Row] = fgetc( infptr );
		}
	}
	
	// Apply the gamma correction by 0.5:
	OldPixelValue = fgetc(infptr);
	do{
		NewPixelValue = 255*(pow((OldPixelValue/255),(1/0.5)));
		OldPixelValue = fgetc(infptr);
	} while(!feof(infptr));
	
	// Apply the gamma correction by 1:
	//OldPixelValue = fgetc(infptr);
	//do{
	//	NewPixelValue = 255*(OldPixelValue/255);
	//	OldPixelValue = fgetc(infptr);
	//} while(!feof(infptr));
	
	// Apply the gamma correction by 2:
	//OldPixelValue = fgetc(infptr);
	//do{
	//	NewPixelValue = 255*(pow((OldPixelValue/255),(0.5)));
	//	OldPixelValue = fgetc(infptr);
	//} while(!feof(infptr));
	
	// Save the output image:
	for( Column = 0; Column < NCols; Column++ ){
		for ( Row = 0; Row < MRows; Row++ ){
			fputc( Matrix[Column][Row], outfptr );
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
