#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <math.h>
#include "resource.h"
#include "Punto.h"

class PlanoXY{

private:
    int zoom;
    PuntoI pIni;
    PuntoI pFin;
    PuntoF pCen;

    HDC hdc;
    PuntoF getCentro();
    void dibujarPlano();
    float intLagrange(PuntoF *puntos,int n,float x);
    float g(float *a0,int n,float s);
    float c_b(float s,float n);
    float fact (float n);
public:
    PlanoXY(HDC h,int nivelDeZoom,PuntoI i,PuntoI f);
    void fSin();
    void fCos();
    void fTan();
    void Newton(PuntoF *puntos,int n ,float h);
    void Lagrange(PuntoF *puntos,int n);
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

    // RECTA VERTICAL  CENTRO
    for (int y = pIni.y; y <= pFin.y; ++y)    {
        SetPixel(hdc, centro.x, y, RGB(53, 222, 243));
    }
    // RECTA HORIZONTAL CENTRO
    for (int x = pIni.x; x <= pFin.x; ++x)    {
        SetPixel(hdc, x, centro.y, RGB(53, 222, 243));
    }

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
    // size  tamano del esfera
    float size = zoom * 0.1;
    // punto dde paso transformado
    PuntoF ppt;
    for (int i = 0; i < n; ++i){
        ppt.x = pCen.x + (puntos[i].x * zoom);
        ppt.y = pCen.y - (puntos[i].y * zoom);
        Ellipse(hdc, ppt.x - size, ppt.y - size, ppt.x + size, ppt.y + size);
    }

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

