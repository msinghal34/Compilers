int a, b;
int e, f;
void main(){
	int c, d;
	a =10;
	b=23;
	c=50;
	d=2;

	while(a<b){
		c = c*d;
		b = b-d;
	}

	print a;
	print b;
	print c;
	print d;
	print e;
	print f;

	while(a+c>=b+d){
		a = a+d;
		while(a>b){
			a=a-c;
		}
	}

	print a;
	print b;
	print c;
	print d;
	print e;
	print f;

}