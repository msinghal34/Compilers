
void main()
{
	int a, c;
	int b, d;

	a = 3;		// 'a' - register v0
	c = 4;		// 'c' - register v1
	b = a;		// 'b' - register v0
	d = c;		// 'd' - register v1
	a = d;		// 'a' - register v1
	c = b;		// 'c' - register v0
}
