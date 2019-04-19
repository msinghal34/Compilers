int ack(int, int);
void main()
{
	int m1, m2, n1, n2;
	int c, d;
	m1 = 1;
	m2 = 4;
	n1 = 2;
	n2 = 0;
	c = ack(m1, n1);
	d = ack(m2, n2);		
	print c;
	print d;
}

int ack(int m, int n)
{
	if (m == 0)
	{
		return n+1;
	}
	else if (m > 0 && n == 0)
	{
		return ack(m-1,1);
	}
	else
	{
		return ack(m-1, ack(m, n-1));
	}
}