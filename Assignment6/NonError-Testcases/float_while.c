float a, b;
float e, f;
void main(){
	float c, d;
	a =10.2;
	b=23.5;
	c=50.3;
	d=2.2;

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