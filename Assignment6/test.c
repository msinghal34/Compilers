
int foo(int a,int b){
	int c;
	int d;
	int gel(int b){
		return b;
	}
	c=a+b;
	if(a==0){
		return b;
	}
	d = foo(a-1,b-1);
	c = c + gel(d);
	return c;
}


int main()
{
	int a;
	a=1;
	a=foo(4,5);

	// print a;

}