class PuntoF {
public:
    float x;
    float y;
    
    PuntoF(float a,float b){
        x = a;
        y = b;
    }
    
    PuntoF(){
        x = 0;
        y = 0;
    };

    void asign(float a,float b){
        x = a;
        y = b;
    }
};



class PuntoF_3D {
public:
    float x;
    float y;
    float z;
    
    PuntoF_3D(float a,float b,float c){
        x = a;
        y = b;
        x = c;
    }
    
    PuntoF_3D(){
        x = 0;
        y = 0;
        x = 0;
    };

    void asign(float a,float b,float c){
        x = a;
        y = b;
        z = c;
    }
};

////////////////////////////////////////////////////////////////////////

class PuntoI_3D {
public:
    int x;
    int y;
    int z;
    
    PuntoI_3D(int a,int b,int c){
        x = a;
        y = b;
        z = c;
    }
    
    PuntoI_3D(){
        x = 0;
        y = 0;
        z = 0;
    };
    
    void asign(int a,int b,int c){
        x = a;
        y = b;
        z = c;
    }
    
};

class PuntoI {
public:
    int x;
    int y;
    
    PuntoI(int a,int b){
        x = a;
        y = b;
    }
    PuntoI(){
        x = 0;
        y = 0;
    };
    void asign(int a,int b){
        x = a;
        y = b;
    }
};
