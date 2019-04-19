float g(float i,float j, float k, float y, int ee);
int f(int);

int f(int e){
	int y;
	int e;
	while (e >= 1)
	{
		e = e - 1;
		e = f(e);
	}
	return e-1;
}

void main()
{
	int a;
	int b;
	float c;
	a = 4;
	b = 5;
	b = f(1==1?a:b);
	print b;
	c=g(4.0,5.0, 3.0+2.0, -1.0*76.0, 23);
	print c;
	a = f(1);
	print a;
}

float g(float i,float j, float k, float y, int r){
	int r,s,t,u;
	s=1+2;
	t=3;
	r=0;
	i = 3.0;
	f(r);
	return 0.0;
}