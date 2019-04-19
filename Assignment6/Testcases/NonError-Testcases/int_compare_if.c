int a, b;
int e, f;
void main(){
	int c, d;
	a =10;
	b=23;
	c=50;
	d=2;
	if(a<b){
		b = c/d;
	}
	if(a>b){
		b = c-d;
	}
	if(a==b){
		b = c*d;
	}
	if(c!=d){
		c = a*b;
	}
	if(c>=d){
		c=a-b;
	}
	if(c<=d){
		c=a+b;
	}


	e=29;
	f=4;
	if(e>f){
		f=b*d;
		if(f>e){
			e=f+e;
		}
	}

	print a;
	print b;
	print c;
	print d;
	print e;
	print f;
}