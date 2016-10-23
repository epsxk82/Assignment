#pragma once

#include <string>
#include <stdio.h>
#include <sstream>
#include <vector>
#include <iostream>

class Util
{
public :
	//파일 읽어오는 유틸 함수
	static char const* const readFile(char const* const fileName)
	{
		FILE* file;
		errno_t error = fopen_s(&file, fileName, "rt");
		if(error)
		{
			return NULL;
		}
		else
		{
			fseek(file, 0, SEEK_END);
			long fileSize = ftell(file);
			fseek(file, 0, SEEK_SET);

			char* fileBuffer = new char[fileSize]; 
			int readSize = fread_s(fileBuffer, fileSize, 1, fileSize, file);
			fileBuffer[readSize] = '\0';

			fclose(file);

			return fileBuffer;
		}
	}

	//파일에 쓰는 유틸함수
	static void writeFile(std::vector<std::string> const& result, char const* const fileName)
	{
		FILE* file;
		errno_t error = fopen_s(&file, fileName, "wt");
		if(!error)
		{
			std::string content;
			std::vector<std::string>::size_type resultTotal = result.size();
			for(std::vector<std::string>::size_type resultIndex = 0; resultIndex < resultTotal; ++resultIndex)
			{
				content += (result[resultIndex] + "\n");
			}

			fwrite(content.c_str(), 1, content.size(), file);
			fclose(file);
		}
	}

	//검증을 위해 결과를 출려파일과 비교하는 함수
	static void checkEqual(std::vector<std::string> const& result, std::string const& outputFileName)
	{
		char const* const fileBuffer = readFile(outputFileName.c_str());
		if(fileBuffer)
		{
			std::vector<std::string> outputResult = readByLine(fileBuffer);
			delete [] fileBuffer;

			if(compare(result, outputResult) == true)
			{
				log("equal");
			}
			else
			{
				logAssertError(result, outputResult);
			}
		}
		else
		{
			log("output file is invalid(fileName : " + outputFileName + ")");
		}
	}

	//검증을 위해 두 출력파일을 비교하는 함수
	static void checkEqual(std::string const& firstOutputFileName, std::string const& secondOutputFileName)
	{
		char const* const firstFileBuffer = readFile(firstOutputFileName.c_str());
		char const* const secondFileBuffer = readFile(secondOutputFileName.c_str());
		if(firstFileBuffer && secondFileBuffer)
		{
			std::vector<std::string> firstOutputResult = readByLine(firstFileBuffer);
			delete [] firstFileBuffer;
			std::vector<std::string> secondOutputResult = readByLine(secondFileBuffer);
			delete [] secondFileBuffer;
			if(compare(firstOutputResult, secondOutputResult) == true)
			{
				log("equal");
			}
			else
			{
				logAssertError(firstOutputResult, secondOutputResult);
			}
		}
		else
		{
			if(!firstFileBuffer)
			{
				log("output file is invalid(fileName : " + firstOutputFileName + ")");
			}
			if(!secondFileBuffer)
			{
				log("output file is invalid(fileName : " + secondOutputFileName + ")");
			}
		}
	}

	//new line를 구분자로 사용하여 토큰 리스트를 생성
	static std::vector<std::string> readByLine(char const* const fileBuffer)
	{
		std::vector<std::string> outputResult;

		std::stringstream content(fileBuffer);
		while(content.eof() == false)
		{
			std::string result;
			content >> result;
			if(result.empty() == false)
			{
				outputResult.push_back(result);
			}
		}

		return outputResult;
	}

//유틸 메소드
private:	
	static void log(std::string const& message)
	{
		std::cout << message << std::endl;
	}

	static void logAssertError(std::vector<std::string> const& result, std::vector<std::string> const& outputResult)
	{
		std::cout << "assert failed" << std::endl;
		
		std::cout << "expected : " << std::endl;
		std::vector<std::string>::size_type outputResultTotal = outputResult.size();
		for(std::vector<std::string>::size_type outputResultIndex = 0; outputResultIndex < outputResultTotal; ++outputResultIndex)
		{
			std::cout << outputResult[outputResultIndex] << " ";
		}
		std::cout << std::endl;

		std::cout << "actual : " << std::endl;
		std::vector<std::string>::size_type resultTotal = result.size();
		for(std::vector<std::string>::size_type resultIndex = 0; resultIndex < resultTotal; ++resultIndex)
		{
			std::cout << result[resultIndex] << " ";
		}
		std::cout << std::endl;
	}

	static bool compare(std::vector<std::string> const& result, std::vector<std::string> const& outputResult)
	{
		std::vector<std::string>::size_type resultTotal = result.size();
		std::vector<std::string>::size_type outptuResultTotal = outputResult.size();
		if(resultTotal == outptuResultTotal)
		{
			bool equal = true;
			for(std::vector<std::string>::size_type resultIndex = 0; resultIndex < resultTotal; ++resultIndex)
			{
				if(result[resultIndex] != outputResult[resultIndex])
				{
					equal = false;
					break;
				}
			}

			return equal;
		}
		else
		{
			return false;
		}
	}
};