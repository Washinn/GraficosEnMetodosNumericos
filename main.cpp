#include "PlanoXY.h"

HINSTANCE hInst;

BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_PAINT:
        {

            HDC hdc;
            PAINTSTRUCT paint_struct;
            RECT r;
            GetClientRect(hwndDlg, &r);
            hdc = BeginPaint(hwndDlg,&paint_struct);


            PuntoI p_ini(0,0);
            PuntoI p_fin(r.right,r.bottom);
            // SE CREA UNA INSTANCIA DE LA CLASE PLANOO
            PlanoXY plano1(hdc,25,p_ini,p_fin);


            //plano1.integral(1,4,30);

            PuntoF puntos[3];
            puntos[0].asign( -2.0, 4.0);
            puntos[1].asign( -1.0, 1.0);
            puntos[2].asign( 2.0, 4.0);
            plano1.Lagrange(puntos,3);

            PuntoF puntos_2[8];
            puntos_2[0].asign(-6.0, 3.0);
            puntos_2[1].asign(-4.0, 5.0);
            puntos_2[2].asign(-2.0, 6.0);
            puntos_2[3].asign(0.0, 6.6);
            puntos_2[4].asign(2.0, 4.5);
            puntos_2[5].asign(4.0, 4.0);
            puntos_2[6].asign(6.0, 6.0);
            puntos_2[7].asign(8.0, 8.0);
            plano1.Newton(puntos_2,8,2.0);

            plano1.fCos();
            plano1.fSin();
            plano1.fTan();

            PuntoF pcentro;
            pcentro.asign(1.0,2.0);
            plano1.circunferencia(5,pcentro);

            plano1.integral(2,5);
            plano1.grafic3D();
            EndPaint(hwndDlg,&paint_struct);
        }

        case WM_INITDIALOG:
        {
        }
        return TRUE;

        case WM_CLOSE:
        {
            EndDialog(hwndDlg, 0);
        }
        return TRUE;

        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
            }
        }
        return TRUE;
    }
    return FALSE;
}



int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    hInst=hInstance;
    InitCommonControls();
    return DialogBox(hInst, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);
}
