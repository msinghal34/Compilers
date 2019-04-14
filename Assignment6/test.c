
int f(int a,int b){
	return 0;
}

int g(int a){
	a = f(a,a);
	return a;
}


void main()
{
	int a;
	a=g(0);
	// a=foo(4,5);

	print a;

}


