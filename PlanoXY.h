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
public:
    PlanoXY(HDC h,int nivelDeZoom,PuntoI i,PuntoI f);

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
