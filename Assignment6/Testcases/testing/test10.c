int mm;
int fibonacci(int a)
{
    int z,zz,zzz;
    if(a == 0 || a == 1)
    {
    	zzz = 1;
       return zzz;
    }
    
         z=a-1;
        z = fibonacci(z);
        
         zz=a-2;
        
        zz = fibonacci(zz);

        zzz = z + zz;
   
        return zzz;
   
}


void main()
{   
    int b;
     b=4;
    b = fibonacci(b);
    print b;
}


