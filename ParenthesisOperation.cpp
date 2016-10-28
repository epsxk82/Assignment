#include "stdafx.h"
#include "Util.h"

#include <stack>
#include <list>
#include <stdlib.h>
using namespace std;

class ParenthesisOperation
{
//��ȣ���� Ʈ���� �߻���
//Ʈ���� ������Ʈ ����ȭ ������ ������.
class TreeNode
{
public:
	enum NodeType
	{
		Composite = 0,
		Leaf,
		Root
	};

public:
	virtual NodeType getType() const = 0;
	virtual unsigned int getResult() const = 0;

	unsigned int getOperand() const
	{
		return _Operand;
	}
	
protected:
	explicit TreeNode(unsigned int operand) : _Operand(operand)
	{
	}

private:
	unsigned int _Operand;
};


class CompositeNode : public TreeNode
{
public:
	explicit CompositeNode(unsigned int operand) : TreeNode(operand), _Result(0)
	{
	}

	virtual NodeType getType() const
	{
		return Composite; 
	}

	virtual void compose() 	
	{
		unsigned int result = 0;

		list<TreeNode const* const>::const_iterator childLimit = _Children.end();
		for(list<TreeNode const* const>::const_iterator child = _Children.begin(); child != childLimit; ++child)
		{
			result += ((*child)->getResult() % 100000000);
		}
		result *= getOperand();
		result = result % 100000000;

		setResult(result);
	}

	virtual unsigned int getResult() const
	{
		return _Result;
	}

	void addChild(TreeNode const* const child)
	{
		_Children.push_back(child);
	}

protected:
	void setResult(unsigned int result)
	{
		_Result = result;
	}

	list<TreeNode const* const> const& getChildren() const
	{
		return _Children;
	}

private:
	list<TreeNode const* const> _Children;
	unsigned int _Result;
};

class RootNode : public CompositeNode
{
public:
	explicit RootNode(unsigned int operand) : CompositeNode(operand)
	{
	}

	void compose()
	{
		unsigned int result = 0;

		list<TreeNode const* const> const& children = getChildren();
		list<TreeNode const* const>::const_iterator childLimit = children.end();
		for(list<TreeNode const* const>::const_iterator child = children.begin(); child != childLimit; ++child)
		{
			result += ((*child)->getResult() % 100000000);
		}
		result = result % 100000000;

		setResult(result);
	}

	virtual NodeType getType() const
	{
		return TreeNode::Root;
	}
};

class LeafNode : public TreeNode
{
public:
	explicit LeafNode(unsigned int operand) : TreeNode(operand)
	{
	}

	virtual NodeType getType() const
	{ 
		return Leaf; 
	}

	virtual unsigned int getResult() const
	{
		return getOperand();
	}
};

class NodeFactory
{
public:
	TreeNode* createNode(TreeNode::NodeType nodeType, unsigned int operand)
	{
		TreeNode* treeNode = NULL;
		switch(nodeType)
		{
		case TreeNode::Composite:
			treeNode = new CompositeNode(operand);
			break;

		case TreeNode::Leaf:
			treeNode = new LeafNode(operand);
			break;

		case TreeNode::Root:
			treeNode = new RootNode(operand);
			break;
		}
	
		if(treeNode)
		{
			_TreeNodes.push_back(treeNode);
		}
		
		return treeNode;
	}

	~NodeFactory()
	{
		clear();
	}

	void clear()
	{
		list<TreeNode const* const>::const_iterator treeNodeLimit = _TreeNodes.end();
		for(list<TreeNode const* const>::const_iterator treeNode = _TreeNodes.begin(); treeNode != treeNodeLimit; ++treeNode)
		{
			delete *treeNode;
		}
	}

private:;
	list<TreeNode const* const> _TreeNodes;
};


class ParenthesisSymbolParser
{
public:
	//�Ľ��Ͽ� ��ȣ������ ����� ����
	unsigned int parse(string const& parenthesisSymbol)
	{
		NodeFactory nodeFactory;
		stack<TreeNode* const> treeBuildStack;

		//��Ʈ��带 ����
		TreeNode* const rootNode = nodeFactory.createNode(TreeNode::Root, 0);
		treeBuildStack.push(rootNode);

		string::size_type length = parenthesisSymbol.length();
		for(string::size_type characterIndex = 0; characterIndex < length;)
		{
			char symbol = parenthesisSymbol[characterIndex];
			//���� �ɺ��̸�
			if(isStartingSymbol(symbol) == true)
			{
				//�����ɺ� �̸����Ⱑ �����ϴٸ�
				string::size_type nextCharacterIndex = characterIndex + 1;
				if(nextCharacterIndex < length)
				{
					//���� �ɺ��� �̸���
					char lookAhead = parenthesisSymbol[nextCharacterIndex];
					//���� �ɺ��� ���� �ɺ��� ���� ��Ī�ϸ�
					if(isMatching(symbol, lookAhead) == true)
					{
						//������带 ����
						TreeNode const* const leafNode = nodeFactory.createNode(TreeNode::Leaf, getOperand(symbol));
						//���� ������ ž��带 ��Ʈ�� �ϴ� �κ�Ʈ�� ����
						static_cast<CompositeNode* const>(treeBuildStack.top())->addChild(leafNode);

						//���� �ɹ��� ��ŵ
						++characterIndex;
					}
					//�̸����⵵ ���� �ɺ��̸�
					else
						if(isStartingSymbol(lookAhead) == true)
						{
							//������Ʈ ��� ����
							TreeNode* const compositeNode = nodeFactory.createNode(TreeNode::Composite, getOperand(symbol));
							//���彺�ÿ� ��� �߰�
							treeBuildStack.push(compositeNode);
						}
						//�׿��� �����
						else
						{
							//�ùٸ��� ���� ������ ��ȣ �ɹ�
							return 0;
						}
				}
				//�׷��� �ʴٸ�
				else
				{
					//�ùٸ��� ���� ������ ��ȣ �ɹ�
					return 0;
				}
			}
			else
				//���� �ɺ��̸�
				if(isEndingSymbol(symbol) == true)
				{
					CompositeNode* const currentSubtTreeRootNodeReadyForBuild = static_cast<CompositeNode* const>(treeBuildStack.top());
					//������Ʈ ���� ��ġ�Ǵ� ���� �ɹ��̸�
					if(isMatching(currentSubtTreeRootNodeReadyForBuild, symbol) == true)
					{
						//�κа� ���
						currentSubtTreeRootNodeReadyForBuild->compose();
						//������Ʈ ��� ���ÿ��� ����
						treeBuildStack.pop();
						//���� ������ ž��带 ��Ʈ�� �ϴ� 
						static_cast<CompositeNode* const>(treeBuildStack.top())->addChild(currentSubtTreeRootNodeReadyForBuild);
					
					}
					//�׷��� �ʴٸ�
					else
					{
						//�ùٸ��� ���� ������ ��ȣ �ɹ�
						return 0;
					}
				
				}
				//��ȣ �ɹ��� �ùٸ� ������ �ƴ�
				else
				{
					return 0;
				}

			//���� �ɹ��� �̵�
			++characterIndex;
		}

		//���ÿ� �����ִ� ��尡 1�����
		if(treeBuildStack.size() == 1)
		{
			TreeNode* const lastNode = treeBuildStack.top();
			//��Ʈ �����
			if(lastNode->getType() == TreeNode::Root)
			{
				RootNode* const root = static_cast<RootNode* const>(lastNode);
				//��ȣ �ɺ� ��ü�� ���
				root->compose();
				//��ü���� ����.
				return root->getResult();
			}
			//�׷��� �ʴٸ�
			else
			{
				//�ùٸ��� ���� ������ ��ȣ �ɹ�
				return 0;
			}
		}
		//�׷��� �ʴٸ�
		else
		{
			//�ùٸ��� ���� ������ ��ȣ �ɹ�
			return 0;
		}
	}

private:
	//���� �ɹ��� ���� �ɹ��� ���� ��ġ�Ǵ� �ɹ����� �˻�
	bool isMatching(char startingSymbol, char endingSymbol)
	{
		if((startingSymbol == '(') && (endingSymbol == ')'))
		{
			return true;
		}

		if((startingSymbol == '{') && (endingSymbol == '}'))
		{
			return true;
		}

		if((startingSymbol == '[') && (endingSymbol == ']'))
		{
			return true;
		}

		return false;
	}

	//������Ʈ ���� ���� �ɹ��� ���� ��ġ �Ǵ��� �˻�
	bool isMatching(TreeNode const* const node, char endingSymbol)
	{
		unsigned int operand = node->getOperand();
		if((operand == 1) && (endingSymbol == ')'))
		{
			return true;
		}

		if((operand == 2) && (endingSymbol == '}'))
		{
			return true;
		}

		if((operand == 3) && (endingSymbol == ']'))
		{
			return true;
		}

		return false;
	}

	//���� �ɹ����� �˻�
	bool isStartingSymbol(char symbol)
	{
		if((symbol == '(') || (symbol == '{') || (symbol == '['))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//���� �ɹ����� �˻�
	bool isEndingSymbol(char symbol)
	{
		if((')' == symbol) || ('}' == symbol) || (']' == symbol))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//�ɹ��� ���� �ǿ����ڰ��� ����
	unsigned int getOperand(char symbol)
	{
		switch(symbol)
		{
		case '(':
			return 1;

		case '{':
			return 2;

		case '[':
			return 3;

		default:
			return 0;
		}
	}
};

public:
	//��ȣ ������ ����
	vector<string> operate(string const& fileName)
	{
		vector<string> results;

		//���ϳ��� ������.
		char const* const content = Util::readFile(fileName.c_str());
		//��ȿ�� �����̸�
		if(content)
		{
			//���� �������κ��� ��ȣ �ɹ��� ������
			vector<string> parenthesisSymbolLines = Util::readByLine(content);
			delete [] content;

			ParenthesisSymbolParser parenthesisSymbolParser;
			char* sumString = new char[9];
			//���� ��ȣ �ɹ��鿡 ����
			vector<string>::size_type parenthesisSymbolTotal = parenthesisSymbolLines.size();
			for(vector<string>::size_type parenthesisIndex = 1; parenthesisIndex < parenthesisSymbolTotal; ++parenthesisIndex)
			{
				//��ȣ �ɹ��� �Ľ��Ͽ� �� ���踦 ����.
				unsigned int sum = parenthesisSymbolParser.parse(parenthesisSymbolLines[parenthesisIndex]);
				
				_ultoa_s((unsigned long)sum, sumString, 9, 10);
				results.push_back(sumString);
			}
			delete sumString;
		}

		return results;
	}

	//��ȣ ������ �����Ͽ� ���Ͽ� ��
	void operate(string const& inputFilePath, string const& outputFilePath)
	{
		vector<string> result = operate(inputFilePath);
		Util::writeFile(result, outputFilePath.c_str());
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	ParenthesisOperation parenthesisOperation;

	parenthesisOperation.operate("1.input.txt", "output.1.txt");
	parenthesisOperation.operate("2.input.txt", "output.2.txt");
	parenthesisOperation.operate("3.input.txt", "output.3.txt");
	parenthesisOperation.operate("4.input.txt", "output.4.txt");
	parenthesisOperation.operate("5.input.txt", "output.5.txt");
	parenthesisOperation.operate("6.input.txt", "output.6.txt");
	parenthesisOperation.operate("7.input.txt", "output.7.txt");
	parenthesisOperation.operate("8.input.txt", "output.8.txt");
	parenthesisOperation.operate("9.input.txt", "output.9.txt");
	parenthesisOperation.operate("10.input.txt", "output.10.txt");

	Util::checkEqual("1.output.txt", "output.1.txt");
	Util::checkEqual("2.output.txt", "output.2.txt");
	Util::checkEqual("3.output.txt", "output.3.txt");
	Util::checkEqual("4.output.txt", "output.4.txt");
	Util::checkEqual("5.output.txt", "output.5.txt");
	Util::checkEqual("6.output.txt", "output.6.txt");
	Util::checkEqual("7.output.txt", "output.7.txt");
	Util::checkEqual("8.output.txt", "output.8.txt");
	Util::checkEqual("9.output.txt", "output.9.txt");
	Util::checkEqual("10.output.txt", "output.10.txt");
}