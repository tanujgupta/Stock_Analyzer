// Project : Monte Carlo Method of Simulation in Finance
// Calculates Most Probable Value of the stock after some time
// calculates Value at risk for a particular investment

//header files

#include<iostream.h>
#include <fstream>
#include<time.h>
#include<stdio.h>
#include<conio.h>
#include<math.h>

//global variables initilisation
using namespace std;
double delta = 0.4 ;
double f = 0 ;
double E = 2.71828182845 ;
int choice ;
double r ;
int done = 0 ;
int counter = 1,t=1 ;
unsigned long seed = 0 ;
float stockval,tim,drift,vol,uavg,dt=1;
float hdata[100],hday[100],u[100];
int hn;
float nextval[100000];
int histogram[1000];
float min1;
float max1;
float sum,avgval,var,stock,varp;

//function to  calculate volatility of stock
//based on the historical data available

float volat()
{
     int i;
     for(i=1;i<hn;i++)
     {
                      u[i]=log(hdata[i]/hdata[i-1]);
                      sum+=u[i];
     };
     uavg=sum/(hn-1);
     for(i=1;i<hn;i++)
     {
                      vol+=pow((u[i]-uavg),2);

     }

     vol=vol/(hn-1);
     drift=sum+vol/2;
     vol=sqrt(vol);
     return vol;

}

// function taking inputs from the user

void input()
{
     float vol1;
     cout<<"\n enter the value of the stock:";
     cin>>stockval;

     cout<<"\n enter the time(days) after which you want to see the value at risk";
     cin>>tim;
     cout<<"\n Taking the  historical data from input file to calculate volatility.....";
     ifstream inStream;
  	 inStream.open("input.txt");

     while (!inStream.eof())
     {
           hday[hn]=t;
           //inStream >> hday[hn];
           inStream >> hdata[hn];
           t++;
           hn++;

     };
     inStream.close();
     vol1=volat();
     cout<<"\n Volatility is "<<vol1;//"\t drift="<<drift;


};

//function to generate Random Numbers

double rng (int seed )                      //function to generate random numbers
{
   unsigned long a  = 16807 ;
   unsigned long m = pow ( 2 , 31 ) - 1 ;
   double xk = 0 ;
   seed = (seed * a) % m ;
   xk = double(seed)/m ;
   return xk ;
}

// function that calculate the epected values of stock after a given time
// program uses Black Scholes approch to preict the values of stocks based on volatiliy and rate

void simulator(int ch,int seed)
{
     float x1;
     for(int k=0;k<100000;k++)
     {
             nextval[k]=stockval;
     };
     double x0;
     cout<<endl;
     //cout<<"u="<<uavg<<endl;
     for(int j=0;j<100000;j++)
     {
           for(int i=0;i<tim;i++)
           {
                  if ( ch == 1  ) x0 = (double(rand ( )) / RAND_MAX);
                  else
                  {
                        x0 = rng (seed);
                        seed=seed+2*(int(10*x0));
                  }
                  x1=(double(rand ( )) / RAND_MAX);
                  if(x1>0.5)
                  {
                  nextval[j]=nextval[j]+nextval[j]*((uavg*dt)+((x0)*vol*(sqrt(dt))));
                  }
                  else
                  {
                  nextval[j]=nextval[j]-nextval[j]*((uavg*dt)+((x0)*vol*(sqrt(dt))));
                  }
           };
        //   cout<<nextval[j]<<"     ";

     };

     min1=nextval[0];
     max1=nextval[0];
     for(int i=0;i<100000;i++)
     {
                         if(nextval[i]<min1)
                         {
                                          min1=nextval[i];
                         }
                         if(nextval[i]>max1)
                         {
                                           max1=nextval[i];
                         };
     };
     cout<<"minimum expected value = "<<min1<<"\nmaximum expected value = "<<max1;
     for(int i=0;i<1000;i++)
     {
             histogram[i]=0;
     };
     float interval=min1;
     for(int i=0;i<1000;i++)
     {
             for(int j=0;j<100000;j++)
             {
                     if((nextval[j]>=interval) && nextval[j]<(interval+(max1-min1)/1000))
                     {
                                          histogram[i]+=1;
                     };
             }
             interval+=(max1-min1)/1000;
     };
     ofstream f("output.txt");
     for(int i=0;i<1000;i++)
     {
             f<<histogram[i]<<" ,\t\t"<<i*(max1-min1)/1000+min1;
             f<<endl;
     };
     f.close();

 //    cout<<"maxval="<<max<<"        "<<"minval="<<min;


};



// main function

int main ( )
{
   int i,flag=1;
   float loss,totloss,avg;
   float gain;
   input();
   srand ( time(NULL ) ) ;

   cout << "\n Enter your choice for Random Number generator " ;
   cout<<"\n 1. from compiler ";
   cout<<"\n 2. from program \n";
   cin >> choice ;
   if(choice==2)
   {
                cout<<"\n Enter an odd number for  seed:";
                cin>>seed;
   };

   // cout<<"beg";
   simulator(choice,seed);

   while(flag)
   {
   above:
   cout<<"\n Enter choice to see the desired values";
   cout<<"\n 1. Value at Risk";
   cout<<"\n 2. Expected Return\n";
   cin>>choice;
   if(choice>2 || choice<1)
   {
               cout<<"\n Wrong Choice ";
               flag=1;
   }
   else
   {
       flag=0;
   };
   };
   switch(choice)
   {
                 case 1:{
                        cout<<"\n  Enter the percentage of confidence level :\n";
                        cin>>loss;
                        i=0;
                        totloss=0;
                        /*while((min1+i*(max1-min1)/1000)<stockval*(1-loss/100)+(max1-min1)/1000)
                        {
                                                                   totloss+=histogram[i];
                                                                   i++;
                        };*/
                        while(totloss<(1000*(100-loss)))
                        {
                            totloss+=histogram[i];
                            i++;
                        };
                        var = min1 + ((max1-min1)*(i-0.5)/1000);
                        //totloss=totloss/100000;
                        cout<<"\n";
                        varp=(stockval-var)/stockval*100;
                        cout<<"Down side risk with "<<loss<< " % confidence is "<<var<<" which is ";
                        cout<<varp<<" % below stock value";
                        }
                        cout<<"\n Do you want to continue (1 for YES and 2 for NO) ";
                        cin>>flag;
                        if(flag==1)
                        {
                         goto above;
                        }
                        else if (flag==2)
                        {
                               exit(0);
                        };
                 case 2:{
                         int j;
                         gain=0;
                         for(i=1;i<100;i++)
                         {
                          j=0;totloss=0;
                          
                          while(totloss<(1000*(100-i)))
                         {
                            totloss+=histogram[j];
                            j++;
                         };
                         var = min1 + ((max1-min1)*(j-0.5)/1000);
                         varp=(stockval-var)/stockval*100;
                         avg=((min1+((i+0.5)*(max1-min1)/1000))-stockval)/stockval;   
                         gain+=i*varp/10000;
                         }        
                         /*gain=0;
                         for(i=0;i<1000;i++)
                         {
                                            avg=((min1+((i+0.5)*(max1-min1)/1000))-stockval)/stockval;
                                            gain+=(avg)*histogram[i]/100000;

                        };*/
                        //gain=gain*100;
                        cout<<"\n Expected Return = "<<gain<<" %";
                       }
                        cout<<"\n Do you want to continue (1 for YES and 2 for NO) \n";
                        cin>>flag;
                        if(flag==1)
                        {
                         goto above;
                        };


   };


   getche();
   return 0 ;
}

