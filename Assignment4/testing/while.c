float a,b;
void main()
{
	float c,d;
	c=0;
	d=3;
	while(c+d<=3)
		c=c+1;

	do{
		a=a>c?a+d+1:a+1;
	}while(a<10);
}