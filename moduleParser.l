%option noyywrap

/*PATRO PARA FUNCION*/
FUNCION             [_A-Za-z][A-Za-z0-9_]*/\(.*\)

%%

{FUNCION}           {printf("%s", yytext); return(1);}

<<EOF>> {   
            return(0);
        }

%%

/**
 * Función para iniciar el sistema de entrada. 
 * Comprueba que existe el archivo.
 * Introduce en flex el archivo a analizar. 
 * @param path, ruta al archivo a compilar.
 * @return int, código de error (-1 fallo en el inicio, 1 inicio correcto)
 */ 
int iniciaArchivo(char *path){
    yyin = fopen(path,"r");
    if(yyin==NULL){
        imprimeError(2,-1,NULL);
        return(-1);
    }
    return(1);
}