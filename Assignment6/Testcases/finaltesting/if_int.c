int a,b;
void main()
{
	int c,d;
	c=3;
	d=3;
	if(a>b&&c==d||1==1||!(a>b)&&c!=d||1==1){
		c=c+d;
	}
	print a;
	print b;
	if(!(a>b&&c==d)){
		c=c+d;
	}
	if(1==1){
		c=c+d;
	}
	if(c!=d){
		c=c+d;
	}
	print c;
	// print d;
}