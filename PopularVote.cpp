// Assingment.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
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


//인기투표
class PopularityVote
{
public:
	//인기 투표 승자를 얻어온다.
	vector<string> getWinner(string const& voteFileName)
	{
		return getWinner(getVoteResult(voteFileName));
	}

	//인기투표 승자를 얻어와 파일에 쓴다.
	void getWinner(string const& inputFileName, string const& outputFileName)
	{
		vector<string> winner = getWinner(inputFileName);
		Util::writeFile(winner, outputFileName.c_str());
	}

private:
	//인기 투표 승자를 얻어온다.
	vector<string> getWinner(map<string, int> const& voteResult)
	{
		vector<string> winner;

		map<string, int>::size_type totalVotes = voteResult.size();
		//총 득표수가 없지 않다면
		if(0 < totalVotes)
		{
			//총 득표수 크기가 1보다 크다면
			if(1 < totalVotes)
			{
				//득표수 별로 정렬한다.
				vector<pair<string, int>> voteResultBuffer(voteResult.begin(), voteResult.end());
				sort(voteResultBuffer.begin(), voteResultBuffer.end(), compare);

				//1등 득표수를 얻어온다.
				int topVote = voteResultBuffer.begin()->second;
				//1등 득표수에 해당 하는 모든 사람들을 얻어온다.
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

				//이름순로 정렬한다
				sort(winner.begin(), winner.end());
			}
			//총 득표수가 1이라면	
			else
			{
				//그 한사람이 승자
				winner.push_back(voteResult.begin()->first);
			}
		}

		return winner;
	}

	//파일로부터 개표결과를 얻어온다.
	map<string, int> getVoteResult(string const& voteFileName)
	{
		map<string, int> indivisualVoteGain;

		char const* const fileBuffer = Util::readFile(voteFileName.c_str());
		if(fileBuffer)
		{
			//개표를 진행한다.

			stringstream fileContent(fileBuffer);
			delete [] fileBuffer;
			//마지막 투표까지 다읽을 때까지
			while(fileContent.eof() == false)
			{
				//이름을 읽어온다.
				string name;
				fileContent >> name;
				//이름이 유효한 형식이라면, 표를 카운트 한다.
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

	//이름의 유효성을 검사
	bool validName(string const& name)
	{
		//이름이 없다면
		if(name.empty() == true)
		{
			return false;
		}
		else
		{
			//이름이 50자를 넘어간다면
			if(50 < name.size())
			{
				return false;
			}
			else
			{
				//이름에 스페이스가 있다면
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