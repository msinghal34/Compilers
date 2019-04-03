int a,b;
void main()
{
	int a1,a2,a3,a4,a5,a6;
	a1=2;
	a2=3;
	if(a1>=2){
		a3=1;
	}
	if(a3<=0){
		a4=a1+a3;
	}
	if(a1+a3==2*a2){
		a2=a2+1;
	}
	else if(a2>a3){
		a3=a2;
		a2=a3;
	}
	if(a2<a3){
		a4=4;
	}
	print a1;
	print a2;
	print a3;
	print a4;
}