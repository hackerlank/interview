#include <iostream.h>
#include <stdlib.h>

class TREE {
private:
	int key;
	int value;
	TREE* right;
	TREE* left;

public:



	int GetKey() { return key; }
	void SetKey(int new_key) { key = new_key; }
	int GetValue() { return value; }
	void SetValue(int new_value) { value = new_value; }


	TREE* GetRight() { return right; }
	TREE* GetLeft() { return left; }
	void SetRight(TREE* new_tree) { right = new_tree; }
	void SetLeft(TREE* new_tree) { left = new_tree; }

	TREE(int k, int v)
	{
		SetKey(k);
		SetValue(v);
		SetRight(NULL);
		SetLeft(NULL);
	}


	void Add(int k, int v)
	{
		TREE* tmp_tree = new TREE(k, v);

		TREE* r = 0;
		TREE* l = 0;

		if(tmp_tree->GetKey() > GetKey())
		{
			if(r = GetRight())
				r->Add(k, v);
			else
				SetRight(tmp_tree);
		}
		else
		{
			if(l = GetLeft())
				l->Add(k, v);
			else
				SetLeft(tmp_tree);
		}

	}

	void PrintValue() { cout << endl << GetValue(); }

	void Print()
	{
		TREE* left = GetLeft();
		if(left) left->Print();

		cout << GetValue() << endl;

		TREE* right = GetRight();
		if(right) right->Print();

	}

};

void SumTREE(TREE*& tree, float& storage, int& iter)
{
		TREE* left = tree->GetLeft();
		if(left) SumTREE(left, storage, iter);

		storage += tree->GetValue();
		iter++;

		TREE* right = tree->GetRight();
		if(right) SumTREE(right, storage, iter);
}

void DeleteTree(TREE*& tree, int key, TREE* prev)
{
	if(!tree) return;

	if(key > tree->GetKey())
	{
		TREE* right = tree->GetRight();
		DeleteTree(right, key, tree);
	}
	else if (key < tree->GetKey())
	{
		TREE* left = tree->GetLeft();
		DeleteTree(left, key, tree);
	}
	else
	{
		TREE* left = tree->GetLeft();
		TREE* right = tree->GetRight();

		if((!left || !right) && !(!left && !right))
		{
			if(!left)
			{
				tree->SetKey(right->GetKey());
				tree->SetValue(right->GetValue());
				tree->SetRight(NULL);
				delete right;
			}
			if(!right)
			{
				tree->SetKey(left->GetKey());
				tree->SetValue(left->GetValue());
				tree->SetLeft(NULL);
				delete left;
			}
		}
		else if(!left && !right)
		{
			if(prev)
			{
				tree->GetKey() > prev->GetKey() ? prev->SetRight(NULL) : prev->SetLeft(NULL);
				delete tree;
			}
			else
				delete tree;
		}
		else if(left && right)
		{

			
			TREE* subtr = tree->GetRight();
			TREE* prnt = prev;


				if(subtr->GetLeft())
				{
					while(subtr->GetLeft())
					{
						if(subtr->GetLeft()->GetLeft())
						{
							prnt = subtr;
							subtr = subtr->GetLeft();
						}
					}
				}

				tree->SetKey(subtr->GetKey());
				tree->SetValue(subtr->GetValue());
				prnt->SetLeft(subtr->GetRight());
				// !
				delete subtr;
			

		}
	}

}


void DeleteMostLeftLeaf(TREE*& tree)
{
	TREE* temp = tree;

	while(temp->GetLeft())
	{
		temp = temp->GetLeft();
	}

	DeleteTree(tree, temp->GetKey(), tree);
}

void GetMinMax(TREE*& tree, TREE* min, TREE* max)
{
		TREE* left = tree->GetLeft();
		if(left) GetMinMax(left, min, max);

		if(tree->GetValue() < min->GetValue())
			min = tree;

		if(tree->GetValue() > max->GetValue())
			max = tree;

		TREE* right = tree->GetRight();
		if(right) GetMinMax(right, min, max);
}

void SwapMinMax(TREE* min, TREE* max)
{
	TREE* temp = new TREE(0, 0);

	temp->SetValue(min->GetValue());
	min->SetValue(max->GetValue());
	max->SetValue(temp->GetValue());

	delete temp;
}

void main()
{
	TREE* m = new TREE(1, 0);

	m->Add(4, 4);
	m->Add(3, 3);
	m->Add(5, 5);

	m->Print();

	float mid = 0.0;
	int c = 0;

	SumTREE(m, mid, c);

	cout << "Srednee arifm.: " << mid / c << endl;
	cout << "deleting MostLeftLeaf" << endl;

	DeleteMostLeftLeaf(m);

	m->Print();

	TREE* min = new TREE(0,100);
	TREE* max = new TREE(0,-100);

	//cout << "Swapping" << endl;
	//GetMinMax(m, min, max);
	//SwapMinMax(min, max);

	m->Print();

	int sd;
	cin >> sd;
}







