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


	e=29;
	f=4;
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