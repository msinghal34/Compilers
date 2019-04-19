float a, b;
float e, f;
void main(){
	float c, d;
	a =10.2;
	b=23.5;
	c=50.3;
	d=2.2;
	if(a<b){
		b = c/d;
	}
	else if(a>b){
		b = c-d;
	}
	if(a==b){
		b = c*d;
	}
	else if(c!=d){
		c = a*b;
	}
	else if(c>=d){
		c=a-b;
	}
	if(c<=d){
		c=a+b;
	}


	e=29.9;
	f=4.1;
	if(e>f){
		f=b*d;
		if(f>e){
			e=f+e;
		}
		else{
			if(f<e){
				f=f+e;
			}
			else{
				e=e-f;
			}
		}
	}

	print a;
	print b;
	print c;
	print d;
	print e;
	print f;
}