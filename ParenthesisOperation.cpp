#include "stdafx.h"
#include "Util.h"

#include <stack>
#include <list>
#include <stdlib.h>
using namespace std;

class ParenthesisOperation
{
//괄호연산 트리의 추상노드
//트리는 컴포지트 구조화 패턴을 따른다.
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
	//파싱하여 괄호연산의 결과를 리턴
	unsigned int parse(string const& parenthesisSymbol)
	{
		NodeFactory nodeFactory;
		stack<TreeNode* const> treeBuildStack;

		//루트노드를 생성
		TreeNode* const rootNode = nodeFactory.createNode(TreeNode::Root, 0);
		treeBuildStack.push(rootNode);

		string::size_type length = parenthesisSymbol.length();
		for(string::size_type characterIndex = 0; characterIndex < length;)
		{
			char symbol = parenthesisSymbol[characterIndex];
			//시작 심볼이면
			if(isStartingSymbol(symbol) == true)
			{
				//다음심볼 미리보기가 가능하다면
				string::size_type nextCharacterIndex = characterIndex + 1;
				if(nextCharacterIndex < length)
				{
					//다음 심볼을 미리봄
					char lookAhead = parenthesisSymbol[nextCharacterIndex];
					//현재 심볼과 다음 심볼이 서로 매칭하면
					if(isMatching(symbol, lookAhead) == true)
					{
						//리프노드를 생성
						TreeNode const* const leafNode = nodeFactory.createNode(TreeNode::Leaf, getOperand(symbol));
						//빌드 스택의 탑노드를 루트로 하는 부분트리 빌드
						static_cast<CompositeNode* const>(treeBuildStack.top())->addChild(leafNode);

						//다음 심벌은 스킵
						++characterIndex;
					}
					//미리보기도 시작 심볼이면
					else
						if(isStartingSymbol(lookAhead) == true)
						{
							//컴포지트 노드 생성
							TreeNode* const compositeNode = nodeFactory.createNode(TreeNode::Composite, getOperand(symbol));
							//빌드스택에 노드 추가
							treeBuildStack.push(compositeNode);
						}
						//그외의 경우라면
						else
						{
							//올바르지 않은 구성의 괄호 심벌
							return 0;
						}
				}
				//그렇지 않다면
				else
				{
					//올바르지 않은 구성의 괄호 심벌
					return 0;
				}
			}
			else
				//종료 심볼이면
				if(isEndingSymbol(symbol) == true)
				{
					CompositeNode* const currentSubtTreeRootNodeReadyForBuild = static_cast<CompositeNode* const>(treeBuildStack.top());
					//컴포지트 노드와 매치되는 종료 심벌이면
					if(isMatching(currentSubtTreeRootNodeReadyForBuild, symbol) == true)
					{
						//부분값 계산
						currentSubtTreeRootNodeReadyForBuild->compose();
						//컴포지트 노드 스택에서 제거
						treeBuildStack.pop();
						//빌드 스택의 탑노드를 루트로 하는 
						static_cast<CompositeNode* const>(treeBuildStack.top())->addChild(currentSubtTreeRootNodeReadyForBuild);
					
					}
					//그렇지 않다면
					else
					{
						//올바르지 않은 구성의 괄호 심벌
						return 0;
					}
				
				}
				//괄호 심벌이 올바른 구성이 아님
				else
				{
					return 0;
				}

			//다음 심벌로 이동
			++characterIndex;
		}

		//스택에 남아있는 노드가 1개라면
		if(treeBuildStack.size() == 1)
		{
			TreeNode* const lastNode = treeBuildStack.top();
			//루트 노드라면
			if(lastNode->getType() == TreeNode::Root)
			{
				RootNode* const root = static_cast<RootNode* const>(lastNode);
				//괄호 심볼 전체합 계산
				root->compose();
				//전체합을 얻어옴.
				return root->getResult();
			}
			//그렇지 않다면
			else
			{
				//올바르지 않은 구성의 괄호 심벌
				return 0;
			}
		}
		//그렇지 않다면
		else
		{
			//올바르지 않은 구성의 괄호 심벌
			return 0;
		}
	}

private:
	//시작 심벌과 종료 심벌이 서로 매치되는 심벌인지 검사
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

	//컴포지트 노드와 종료 심벌이 서로 매치 되는지 검사
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

	//시작 심벌인지 검사
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

	//종료 심벌인지 검사
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

	//심벌에 대한 피연산자값을 얻어옴
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
	//괄호 연산을 수행
	vector<string> operate(string const& fileName)
	{
		vector<string> results;

		//파일내용 가져옴.
		char const* const content = Util::readFile(fileName.c_str());
		//유효한 파일이면
		if(content)
		{
			//파일 내용으로부터 괄호 심벌들 가져옴
			vector<string> parenthesisSymbolLines = Util::readByLine(content);
			delete [] content;

			ParenthesisSymbolParser parenthesisSymbolParser;
			char* sumString = new char[9];
			//각각 괄호 심벌들에 대해
			vector<string>::size_type parenthesisSymbolTotal = parenthesisSymbolLines.size();
			for(vector<string>::size_type parenthesisIndex = 1; parenthesisIndex < parenthesisSymbolTotal; ++parenthesisIndex)
			{
				//괄호 심벌을 파싱하여 총 누계를 얻어옴.
				unsigned int sum = parenthesisSymbolParser.parse(parenthesisSymbolLines[parenthesisIndex]);
				
				_ultoa_s((unsigned long)sum, sumString, 9, 10);
				results.push_back(sumString);
			}
			delete sumString;
		}

		return results;
	}

	//괄호 연산을 수행하여 파일에 씀
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