int a, b;
int e, f;
void main(){
	int c, d;
	a =10;
	b=23;
	c=50;
	d=2;

	a =((a<b)&&(b>d))?c*d:b-d;
	b =!((a<b)&&(b>d))?c*d:b-d;
	c =((a<b)||(b>d))?c*d:b-d;
	d =!((a<b)||(b>d))?c*d:b-d;

	e=(a<b)?(b>d)?a:b:(c<d)?c:d;
	f=(a<b)?(b<d)?a:b:(c>d)?c:d;

	print a;
	print b;
	print c;
	print d;
	print e;
	print f;
}