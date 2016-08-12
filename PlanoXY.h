#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include "resource.h"
#include "Punto.h"
using namespace std;

class PlanoXY{

private:
    int zoom;
    PuntoI pIni;
    PuntoI pFin;
    PuntoF pCen;
    HDC hdc;
    // agregar punto transformado
    PuntoF getCentro();
    void dibujarPlano();
    float intLagrange(PuntoF *puntos,int n,float x);
    float f(float x,int r,int sgn,PuntoF pcc);
    float g(float *a0,int n,float s);
    float c_b(float s,float n);
    float fact (float n);

    float f(float x,float z);
    float f(float x,float z,int s);
    float f(float x);

    float xx(float x,float z);
    float yy(float x,float z);
    float zz(float x,float z);

public:
    PlanoXY(HDC h,int nivelDeZoom,PuntoI i,PuntoI f);
    void fSin();
    void fCos();
    void fTan();
    void Newton(PuntoF *puntos,int n ,float h);
    void Lagrange(PuntoF *puntos,int n);
    void circunferencia(float r,PuntoF pcc);

    void integral(float a,float b);
    //void integralGrafic(float a,float b);
    //void areaRectangulo();
    void grafic3D();
};











PlanoXY::PlanoXY(HDC h,int nivelDeZoom,PuntoI i,PuntoI f){
    hdc = h;
	zoom = nivelDeZoom;
	pIni = i;
	pFin = f;
	dibujarPlano();
    pCen = getCentro();
}

PuntoF PlanoXY::getCentro(){
    PuntoF centro;
    centro.x = (pIni.x + pFin.x)/2;
    centro.y = (pIni.y + pFin.y)/2;
    return centro;
}

//-----------------------------------------------------------------------------------------------------------------
// DIBUJA LOS CUADROS (RECIBE UN PUNTO INICIAL Y UN PUNTO FINAL  RECIBE UN "z" QUE ES L TAMANO DE LOS CUADROS O NIVEL DE ZOOM)
//-----------------------------------------------------------------------------------------------------------------
void PlanoXY::dibujarPlano(){
    // DIBUJA EL PLANO
    for (float y = pIni.y; y <= pFin.y; y+=1){
        for (float x = pIni.x; x < pFin.x; x+=1){
            SetPixel(hdc, x, y, RGB(39,40,32));
        }
    }
//////////////////////////////////////////////////////////////////////////////////////////////////
//          PROBABILIDAD DE ERROR PORQUE NO SE USA LA FUNCION  "return_centro"
    // SE OBTIENE EL CENTRO DEL CONTEXTO
    PuntoI centro;
    centro.x = (pIni.x + pFin.x)/2;
    centro.y = (pIni.y + pFin.y)/2;
//////////////////////////////////////////////////////////////////////////////////////////////////
//   LINEAS VERTICALES Y
    // DEL CENTRO PARA ARRIBA
    for (int x = centro.x; x <= pFin.x ; x+=zoom){
        for (int y = pIni.y; y <= pFin.y; ++y){
            SetPixel(hdc, x, y, RGB(55, 55, 55));
        }
    }
    // DEL CENTRO PARA ABAJO
    for (int x = centro.x; pIni.x <= x  ; x-=zoom){
        for (int y = pIni.y; y <= pFin.y; ++y){
            SetPixel(hdc, x, y, RGB(55, 55, 55));
        }
    }
// LINEAS HORIZONTALES X
    // DEL CENTRO PARA DERECHA
    for (int y = centro.y; y <= pFin.y ; y+=zoom){
        for (int x = pIni.x; x <= pFin.x; ++x)    {
            SetPixel(hdc, x, y, RGB(55, 55, 55));
        }
    }
    // DEL CENTRO PARA IZQUIERDA
    for (int y = centro.y; pIni.y <= y ; y-=zoom){
        for (int x = pIni.x; x <= pFin.x; ++x)    {
            SetPixel(hdc, x, y, RGB(55, 55, 55));
        }
    }
/*
    // RECTA VERTICAL  CENTRO
    for (int y = pIni.y; y <= pFin.y; ++y)    {
        SetPixel(hdc, centro.x, y, RGB(53, 222, 243));
    }
    // RECTA HORIZONTAL CENTRO
    for (int x = pIni.x; x <= pFin.x; ++x)    {
        SetPixel(hdc, x, centro.y, RGB(53, 222, 243));
    }
*/
}






float PlanoXY::intLagrange(PuntoF *puntos,int n,float x){
    float result_y = 0;
    float result_tmp = 1;
    for (int i=0;i<n;i++) {
        for (int j=0;j<n;j++) {
            if(i != j){
                result_tmp *= ((x - puntos[j].x)/(puntos[i].x - puntos[j].x));
            }
        }
        result_tmp *= puntos[i].y;
        result_y += result_tmp;
        result_tmp = 1;
    }
    return result_y;
}

//---------------------------------------------------------------------------------------------
// DIBUJA  UNA CURVA A PARTIR DE  VARIOS PUNTOS
//---------------------------------------------------------------------------------------------
void PlanoXY::Lagrange(PuntoF *puntos,int n){
    /*
    // size  tamano del esfera
    float size = zoom * 0.1;
    // punto dde paso transformado
    PuntoF ppt;
    for (int i = 0; i < n; ++i){
        ppt.x = pCen.x + (puntos[i].x * zoom);
        ppt.y = pCen.y - (puntos[i].y * zoom);
        Ellipse(hdc, ppt.x - size, ppt.y - size, ppt.x + size, ppt.y + size);
    }
    */
    // PT  = PUNTO TRANSFORMADO
    PuntoF pt;
    // coordenada en "x" y coordenada en "y"  "MATEMATICAMENTE"
    float x,y;
    for (x = puntos[0].x ; x <= puntos[n-1].x ; x += 0.009) {
        y = intLagrange(puntos,n,x);
        pt.x = pCen.x + (x * zoom);
        pt.y = pCen.y - (y * zoom);
        SetPixel(hdc, pt.x , pt.y , RGB(164, 111, 241));
    }
}










//---------------------------------------------------------------------------------------------
// FUNCION FACTORIAL
//---------------------------------------------------------------------------------------------
float PlanoXY::fact(float n){
	if(n > 1.0){
		return n * fact(n-1.0);
	}else if (n == 1.0){
		return 1.0;
	}
}

//---------------------------------------------------------------------------------------------
//  COHEFICIENTE BINOMIAL
//  RECIBE S QUE ES   S = (x-x0)/h
//  RECIBE N QUE ES  EL VALOR EL QUE SE HALLARA  EL FACTORIAL CUANDO SEA
//      DIFERENTE DE CERO O 1
//---------------------------------------------------------------------------------------------
float PlanoXY::c_b(float s,float n){
    if(n == 0){
        return 1;
    }else if (n == 1){
        return s;
    }
    float res_1 = (1.0/fact(n))*s;
    float res_2 = 1.0;
    for(float i = 1.0;i < n;i+=1.0){
        res_2*=(s-i);
    }
    return res_2*res_1;
}

//---------------------------------------------------------------------------------------------
// FUNCION G() MATEMATICAMENTE ES LA SUMATORIA  DE LA DIFERENCIA POR EL COHEFICIENTE BINOMIAL
// ESTA FUNCION RECIBE:
//      ARRAY  EL CUAL CONTIENE LA PRIMERA FILA DE LA TABLA DE DIFERENCIAS
//      N LA CANTIDAD DE PUNTOS
//      S QUE MATEMATICAMENTE ES  S = (x-x0)/h      donde h es la distancia entre
//                                                  punto y punto en el eje x
//---------------------------------------------------------------------------------------------
float PlanoXY::g(float *a0,int n,float s){
    float y = 0;
    float temp_y = 1;
    for(int i = 0 ; i < n ; i+=1 ){
        temp_y *= a0[i]*c_b(s,i);
        y += temp_y;
        temp_y = 1;
    }
    return y;
}

//---------------------------------------------------------------------------------------------
// DIBUJA  UNA CURVA A PARTIR DE  VARIOS PUNTOS
//  RECIBE:
//       puntos : UN ARRAY DE PUNTOS
//      n  LA CANTIDAD DE PUNTOS
//      H LA DISTANCIA  DE SEPARACION  DE LOS PUNTOS EN EL EJE X
//---------------------------------------------------------------------------------------------
void PlanoXY::Newton(PuntoF *puntos,int n ,float h){
    // tbl_d_dif : la  tabla de diferencias
    // s : variable para almacenar el valor de S  matematicamente
    // a0 : es  un puntero para valores flotantes que contendra
    //      la primera  fila de la tabla de diferencias
    // x : es el valor que matematicamente tendra X de igual manera con y
    // pt :  sera el punto transformado en el plano de la pantalla
    float  tbl_d_dif[11][11];
    float  s ;
    float  *a0;
    float  x,y;
    PuntoF pt;
    // size  tamano del esfera
    float size = zoom * 0.1;
    // se dibuja pequenas circuferencias por cada punto
    // esto para ver los puntos por los cuales pasa la funcion
    for (int i = 0; i < n; ++i){
        pt.x = pCen.x + (puntos[i].x * zoom);
        pt.y = pCen.y - (puntos[i].y * zoom);
        Ellipse(hdc, pt.x - size, pt.y - size, pt.x + size, pt.y + size);
    }

    // SE LLENA LA PRIMERA COLUMNA DE LA TABLLA  CON LOS VALORES DE y
    int jj = 0;
    for (int i=0;i<n;i++) {
        tbl_d_dif[i][jj] = puntos[i].y;
    }

    // SE CREA LA TABLA DE  DIFERENCIAS
    //      NO SE TOMA EN CUENTA LA PRIMERA COLUMNA DE LA TABLA
    //      POR TAL MOTIVO  j EMPIEZA EN 1
    for (int j = 1; j < n; ++j){
        for (int i = 0; i < n-j; ++i){
            tbl_d_dif[i][j] = tbl_d_dif[i+1][j-1]-tbl_d_dif[i][j-1];
        }
    }

    //  SE CREA  a0 QUE ES LA PRIMERA FILA DE  LA TABLA DE DIFERENCIAS
    a0 = new float [n];
    int  ii = 0;
    for (int j = 0; j < n; ++j)   {
        a0[j] = tbl_d_dif[ii][j];
    }

    for (x=puntos[0].x;x<puntos[n-1].x;x+=0.005) {
        s = (x - puntos[0].x)/h;
        y = g(a0,n,s);
        pt.x = pCen.x + (x*zoom);
        pt.y = pCen.y - (y*zoom);
        SetPixel(hdc,pt.x,pt.y,RGB(255, 139, 2));
    }
}









/*-----------------------------------------------------------------------------------------------------------------*/
//  GRAFICA DE FUNCIONES TRIGONOMETRICASSSSSSS
/*-----------------------------------------------------------------------------------------------------------------*/
void PlanoXY::fSin(){
    int pxi = -50;
    int pxf = 50;
    float y;
    PuntoF pt;
    for (float x = pxi; x < pxf; x+=0.0005){
        y = sin(x);
        pt.x = pCen.x + (x * zoom);
        pt.y = pCen.y - (y * zoom);
        SetPixel(hdc, pt.x , pt.y , RGB(160, 100, 240));
    }
}
void PlanoXY::fCos(){
    int pxi = -50;
    int pxf = 50;
    float y;
    PuntoF pt;
    for (float x = pxi; x < pxf; x+=0.0005){
        y = cos(x);
        pt.x = pCen.x + (x * zoom);
        pt.y = pCen.y - (y * zoom);
        SetPixel(hdc, pt.x , pt.y , RGB(160, 100, 240));
    }
}
void PlanoXY::fTan(){
    int pxi = -50;
    int pxf = 50;
    float y;
    PuntoF pt;
    for (float x = pxi; x < pxf; x+=0.0005){
        y = tan(x);
        pt.x = pCen.x + (x * zoom);
        pt.y = pCen.y - (y * zoom);
        SetPixel(hdc, pt.x , pt.y , RGB(160, 100, 240));
    }
}



/*-----------------------------------------------------------------------------------------------------------------*/
//  GRAFICA DE FUNCION  CIRCUNFERENCIA
/*-----------------------------------------------------------------------------------------------------------------*/
void PlanoXY::circunferencia(float r,PuntoF pcc){

    // x varia desde punto centro menos el radio hasta punto centro mas es radio
    //int pxi = -50;
    //int pxf = 50;
    int pxi = pcc.x-r;
    int pxf = pcc.x+r;
    float y ;
    PuntoF pt;
    for (float x = pxi; x < pxf; x+=0.00005){
        y = f(x,r,-1,pcc);
        pt.x = pCen.x + (x * zoom);
        pt.y = pCen.y - (y * zoom);
        SetPixel(hdc, pt.x , pt.y , RGB(255, 2, 106));
    }
    for (float x = pxi; x < pxf; x+=0.00005){
        y = f(x,r,1,pcc);
        pt.x = pCen.x + (x * zoom);
        pt.y = pCen.y - (y * zoom);
        SetPixel(hdc, pt.x , pt.y , RGB(255, 2, 106));
    }
}


// ECUACION DE CIRCUNFERENCIA  CON CENTRO EN pcc (H,K)
float PlanoXY::f(float x,int r,int sgn,PuntoF pcc){
    return  (( sgn * sqrt(pow(r,2) - pow(x,2) + (2*x*pcc.x) - pow(pcc.x,2)) ) + pcc.y);
}







// INTEGRAL
//  PRETENDE CREAR UNA FUNCION QUE DADOS 2 INTERVALOS  NOS DEVUELVA LA INTEGRAL  ENTRE ESOS INTERVALOS
//  LA FUNCION  QUE SE INTEGRARA  YA ESTARA DEFINIDA
//  LUEGO DE DEOVLER  EL RESULTADO POR LA CONSOLA
//  SE PRETENDE GRAFICAR LA FUNCION Y
//  FINALEMTE  MOSTRAR EL AREA  DE LA INTEGRAL
//
void PlanoXY::integral(float a,float b){

	//	n ES LA CANTIDAD DE INTERVALOS
	//	a ES EL PUNTO INICIAL EN X
	//	b ES EL PUNTO FINAL EN X
	//	h ES LA CANTIDAD DEL INTERVALO  ENTRE LA CANTIDAD DE INTERVALOS
	//	I ES EL VALOR DE LA INTEGRAL
	//	s ES EL ACUMULADOR DONDE SE SUMARA LOS VALORES ENTRE LOS INTERVALOS
	//		a Y b ( SIN INCLUIR  ESTOS  EXTREMOS , ES DECIR  A Y B)
	//		YA QUE f(a) Y f(b) NO SE MULTIPLICAN POR 2 ,
	//		EN CAMBIO LOS  VALORES QUE ESTAN EN EL INTERVALO SII

    float n,
        h,
        I,
        f0,
        fn,
        s;

    n = 50;
	h = (b-a)/n;
	f0 = f(a),
	fn = f(b),
	s = 0;

	for (float i = a+h; i <= b-h; i+=h ){
		s += 2*f(i);
	}

	I = (h/2)*(f0+s+fn);
	cout<<"el valor de la integral desde -> "<<a<<" hasta "<<b<<" es  "<<I<<endl;

    //---------------------------------------------------------------

    //  PARA GRAFICAR SE  TENDRA QUE TENER UN INTERVALO EN EL EJE X
    //  x VARIA  DESDE x0 HASTA xn
    float x0,xn;
    PuntoF pt;
    float x,y;
    x0=-50;
    xn=50;

    for(x=x0;x<=xn;x+=0.001){
        y=f(x);
        pt.x=pCen.x+x*zoom;
        pt.y=pCen.y-y*zoom;
        SetPixel(hdc,pt.x,pt.y,RGB(255, 2, 106));
    }

    //--------------------------------------------------------------
    // EN ESTA SECCION SE  GRAFICA EL AREA DEL LA INTEGRAL
    for(x=a;x<=b;x+=0.2){
        for(y=0;y<=f(x);y+=0.2){
            pt.x = pCen.x+x*zoom;
            pt.y = pCen.y-y*zoom;
            SetPixel(hdc,pt.x,pt.y,RGB(117,113,94));
        }
    }

}



//FUNCION CUADRARTICA
float PlanoXY::f(float x){
	//return pow(x,2);
    return pow((x-4),3)+(2*pow((x-4.5),2)+x-1) ;
}



float PlanoXY::f(float x,float z){
    return sqrt ((x*x)+(z*z));
    //return 50-pow(x,2)-pow(z,2);
    //return acos(z-sin(x));
    //return sgn*(sqrt(4000-pow(z,2)-pow(x,2))); //esfera
    //return s*(sqrt (9*(((z*z)/4)-((x*x)/16))));
}


float PlanoXY::f(float x,float z,int sgn){
    //return sqrt ((x*x)+(z*z));
    //return 25-pow(x,2)-pow(z,2);
    //return acos(z-sin(x));
    //return sgn*(sqrt(4000-pow(z,2)-pow(x,2))); //esfera
    //return sgn*(sqrt (9*(((z*z)/4)-((x*x)/16))));
}



/*
void PlanoXY::integralGrafic(float a,float b){
    //	n ES LA CANTIDAD DE INTERVALOS
	//	a ES EL PUNTO INICIAL EN X
	//	b ES EL PUNTO FINAL EN X
	//	h ES EL INTERVALO  ENTRE LA CANTIDAD DE INTERVALOS
	//	I ES EL VALOR DE LA INTEGRAL
	//	s ES EL ACUMULADOR DONDE SE SUMARA LOS VALORES ENTRE LOS INTERVALOS
	//		a Y b ( SIN INCLUIR  ESTOS  EXTREMOS , ES DECIR  A Y B)
	//		YA QUE f(a) Y f(b) NO SE MULTIPLICAN POR 2 ,
	//		EN CAMBIO LOS  VALORES QUE ESTAN EN EL INTERVALO SII



	float 	n = 50;
        h = (b-a)/n,
		I,
		f0 = f(a),
		fn = f(b),
		s = 0;

	for (float i = a+h; i <= b-h; i+=h ){
		s += 2*f(i);
	}

	I = (h/2)*(f0+s+fn);

	cout<<I<<endl;
}
*/


void PlanoXY::grafic3D(){
    int Maxx=600;
    int Maxy=500;

    // ROTACION RESPECTO AL EJE X
    // ROTACION RESPECTO AL EJE Y
    //int rot_x = 60;
    int rot_x = 45;
    int rot_y = 85;
/*
    int rot_x = 45;
    int rot_y = 5;
*/



    int posicion1 = rot_x;
    int posicion2 = rot_y;
    float Elev=posicion1*(2*M_PI)/100;
    float Giro=posicion2*(2*M_PI)/100;
    float x,y,z;
    float var=0.5;
    float xini=-100,xfin=100;
    float posx,posy;
    float x2D,y2D;

    //PuntoF_3D

    //float x,y,z;



    for(x=xini;x<xfin;x+=var){
        for(z=xini;z<xfin;z+=var){
            y=f(x,z);
            x2D= x*cos(Elev)-z*sin(Elev);
            y2D=-x*sin(Elev)*sin(Giro)+y*cos(Giro)-z*cos(Elev)*sin(Giro);
            posx=x2D*Maxx/(xfin-xini);
            posy=y2D*Maxx/(xfin-xini);
            //SetPixel(hdc,pCen.x+posx,pCen.y-posy,RGB(200+x,sqrt(190+y),96+z));
            SetPixel(hdc,pCen.x+posx,pCen.y-posy,RGB(sqrt(200+x),190+y,96+z));
            //SetPixel(hdc,pCen.x+posx,pCen.y-posy,RGB(x*x,z*z,y*y));

            //SetPixel(hdc,pCen.x+posx,pCen.y-posy,RGB(240,230,136));
        }
    }



/*
    for(x=xini;x<xfin;x+=var){
        for(z=xini;z<xfin;z+=var){
            y=f(x,z,1);
            x2D= x*cos(Elev)-z*sin(Elev);
            y2D=-x*sin(Elev)*sin(Giro)+y*cos(Giro)-z*cos(Elev)*sin(Giro);
            posx=x2D*Maxx/(xfin-xini);
            posy=y2D*Maxx/(xfin-xini);
            SetPixel(hdc,pCen.x+posx,pCen.y-posy,RGB(sqrt(200+x),190+y,96+z));
            //SetPixel(hdc,pCen.x+posx,pCen.y-posy,RGB(x*x,z*z,y*y));

            //SetPixel(hdc,pCen.x+posx,pCen.y-posy,RGB(240,230,136));
        }
    }

    for(x=xini;x<xfin;x+=var){
        for(z=xini;z<xfin;z+=var){
            y=f(x,z,-1);
            x2D= x*cos(Elev)-z*sin(Elev);
            y2D=-x*sin(Elev)*sin(Giro)+y*cos(Giro)-z*cos(Elev)*sin(Giro);
            posx=x2D*Maxx/(xfin-xini);
            posy=y2D*Maxx/(xfin-xini);
            SetPixel(hdc,pCen.x+posx,pCen.y-posy,RGB(sqrt(200-x),190+y,96+z));
            //SetPixel(hdc,pCen.x+posx,pCen.y-posy,RGB(240,230,136));
        }
    }
*/



}


