int i1,i2;
void func1();
float f1,f2;
int func2(int,float,int);
float func3(int p,float q,float r);

int func4(){
	i1=4;
	return i1+i1;
}

void func1(){
	i2=5;
	f2=5.0;
}

int func2(int a,float b,int i1){
	a=a+10;
	i1=5;
	return a+i1+i2+2;
}

float func3(int a,float b,float c){
	b =b+f2;
	c=c+10.5;
	return b+c;
}

void main(){
	func1();
	// print i2; print f2;
	i2 = func4();
	// print i1; print i2;
	i2 = func2(i1,f2,i2);
	// print i1; print i2;
	f2 = func3(i1,f1,f2);
	print f1; print f2;

}