int m;
int f(int n){
	int ret, k;
	if(n == 0){
		ret =1;
		return ret;
	}
	k = n-1;
	ret = f(k);
	ret = n*ret;
	return ret;
}
void main(){
	int a;
	int b;
	b=5;
	a = f(b);
	print a;
	m=2;
	print m;
}