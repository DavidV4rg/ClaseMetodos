#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

#define M_PI 3.14159265359


int main()
{
    
    float dx = 0.005;
    float c = 300;
    float l = 1.0;
    float A0 = 0.1;
    int N_puntos =(l/dx)+1;
    float dt = (dx*0.9)/c;
    float xpast[N_puntos], xpres[N_puntos], xfutu[N_puntos], x[N_puntos];
    float x_pas[N_puntos], x_pre[N_puntos], x_fut[N_puntos];
    float x_pa3[N_puntos], x_pr3[N_puntos], x_fu3[N_puntos], x_3[N_puntos];
    float t[N_puntos], tiem[N_puntos]; 
    
    //Condiciones iniciales extremos fijos
    t[0] = 0;
    t[N_puntos] = l;
    x[0] = 0;
    xpast[0] = 0;
    xpast[N_puntos] = 0;
    xpres[0] = 0;
    xpres[N_puntos] = 0;
    xfutu[0] = 0;
    xfutu[N_puntos] = 0;
    tiem[0] = 0;
    
    //Condiciones iniciales con 1 extremo libre
    x_pre[0] = 0;
    x_pas[0] = 0;
    x_fut[0] = 0;
    
    //condiciones iniciales caso 3
    x_pa3[0]=0;
    x_3[0]=0;
    x_fu3[0]=0;
    
    double r = pow((c*dt)/dx,2);    
    ofstream outfile;
    outfile.open("inicial.dat");
    
    ofstream outfile2;
    outfile2.open("libre.dat");
    
    
    //Condiciones iniciales
    for (int i = 1; i<N_puntos+1; i++)
    {
        t[i] = t[i-1] + dx;
        
        x_3[i] = 0;
        x_pa3[i] = x_3[i];
        
        if(t[i]<= l/2)
        {
            x[i] = (2*A0/l)*t[i];
            xpast[i] = x[i];
            x_pas[i] = x[i];
        }        
        else
        {
            x[i] = -(2*A0/l)*t[i] + 2*A0;
            xpast[i] = x[i];
        }        
    }

    //Primer paso        
    for (int i = 1; i <=N_puntos; i++)
    {
        xpres[i] = (r/2)*(xpast[i+1]+xpast[i-1]-2*xpast[i]) + xpast[i]; 
        x_pre[i] = xpres[i];
        x_pre[N_puntos-1] = x_pre[N_puntos-2];
     
        x_pr3[i] = (r/2)*(x_pa3[i+1]+x_pa3[i-1]-2*x_pa3[i])+ x_pa3[i];
          if(i==N_puntos-1)
        {
            x_pr3[i] = A0*sin(3*c*tiem[0]*M_PI/l);
        }
            
        outfile << t[i-1] << ";" << xpast[i-1]<<";"<< xpres[i-1] << endl;
        outfile2 << t[i-1] << ";" <<x_pas[i-1] << ";" << x_pre[i-1] << endl;
    }
    outfile.close();
    outfile2.close();
    
    //Siguientes pasos
    int contador = 0;
    ofstream outfile1;
    outfile1.open("data.dat");
    ofstream outfile3;
    outfile3.open("data1.dat");
    for (int i = 0; i<=N_puntos+250; i++)
    {
        contador = contador+1;
        for (int k = 1; k <= N_puntos; k++)
        {
            xfutu[k]= r*(xpres[k+1]+xpres[k-1]-2*xpres[k])-xpast[k]+2*xpres[k];
            x_fut[k]= r*(x_pre[k+1]+x_pre[k-1]-2*x_pre[k])-x_pas[k]+2*x_pre[k];
            x_fut[k] = x_fut[k-1];
            
            if(contador%15==0)
            {
                outfile1 << xfutu[k-1] <<";"<< t[k-1] <<endl;
                outfile3 << x_fut[k-1] <<";"<< t[k-1] <<endl;
            }
        }            
            for(int z =1; z<N_puntos; z++)
            {
                xpast[z] = xpres[z];
                xpres[z] = xfutu[z]; 
                x_pas[z] = x_pre[z];
                x_pre[z] = x_fut[z];                
            }        
    }
    outfile1.close();
    int contador1 = 0;
    
    for (int i = 0; i <= N_puntos+500; i++)
    {
        contador1 = contador1 +1;
        for (int k = 1; k <= N_puntos; k++)
        {            
            x_fut[k]= r*(x_pre[k+1]+x_pre[k-1]-2*x_pre[k])-x_pas[k]+2*x_pre[k];
            x_fut[k] = x_fut[k-1];
            
            if(contador%25==0)
            {
                outfile3 << x_fut[k-1] <<";"<< t[k-1] <<endl;
            }
        }            
            for(int z =1; z<N_puntos; z++)
            { 
                x_pas[z] = x_pre[z];
                x_pre[z] = x_fut[z];                
            }        
    }
 
    outfile3.close();
    
    int contador2 = 0;
    ofstream outfile4;
    outfile4.open("caso3.dat");
    for (int i = 0; i <= 1000; i++)
    {
        contador2 = contador2 + 1;
        for (int k = 1; k <= N_puntos; k++)
        {
            x_fu3[k]= r*(x_pr3[k+1]+x_pr3[k-1]-2*x_pr3[k])-x_pa3[k]+2*x_pr3[k];
            x_fu3[k-1] = x_fu3[k-2];
            
            if(k==N_puntos-1 && contador%25 == 0)
            {
                x_fu3[i] = A0*sin(3*c*tiem[0]*M_PI/l);
                outfile4 << x_fu3[k-1] << ";" << t[k-1] << endl;
            }   
            for (int z = 1; z<N_puntos; z++)
            {
                x_pa3[z] = x_pr3[z];
                x_pr3[z] = x_fu3[z];
            }
        }
    }
    outfile4.close();
    return 0;
}
