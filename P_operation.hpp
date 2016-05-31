Node* P_operation(Node* x, int type){
	//printf("P_id = %d  type = %d  PQ_type = %d\n",x -> id, type,x -> PQ_type);
	std::vector<Node* > a, b, c;
	for (int i = 0; i < (int )x -> edge.size(); i ++)
	{
		Node* cur = x -> edge[i];
		if (cur -> size == cur -> sum) a.push_back(cur);
		else if (cur -> sum == 0) b.push_back(cur);
		else c.push_back(cur);
	}
	if ((int )c.size() > 2) throw No_solution();
	if (!(type == 0 || (int )c.size() >= 2)) throw No_solution();
	if ((int )a.size() == 0 && (int )c.size() == 1)
	{
		if (type == 0)
		{
			for (int i = 0; i < (int )x -> edge.size(); i ++)
				if (x -> edge[i] == c[0])
				{
					x -> edge[i] = solve(c[0], 0);
					return x;
				}
		}
	}

	Node* ta = 0;
	int sz = (int )a.size();
	if (sz)
	{
		if (sz == 1) ta = a[0];
		else
		{
			ta = new Node();
			ta -> PQ_type = 0;
			ta -> id = 0;
			ta -> edge = a;
		}
	}

	std::vector<Node* > tmp;
	if (type == 0 || type == 2)
	{
		if ((int )c.size()) tmp.push_back(solve(c[0], 2));
	}
	if (ta != NULL)
		tmp.push_back(ta);

	if (type == 0)
	{
		if ((int )c.size() == 2)
			tmp.push_back(solve(c[1], 2));
	}
	else if (type == 1)
	{
		int csz = (int )c.size();
		if (csz >= 1)
			tmp.push_back(solve(c[csz - 1], 1));
	}
	
	Node* nc = 0;
	if ((int )tmp.size() == 1) nc = tmp[0];
	else
	{
		nc = new Node();
		nc -> PQ_type = 2;
		nc -> id = 0;
		nc -> edge = tmp;
	}

	Node* nb = 0;
	if ((int )b.size() == 1) nb = b[0];
	else
	{
		nb = new Node();
		nb -> PQ_type = 0;
		nb -> id = 0;
		nb -> edge = b;
	}
	
	Node* r;
	if ((int )b.size() == 0) r = nc;
	else
	{
		r = new Node();
		r -> id = 0;
		if (type == 0)
		{
			r -> PQ_type = 0;
			r -> edge = b;
			r -> edge.push_back(nc);
		}
		else
		{
			r -> PQ_type = 1;
			if (type == 1)
				r -> edge.push_back(nc), r -> edge.push_back(nb);
			else
				r -> edge.push_back(nb), r -> edge.push_back(nc);
		}
	}
	return r;
}
