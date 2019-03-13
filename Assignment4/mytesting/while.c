float a,b;
void main()
{
	float c,d;
	a=1.0;
	b=2.0;
	c=0.0;
	d=3.0;
	while(c+d<=3.0)
		c=c+1.0;

	do{
		a=a>c?a+d+1.0:a+1.0;
	}while(a<10.0);
}
// void main()
// {
// 	float c,d;
// 	c=0.0;
// 	d=3.0;
// 	while(c+d<=3.0)
// 		c=c+1.0;
// }