float a,b;
void main()
{
	float c,d;
	c=0.9;
	d=3.1;
	if(a>b&&c==d||1.0==1.0||!(a>b)&&c!=d||1.0==1.0){
		c=c+d;
	}
	print a;
	print b;
	if(!(a>b&&c==d)){
		c=c+d;
	}
	else if(1.0==1.0){
		c=c+d;
	}
	if(c!=d){
		c=c+d;
	}
	print c;
	// print d;
}
