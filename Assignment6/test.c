
int g(int a);

void main()
{
	int a;
	a=g(0);
	// a=foo(4,5);

	print a;

}

int f(int a){
	return 0;
}

int g(int a){
	a = f(a);
	return a;
}


// int foo(int a,int b){
// 	int c;
// 	int d;
// 	// int gel(int b){
// 	// 	return b;
// 	// }
// 	// c=a+b;
// 	if(a==0){
// 		return b;
// 	}
// 	// d = foo(a-1,b-1);
// 	// c = c + gel(d);
// 	// return c;
// }