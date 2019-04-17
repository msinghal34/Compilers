

// int i1,i2;
// void func1(int a);
// float f1,f2;

int f(int a);


// int g(int a,int b){
// 	return b;
// }
int g(int a,int b,int c,float e,int f);
int g(int a,int b,int c,float e,int f){
	a = a+c+f;
	e = e + e;
	return a;
}


void main()
{
	int a;
	// a = g(a,a);
	g(a,a,a,0.1,1);
	a = f(a);
	// a=f(0);
	// a=foo(4,5);

	print a;

}


