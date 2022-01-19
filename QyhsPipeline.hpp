#pragma once
#include <string>
#include <vector>
namespace QYHS
{
	class QyhsPipeline
	{
	public:
		QyhsPipeline(const std::string& vertFilePath, const std::string& fragFilePath);

	private:
		std::vector<char> readFile(const std::string& filePath);
		void createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath);
	};

}