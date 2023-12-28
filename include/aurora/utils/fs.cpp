#include "fs.hpp"

std::string ReadString(const std::string path)
{
    const std::ifstream input_stream(path, std::ios_base::binary);

    if (input_stream.fail()) {
        throw std::runtime_error("Failed to open file located at " + path);
    }

    std::stringstream buffer;
    buffer << input_stream.rdbuf();

    return buffer.str();
}

std::string GetFileNameWithoutExt(std::string path)
{
	std::filesystem::path p(path);


	return p.stem().string();
}

bool WriteString(std::string path, std::string data)
{

	std::filesystem::create_directories(std::filesystem::path(path).parent_path());

	// Create and open a text file
	std::ofstream MyFile(path);

	// Write to the file
	MyFile << data;

	// Close the file
	MyFile.close();

	return true;
}
