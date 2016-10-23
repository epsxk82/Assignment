// Assingment.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "Util.h"

#include <fstream>
#include <map>
#include <algorithm>
#include <xfunctional>
using namespace std;

bool compare(pair<string, int> const& first, pair<string, int> const& second)
{
	if(second.second < first.second)
	{
		return true;
	}
	else
	{
		return false;
	}
}


//�α���ǥ
class PopularityVote
{
public:
	//�α� ��ǥ ���ڸ� ���´�.
	vector<string> getWinner(string const& voteFileName)
	{
		return getWinner(getVoteResult(voteFileName));
	}

	//�α���ǥ ���ڸ� ���� ���Ͽ� ����.
	void getWinner(string const& inputFileName, string const& outputFileName)
	{
		vector<string> winner = getWinner(inputFileName);
		Util::writeFile(winner, outputFileName.c_str());
	}

private:
	//�α� ��ǥ ���ڸ� ���´�.
	vector<string> getWinner(map<string, int> const& voteResult)
	{
		vector<string> winner;

		map<string, int>::size_type totalVotes = voteResult.size();
		//�� ��ǥ���� ���� �ʴٸ�
		if(0 < totalVotes)
		{
			//�� ��ǥ�� ũ�Ⱑ 1���� ũ�ٸ�
			if(1 < totalVotes)
			{
				//��ǥ�� ���� �����Ѵ�.
				vector<pair<string, int>> voteResultBuffer(voteResult.begin(), voteResult.end());
				sort(voteResultBuffer.begin(), voteResultBuffer.end(), compare);

				//1�� ��ǥ���� ���´�.
				int topVote = voteResultBuffer.begin()->second;
				//1�� ��ǥ���� �ش� �ϴ� ��� ������� ���´�.
				vector<pair<string, int>>::size_type totalOfVotes = voteResultBuffer.size();
				for(vector<pair<string, int>>::size_type voteIndex = 0; voteIndex < totalOfVotes; ++voteIndex)
				{
					pair<string, int> const& vote = voteResultBuffer[voteIndex];
					if(vote.second == topVote)
					{
						winner.push_back(vote.first);
					}
					else
					{
						break;
					}
				}

				//�̸����� �����Ѵ�
				sort(winner.begin(), winner.end());
			}
			//�� ��ǥ���� 1�̶��	
			else
			{
				//�� �ѻ���� ����
				winner.push_back(voteResult.begin()->first);
			}
		}

		return winner;
	}

	//���Ϸκ��� ��ǥ����� ���´�.
	map<string, int> getVoteResult(string const& voteFileName)
	{
		map<string, int> indivisualVoteGain;

		char const* const fileBuffer = Util::readFile(voteFileName.c_str());
		if(fileBuffer)
		{
			//��ǥ�� �����Ѵ�.

			stringstream fileContent(fileBuffer);
			delete [] fileBuffer;
			//������ ��ǥ���� ������ ������
			while(fileContent.eof() == false)
			{
				//�̸��� �о�´�.
				string name;
				fileContent >> name;
				//�̸��� ��ȿ�� �����̶��, ǥ�� ī��Ʈ �Ѵ�.
				if(validName(name) == true)
				{
					if(0 < indivisualVoteGain.count(name))
					{
						++indivisualVoteGain[name];
					
					}
					else
					{
						indivisualVoteGain[name] = 1;
					}
				}
			}
		}
		

		return indivisualVoteGain;
	}

	//�̸��� ��ȿ���� �˻�
	bool validName(string const& name)
	{
		//�̸��� ���ٸ�
		if(name.empty() == true)
		{
			return false;
		}
		else
		{
			//�̸��� 50�ڸ� �Ѿ�ٸ�
			if(50 < name.size())
			{
				return false;
			}
			else
			{
				//�̸��� �����̽��� �ִٸ�
				if(name.find(" ") == string::npos)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	PopularityVote popularityVote;
	
	popularityVote.getWinner("1.input.txt", "output.1.txt");
	popularityVote.getWinner("2.input.txt", "output.2.txt");
	popularityVote.getWinner("3.input.txt", "output.3.txt");
	popularityVote.getWinner("4.input.txt", "output.4.txt");
	popularityVote.getWinner("5.input.txt", "output.5.txt");
	popularityVote.getWinner("6.input.txt", "output.6.txt");
	popularityVote.getWinner("7.input.txt", "output.7.txt");
	popularityVote.getWinner("8.input.txt", "output.8.txt");
	popularityVote.getWinner("9.input.txt", "output.9.txt");
	popularityVote.getWinner("10.input.txt", "output.10.txt");
	popularityVote.getWinner("11.input.txt", "output.11.txt");

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
	Util::checkEqual("11.output.txt", "output.11.txt");
	
	return 0;
}