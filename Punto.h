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
