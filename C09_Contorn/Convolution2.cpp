//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//        This program reads an input grayscale image in *.pgm format
//        and converts it into a negative grayscale image in *.pgm format
//        by taking the one's complement of every pixel in the file.
//
//------------------------------------------------------------------------------
//                  Version updated: August 24, 2009
//                               RickWare
//------------------------------------------------------------------------------

//==============================================================================
//------------------------P R O G R A M-----------------------------------------
//==============================================================================
//
//  main();
//     heading();               display program heading
//     openfiles();             opens input & output image files
//     negative();              convert original image to negative
//     readhdr();               reads input image file header
//     closefiles();            closes input and processed output image files
//------------------------------------------------------------------------------
void     heading(void);
void     openfiles(void);
void     negative(void);
void     readhdr(void);
void     closefiles();
//------------------------------------------------------------------------------
# include <stdio.h>
# include <math.h>
# include <stdlib.h>
# include <string.h>
//------------------------------------------------------------------------------
int      MRows, NCols;                     //indices for rows and cols
FILE     *infptr, *outfptr;                //input/output file pointers
char     infile[40];
char     static  outfile[40];              //names of input/output image files

//------------------------------------------------------------------------------
//-----------------------------MAIN---------------------------------------------
//------------------------------------------------------------------------------
main()
{
    //--------------( Display Heading with information )------------------------
    heading();
    //---------------(Open Input & Output Image Files)--------------------------
    openfiles();
    //---------------------(Obtain negative of image)---------------------------
    negative();
    //------------------(Close Any Open Image Files)----------------------------
    closefiles();
    //--------------------------------------------------------------------------
    printf("\n Bye! Bye!\n");
    system("PAUSE");
    return (1);
} //end main()

//------------------------------------------------------------------------------
//----------------------------HEADING-------------------------------------------
//------------------------------------------------------------------------------
void heading()
{ int      i;
  for (i=0;i<16;i++) printf("                                     +\n");
  printf("                       Digital Image Processing Program\n");
  printf("                         Updated: August 24, 2009\n");
  printf("                                 RickWare\n");
  for (i=0;i<4;i++)  printf("                                     +\n");
}//end heading()

//------------------------------------------------------------------------------
//--------------------------OPENFILES-------------------------------------------
//------------------------------------------------------------------------------
void openfiles(void)
{
  printf("\n                 OPEN an image file\n");
  printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  printf(" Enter name of *.pgm INPUT image file (example: lena.pgm) ? ");
  scanf("%s",&infile);
  printf(" Enter name of *.pgm OUTPUT image file (example: lenaconv.pgm ? ");
  scanf("%s",&outfile);
    
  if ((infptr = fopen(infile, "rb")) == NULL)
  { printf(" Can NOT open input image file: <%s>\n",infile);
    printf(" Exiting program..... "); system("PAUSE"); exit(1);
  }
  else  printf(" Input file <%s> opened sucessfully\n\n",infile);
  
  if ((outfptr = fopen(outfile,"wb")) == NULL)
  { printf(" Can NOT open output image file <%s>\n\n",outfile);
    printf(" Exiting program....."); system("PAUSE"); exit(1);
  }
  else printf(" Output file <%s> is opened sucessfully\n\n",outfile);
  
  readhdr();
  return;
}//end openfiles()

//------------------------------------------------------------------------------
//------------------------------READHDR-----------------------------------------
//------------------------------------------------------------------------------
void readhdr()
{
  int     i, k=0, MaxRGB;
  char    c, c1, buffer[128];
  //-----------------------[Read PPM File Header]-------------------------------
  printf("\n\n File <%s> Header Bytes:\n",infile);
  printf("------------------------------------------------------------\n");
    i = 0;
    do
    { c = fgetc(infptr);
      buffer[i] = c; i++;
    } while (c != '\n');
    
     c1 = buffer[1];
     
     if (c1 == '5')
     {
       printf("\nFile type is:  <P%c>\n",c1);
     }
     else
     { printf(" Image is in WRONG format!! Quitting.........\n\n");
       system("PAUSE");
       exit(0);
     }
    
  fscanf(infptr,"%d %d %d",&NCols, &MRows, &MaxRGB);
  c = fgetc(infptr);
  printf("%d ",NCols);
  printf("%d     <-----(Width & Height)\n", MRows);
  printf("%d         <-----(Max. RGB Level)\n\n",MaxRGB);
}//end readhdr()

//------------------------------------------------------------------------------
//--------------------------Obtain convolved image------------------------------
//---------------by taking the one's complement of every pixel------------------
//------------------------------------------------------------------------------
void negative()
{
  unsigned char Gray;
  unsigned char mat [512][512];
  unsigned char Gx	[512][512];
  unsigned char Gy	[512][512];
  unsigned char conv[512][512];
  // Blur mask
  //         char mask[  3][  3] ={{ 1, 1, 1},
  //                               { 1, 1, 1},
  //								 { 1, 1, 1}};

  // Sobel horizontal mask (edge detection)
           int maskH[  3][  3] = {{ 1, 2, 1},
                                  { 0, 0, 0},
  								  {-1,-2,-1}};
								 
  // Sobel vertical mask (edge detection)
           int maskV[  3][  3] = {{-1, 0, 1},
                                  {-2, 0, 2},
  								  {-1, 0, 1}};
								 
  // Sharpening mask (second derivative)
  //          int mask[  3][  3] = {{ 0, 1, 0},
  //                                 { 1,-4, 1},
  //								 { 0, 1, 0}};
								 
  // Sharpening mask (second derivative)
  //         int mask[  3][  3] = {{ 1, 1, 1},
  //                               { 1,-8, 1},
  //		     					 { 1, 1, 1}};
								 
  int col, row, value;
  int dividefactor = 0;

  //Add *.pgm Header to output file
  fprintf(outfptr,"P5\n512 512\n255\n");
  
  //Read input image and store in matrix
  for (col = 0; col < MRows; col++)
    for (row = 0; row < NCols; row++) {
        Gray = fgetc(infptr);
        Gx[col][row] = Gray;
    }
    
  // Clear convolution matrix
  for (col = 0; col < MRows; col++)
      for (row = 0; row < NCols; row++)
      	conv[col][row] = 0;
    
  // Calcula dividefactor for Sobel Horizontal Mask:
  for (col = 0; col < 3; col++)
	for (row = 0; row < 3; row++) {
      	dividefactor += maskH[col][row];
      }

  printf ("dividefactor (antes) = %d \n",dividefactor);
      
  if (dividefactor == 0)
		dividefactor = 1;
		
  printf ("dividefactor (despu�s) = %d \n",dividefactor);
  
  // Clear Gx matrix
  for (col = 0; col < MRows; col++)
      for (row = 0; row < NCols; row++)
      	Gx[col][row] = 0;
  
  //Convolve image with horizontal mask
    for (col = 1; col < MRows-1; col++)
      for (row = 1; row < NCols-1; row++) {
      	value          = ((maskH[0][0] * mat[col-1][row-1]  +
      	                   maskH[0][1] * mat[col  ][row-1]  +
      	                   maskH[0][2] * mat[col+1][row-1]  +
      	                   maskH[1][0] * mat[col-1][row  ]  +
      	                   maskH[1][1] * mat[col  ][row  ]  +
      	                   maskH[1][2] * mat[col+1][row  ]  +
      	                   maskH[2][0] * mat[col-1][row+1]  +
      	                   maskH[2][1] * mat[col  ][row+1]  +
      	                   maskH[2][2] * mat[col+1][row+1]) / dividefactor);
    	Gx[col][row] = value;
    	if      (value <   0) Gx[col][row] = 0;
    	else if (value > 255) Gx[col][row] = 255;
    	else                  Gx[col][row] = value;
      }
  

  // Calcula dividefactor for Sobel Horizontal Mask:
  for (col = 0; col < 3; col++)
	for (row = 0; row < 3; row++) {
      	dividefactor += maskH[col][row];
      }

  printf ("dividefactor (antes) = %d \n",dividefactor);
      
  if (dividefactor == 0)
		dividefactor = 1;
		
  printf ("dividefactor (despu�s) = %d \n",dividefactor);
  
  // Clear Gy matrix
  for (col = 0; col < MRows; col++)
      for (row = 0; row < NCols; row++)
      	Gy[col][row] = 0;
  
  //Convolve image with vertical mask
    for (col = 1; col < MRows-1; col++)
      for (row = 1; row < NCols-1; row++) {
      	value          = ((maskV[0][0] * mat[col-1][row-1]  +
      	                   maskV[0][1] * mat[col  ][row-1]  +
      	                   maskV[0][2] * mat[col+1][row-1]  +
      	                   maskV[1][0] * mat[col-1][row  ]  +
      	                   maskV[1][1] * mat[col  ][row  ]  +
      	                   maskV[1][2] * mat[col+1][row  ]  +
      	                   maskV[2][0] * mat[col-1][row+1]  +
      	                   maskV[2][1] * mat[col  ][row+1]  +
      	                   maskV[2][2] * mat[col+1][row+1]) / dividefactor);
    	Gy[col][row] = value;
    	if      (value <   0) Gy[col][row] = 0;
    	else if (value > 255) Gy[col][row] = 255;
    	else                  Gy[col][row] = value;
      }
  

  // printf("Fisrt convolved pixel = %d \n",conv[1][1]);
  
  // Obtain Magnitude of Gx and Gy to obtain contour
  for (col = 1; col < MRows-1; col++){
    for (row = 1; row < NCols-1; row++) {
    	value = pow(Gx[col][row],2) + pow(Gy[col][row],2);
    	conv[col][row] = sqrt(value);
	}
  }
  
  
  //Save convolved image 
  for (col = 0; col < MRows; col++){
    for (row = 0; row < NCols; row++) {
        Gray = conv[col][row];
        fputc(Gray,outfptr);
    }  
  }
  
}

//------------------------------------------------------------------------------
//----------------------------CLOSEFILES----------------------------------------
//------------------------------------------------------------------------------
void closefiles()
{ //------------------------(Close Files)---------------------------------------
  fclose(infptr);
  fclose(outfptr);

  return;
} //end closefiles()
//------------------------------------------------------------------------------
