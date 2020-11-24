double factorial(double valor){
    if(valor==0){
        return(1);
    }
    else if(valor==1){
        return(1);
    }
    else{
        return(valor*factorial(valor-1));
    }
}
