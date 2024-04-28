//maths library
#ifndef ESP_MATHS_H
#define ESP_MATHS_H
#include <math.h>
#define _PI 3.1415926535897932384626433832795

typedef unsigned char BYTE;
#ifndef byte
	typedef unsigned char byte;
#endif
//Linux POINT class:
class POINT {
public:
        double x, y;
};


/*calcul de factoriel*/
long long fact(long long n)
{//cas particuliers: 0! et 1!
if((n==0)||(n==1)){return 1;}
return n*fact(n-1);
}

//retourne la partie entiere d'un nombre
int intPart(double x)
{
	if(x>=0)return (int)x;
	else
	{
		return (int)x-1;
	}
}


//[ßoon]retourne la partie decimale d'un nombre <u>relatif</u>
double floatPart(double x)//utilise partEnt
{
			if(x>=0)
	{ return (x-intPart(x)); }
	else
	{ return -( -x+(intPart(x)+1) ); }
}

/*[ßoon] Calcule la racine ennième d'un nombre*/
double racine(double x,double n)
{
	return pow( x,(double)1/n );
}

/*[ßoon] Renvoie le Carré d'un nombre*/
double sqr(double a){return a*a;}
/*round function*/
int arround(double x)
{int i=(int)x;double d=x-i;
if(d>=0.5)return i+1;else return i;
}
//renvoie le plus petit diviseur de nombre entier
int EntDiv(int diviseur,int max=4096)
  {float f;int i,d=1;
 while(1)
 { d++;
f=(float)diviseur/d; //cout<<"f:"<<f<<endl;
i=(diviseur/d); //cout<<"i:"<<i<<endl;

if( ((f==i)||(f==i+1)) )
{
if(diviseur/d<=max)return d;
}
if((d>max)){return 1;}
if( (d>=diviseur) ){return 1;}

}
  }//end EntDiv


/*TRIGONOMETRIE en degrées*/
double _cos(double x){return cos(x*_PI/180);}
double _sin(double x){return sin(x*_PI/180);}
double _tan(double x){return tan(x*_PI/180);}

int _round(float x)
{int i=(int)x;float d=x-i; return (d<=0.5)?i:i+1;}

double absolute(double x)
{
	return (x<0)?-x:x;
}

int sign(double x){return (x<0)?-1:1; }
/*
POINT indexToPoint(int index,int width, int height)
{
	int l,c,c0;
	double d;

	d=((double)index)/(width+0.1);

	l=(int)d+1; //retrouver la ligne

	c0= l*width - width;//valeur de la colone 1

	c = (index-c0)+1;

	POINT p;
	p.x=l; p.y=c;
	return p;
}
*/

POINT indexToPoint(int index,int width, int height)
{
	int l,c,c0;
	double d;

	d=((double)index)/(width);

	l=(int)d+1; //retrouver la ligne

	c0= l*width - width;//valeur de la colone 1

	c = (index-c0)+1;

	POINT p;
	p.x=l; p.y=c;
	return p;
}



//division euclidienne
double divE(double dividand,double diviseur, long int &quotient, int &rest)
{
    double result =(dividand/diviseur);
    quotient= (long int) floor(result);
    rest=(dividand-quotient*diviseur);
    return result;
}

//retrieve r,g,b color component
void toRGB(int color,int& r,int& g,int& b)
{
b=(int)color/65536;
g=(int)(color-b*65536)/256;
r=(int)(color-b*65536-g*256);
}//toRGB

//règle de 3 pour calculer un pourcentage
short percentValue(long value,long total)
{
	if(value>=total) return 100;
	else
	{
		short p=( ((value)*100)/(total));
		//return (byte)p;
		return p;
	}
}

#endif
