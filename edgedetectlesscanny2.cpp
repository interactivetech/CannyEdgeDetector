/* A program that alters images*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define  BUFFER_SIZE  70
#define  TRUE          1
#define  FALSE         0

//Declare and initialize variables outside main function
int**  img;
double** temppicx;
double** temppicy;
int** candid;
int** finaly;
double**  sobelout;
int** sobelout2;
int    numRows;
int    numCols;
int    maxVal;
FILE*  fo1;

//Declare functions
void addtopixels(int** imgtemp, int value);
void  writeoutpic(char* fileName, int** imgtemp);
int** readpic(char* fileName);
void  readHeader(FILE* imgFin);
int   isComment(char* line);
void  readImgID(char* line);
void  readImgSize(char* line);
void  readMaxVal(char* line);
int** setImage();
double** setdImage();
void  readBinaryData(FILE* imgFin, int** imgtemp);
void  subtractpixels(int** out, int** first, int** second);
void  cannyfunc(int** pic, double** edges, double** tempx, double** tempy, int** candid, int** final);
int F1(int** PIC1, int numRows, int numCols, int Value1);
void F2(int** PIC1, int numRows, int numCols, int Value1);
void F3(int** PIC1, int numRows, int numCols, int Value1);
void F4(int** PIC1, int numRows, int numCols, int Value1);
void F5(int** PIC1, int numRows, int numCols, int Value1);
void copy(int **pic1, int numRows, int numCols, int Value1);

int main()
//Declare and initialize Variables inside main function
{
        char fileName[BUFFER_SIZE];
        int i,j,rows,cols,user_input=0,Value1=0,loop=1;
        char ci;

//Print out menu
while (loop = 1){
       // printf("Please select an option:\n");
       // printf("1) Read in a picture file and process it.\n");
       // printf("2) Read in two picture files and subtract the second picture from the first.\n");
        printf("Read in a pic and apply the sobelfunc function to it.                           Use a grayscale pic saved as a *.pgm in your computer                           Please enter the number 3 on your keyboard and then hit the enter key                        \n");
      //  printf("4) Exit.\n");

        scanf("%d", &user_input);

//Read in an image and prompt user to enter a value for the image
 
        if (user_input == 1){
        printf("enter image filename in the format *.pgm: ");
        scanf("%s", fileName);

        img = readpic(fileName);

        printf("Successfully read image file '%s'\n", fileName);
        
        printf("Enter a value for the picture: ");
        scanf("%d", &Value1);
        system("pause");
        F3(img, numRows, numCols, Value1);
        F4(img, numRows, numCols, Value1);

        printf("Enter image filename for output: ");
        scanf("%s", fileName);

        writeoutpic(fileName,img);

        free(img);
        img = NULL;
                                  }
//Read in two images and subtract the second from the first
        else if (user_input == 2) {
        printf("Enter first image filename: ");
          scanf("%s", fileName);
       //   temppicx = readpic(fileName);
          
          printf("Enter second image filename: ");
          scanf("%s", fileName);
         // temppicy = readpic(fileName);
          
        //  sobelout = setImage();      
       //   subtractpixels(sobelout, temppicx, temppicy);
          printf("Enter image filename for output: ");
          scanf("%s", fileName);

         // writeoutpic(fileName,sobelout);    
          free(temppicx);
          temppicx = NULL;   
          free(temppicy);
          temppicy = NULL;
        
                                   }
//Read in a picture and apply the sobelfunc function to it
        else if (user_input == 3){

        printf("Enter pic filename (*.pgm, example dog.pgm): ");
                scanf("%s", fileName);

                img = readpic(fileName);  

                sobelout= setdImage();
                          
                
                candid = setImage();  

                sobelout= setdImage();
                temppicx= setdImage();
                temppicy= setdImage();
                
                finaly= setImage();
                sobelout2=setImage();
                 
               cannyfunc(img,sobelout,temppicx,temppicy,candid,finaly);

                printf("Enter mag filename for output: ");
                scanf("%s", fileName);
for(i=0;i<numRows;i++) for (j=0;j<numCols;j++) sobelout2[i][j]= (int) sobelout[i][j];
              writeoutpic(fileName,sobelout2); 
                
                    
                printf("Enter peaks filename for output: ");
                scanf("%s", fileName);

               writeoutpic(fileName,candid); 
                printf("Enter final filename for output: ");
                scanf("%s", fileName);

             writeoutpic(fileName,finaly);
                    
                    
                free(img);
                img = NULL;
        
                                  }
  
        
//Exit the program.
        else if (user_input == 4) {
        return(EXIT_SUCCESS);     
                                 }
                                  }
        
}
//Use these functions:

void addtopixels(int** imgtemp, int value)
{  
        int i,j;
        
        for (i=0;i<numRows;i++)
        { for (j=0;j<numCols;j++)
                {
                  imgtemp[i][j] += value;
                }
        }
}

void writeoutpic(char* fileName, int** imgtemp)
{
        int i,j;
        char ci;
        FILE* fo1;
        
        if((fo1 = fopen(fileName, "wb")) == NULL)
        {
                printf("Unable to open out image file '%s'\n", fileName);
                system("PAUSE");
                exit(EXIT_FAILURE);
        }

        fprintf(fo1,"P5\n");
        fprintf(fo1,"%d %d\n", numRows, numCols);
        fprintf(fo1,"255\n");

        for (i=0;i<numRows;i++)
        { for (j=0;j<numCols;j++)
                {
                  ci   =  (char) (imgtemp[i][j]);
                  fprintf(fo1,"%c", ci);
                }
        }
}




int** readpic(char* fileName)
{
        FILE* imgFin;
        int** imgtemp;

        if((imgFin = fopen(fileName, "rb")) == NULL)
        {
                printf("Unable to open image file '%s'\n", fileName);
                system("PAUSE");
                exit(EXIT_FAILURE);
        }

        readHeader(imgFin);


        imgtemp  = setImage();
        readBinaryData(imgFin, imgtemp);

        fclose(imgFin);
        
        return  imgtemp;

}

void readHeader(FILE* imgFin)
{
        int  haveReadImgID   = FALSE;
        int  haveReadImgSize = FALSE;
        int  haveReadMaxVal  = FALSE;
        char line[BUFFER_SIZE];

        while(!(haveReadImgID && haveReadImgSize && haveReadMaxVal))
        {
                fgets(line, BUFFER_SIZE, imgFin);

                if((strlen(line) == 0) || (strlen(line) == 1))
                        continue;

                if(isComment(line))
                        continue;

                if(!(haveReadImgID))
                {
                        readImgID(line);
                        haveReadImgID = TRUE;
                }
                else if(!(haveReadImgSize))
                {
                        readImgSize(line);
                        haveReadImgSize = TRUE;
                }
                else if(!(haveReadMaxVal))
                {
                        readMaxVal(line);
                        haveReadMaxVal = TRUE;
                }
        }

}

int isComment(char *line)
{
        if(line[0] == '#')
                return(TRUE);

        return(FALSE);
}

void readImgID(char* line)
{
        if(strcmp(line, "P5\n") != 0)
        {
                printf("Invalid image ID\n");
                system("PAUSE");
                exit(EXIT_FAILURE);
        }
}

void readImgSize(char* line)
{
        unsigned i;

        for(i = 0; i < strlen(line); ++i)
        {
                if(!((isdigit((int) line[i])) || (isspace((int) line[i]))))
                {
                        printf("Invalid image size\n");
                        system("PAUSE");
                        exit(EXIT_FAILURE);
                }
        }

        sscanf(line, "%d %d", &numRows, &numCols);
}

void readMaxVal(char* line)
{
        unsigned i;

        for(i = 0; i < strlen(line); ++i)
        {
                if(!((isdigit((int) line[i])) || (isspace((int) line[i]))))
                {
                        printf("Invalid image max value\n");
                        system("PAUSE");
                        exit(EXIT_FAILURE);
                }
        }

        maxVal = atoi(line);
}

int** setImage()
{
        int** imgtemp;
        unsigned i;

        imgtemp = (int**) calloc(numRows, sizeof(int*));

        for(i = 0; i < numRows; ++i)
        {
                imgtemp[i] = (int*) calloc(numCols, sizeof(int));
        }
        return imgtemp;
}
double** setdImage()
{
        double** imgtemp;
        unsigned i;

        imgtemp = (double**) calloc(numRows, sizeof(double*));

        for(i = 0; i < numRows; ++i)
        {
                imgtemp[i] = (double*) calloc(numCols, sizeof(double));
        }
        return imgtemp;
}
void readBinaryData(FILE* imgFin, int** imgtemp)
{
        unsigned  i;
        unsigned  j;
        for(i = 0; i < numRows; ++i)
        {
                for(j = 0; j < numCols; ++j)
                {
                            imgtemp[i][j] = 
                            fgetc(imgFin);
                }
        }
}

void subtractpixels(int** out, int** first, int** second){
        
        int i,j;
        
        for (i=0;i<numRows;i++)
        { for (j=0;j<numCols;j++)
                {
                  out[i][j] = abs(first[i][j] - second[i][j]);
                }
        }     
}

void cannyfunc(int** pic, double** edges, double** tempx, double** tempy,int** cand,int** final){


         
        double maxival;
double maskval,maskx[100][100],masky[100][100];
int centx,centy;
        

        int i,j,p,q,mr;
        double sum1,sum2;
        double threshold;
        double sig,slope;
        sig=1;
        

        mr = 3*(int)sig;

centx=50;centy=50;

        //Completing a Gaussian blur on image

        for (p=-mr;p<=mr;p++)
        {  for (q=-mr;q<=mr;q++)
           {
              maskval = -q*
                      (exp(-1*(((p*p)+(q*q))/(2*(sig*sig)))));
              (maskx[p+centy][q+centx]) = maskval;
  //           printf("%4.2f ", maskx[p+centy][q+centx]);
             
              maskval = -p*
                      (exp(-1*(((p*p)+(q*q))/(2*(sig*sig)))));
              (masky[p+centy][q+centx]) = maskval;
              
           }
       //    printf("\n");
        }
//system("pause");
         for (i=mr;i<numRows-mr;i++)
        { for (j=mr;j<numCols-mr;j++)
          {
             sum1 = 0;
             sum2 = 0;
             for (p=-mr;p<=mr;p++)
             {
                for (q=-mr;q<=mr;q++)
                {
                   sum1 += pic[i+p][j+q] * maskx[p+centy][q+centx];
                   sum2 += pic[i+p][j+q] * masky[p+centy][q+centx];
                }
             }
             tempx[i][j] = sum1;
             tempy[i][j] = sum2;
             
         //  printf(" here 8888   sum1, 2=  %6.3lf %6.3lf \n",  sum1,sum2);  
//printf(" here 777    tempx,y=  %6.3lf %6.3lf \n",  tempx[i][j],tempy[i][j]);
          }
      //    system("pause");
        }
        
     
        
        
        
        
        
        //Finding the Intensity Gradient of the Image

        maxival = 0;
        for (i=mr;i<numRows-mr;i++)
        { for (j=mr;j<numCols-mr;j++)
          {
              
             // printf(" here 6    tempx,y=  %6.3lf %6.3lf \n",  tempx[i][j],tempy[i][j]);   
             edges[i][j]=sqrt((double)((tempx[i][j]*tempx[i][j]) +
                                      (tempy[i][j]*tempy[i][j])));
             if (edges[i][j] > maxival)
                maxival = edges[i][j];

           }
        }



        for (i=0;i<numRows;i++)
          { for (j=0;j<numCols;j++)
            {
             edges[i][j] = ((edges[i][j]*1.0) / maxival) * 250;            
             
            }
          }
       //printf(" here 1 \n");   
          
//non-maximal suppression
for(i=mr;i<numRows-mr;i++){
   for(j=mr;j<numCols-mr;j++){
    
//printf(" here 4    tempx,y=  %6.3lf %6.3lf \n",  tempx[i][j],tempy[i][j]);   
      if((tempx[i][j]) == 0.0) {
         tempx[i][j] = 0.00001;
       }
    //   printf(" here 3    tempx,y=  %6.3lf %6.3lf \n",  tempx[i][j],tempy[i][j]);   
          
      slope = tempy[i][j]/tempx[i][j];
    //  printf(" here 2 ij=%d %d  \n", i, j);   
          
      if( (slope <= .4142)&&(slope > -.4142)){
         if((edges[i][j] > edges[i][j-1])&&(edges[i][j] > edges[i][j+1])){
            cand[i][j] = 220;
          }
       }
      else if( (slope <= 2.4142)&&(slope > .4142)){
         if((edges[i][j] > edges[i-1][j-1])&&(edges[i][j] > edges[i+1][j+1])){
             cand[i][j] = 220;
           }
       }
      else if( (slope <= -.4142)&&(slope > -2.4142)){
         if((edges[i][j] > edges[i+1][j-1])&&(edges[i][j] > edges[i-1][j+1])){
             cand[i][j] = 220;
           }
       }else{
         if((edges[i][j] > edges[i-1][j])&&(edges[i][j] > edges
         [i+1][j])){
             cand[i][j] = 220;
          }
        }
     }
  }
          
 
 int high,low,moretogo;
  
 high=70;
 low=30;
  //edge tracking by hysteresis
for ( i = 0; i < numRows; i++ )
        {
                for ( j = 0; j < numCols; j++ )
                {
                        if ( cand[i][j] == 220 )
                        {
                                if ( edges[i][j] > high )
                                {
                                        cand[i][j] = 0;
                                        final[i][j] = 220;
                                }
                                else if ( edges[i][j] < low )
                                {
                                        cand[i][j] = 0;
                                }
                        }
                }
        }
moretogo = 1;
while ( moretogo )
     {
     moretogo = 0;
        for ( i = 0; i < numRows; i++ )
        {
                for ( j = 0; j < numCols; j++ )
                {
                          if ( final[i][j] == 220 )
                          {
                                for ( p = -1; p <= 1; p++ )
                                {
                                        for ( q = -1; q <= 1; q++ )
                                        {
                                                if ( cand[i+p][j+q] == 220 )
                                                {
                                                        cand[i+p][j+q] = 0;
                                                        final[i+p][j+q] = 220;
                                                        moretogo = 1;
                                                }
                                        }
                                }
                        }
                }
        }
}
              

          
          
          
          
}
int F1(int** PIC1, int numRows, int numCols, int Value1){
    int i, j, num_value1=0;
    for (i=0;i<numRows;i++){
         for (j=0;j<numCols;j++)
                {if(PIC1[i][j]==Value1)
                  num_value1++;
                  
                }
        }return num_value1;
        }
void F2 (int** PIC1, int numRows, int numCols, int Value1){
     int i, j;
     for (i=0;i<numRows;i++){
         for (j=0;j<numCols;j++)
                {
                if(PIC1[i][j]>Value1)
                PIC1[i][j]=220;
                else  
                PIC1[i][j] = 0;
                }
        }
        }
void F3 (int** PIC1, int numRows, int numCols, int Value1){
     int i, j, centrow, centcol, p, q;
     for(centcol=numCols/8;centcol<numCols;centcol+=numCols/8){
     for(centrow=numRows/8;centrow<numRows;centrow+=numRows/8){
         for (i=0;i<numRows;i++){
         for (j=0;j<numCols;j++){
                p=i-centrow;
                q=j-centcol;
                if((p*p)+(q*q)<=((numCols/16)*(numCols/16))){
                if(PIC1[i][j]+30<220){
                PIC1[i][j]+=50;}}
                }
        }
        }
        }
        }
void F4 (int** PIC1, int numRows, int numCols, int Value1){
     int i, j, colrate, numrate;
     for(colrate=(numCols/8);colrate<numCols;colrate+=(numCols/8)){
         for (j=0;j<numRows;j++)
                {
                PIC1[(colrate)][j]=Value1;
                }
         for (i=0;i<numCols;i++)
                {
                PIC1[i][colrate]=Value1;
                }
                }
        }
        
void F5 (int** PIC1, int numRows, int numCols, int Value1){
     int i, j;
     
         for (i=0,j=0;i<numRows;i+=3,j+=1)
                {
                PIC1[i][j]=Value1;
                }
         
        
        }
void copy(int **pic1, int numRows, int numCols, int Value1){
     int i,j,centrow=0, centcol=0,p,w,radius=5;
     
     
     for(i=0;i<numRows;i++){ 
     centrow+=(numRows/8);
     for(j=0;j<numCols;j++)
    {
    
    centcol+=(numCols/8);                      
                             
    
    p=i-centcol;
    w=j-centrow;
    if(p*p+w*w<=radius*radius){
          pic1[i][j]+=Value1;}
         
          
          
     }
    
     }
     
     }
     
