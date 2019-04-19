int gcd(int, int);
int gcd(int m, int n)
{
	int f;
	// if (n == 0)
	// {
	// 	return m;
	// }
	// else
	// {
	// 	f = m - (m/n)*n;
	// 	m = n;
	// 	n = f;
	// 	f = gcd(m,n);
	// 	return f; 
	// }
}
void main1(int a)
{
	int m1, n1;
	int c;
	m1 = 132;
	n1 = 152;
	c = gcd(m1, n1);
	print c;
}
