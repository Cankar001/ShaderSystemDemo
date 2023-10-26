#include "Core/FileSystem.h"

#ifdef SHADER_SYSTEM_PLATFORM_MACOS

namespace ShaderSystem
{
	bool FileSystem::SetCurrentWorkingDirectory(const std::filesystem::path &inPath)
	{
		return true;
	}

	std::filesystem::path FileSystem::GetCurrentWorkingDirectory()
	{
		return std::filesystem::path();
	}

	bool FileSystem::FileExists(const std::filesystem::path &inPath)
	{
		return true;
	}

	bool FileSystem::RemoveFile(const std::filesystem::path &inPath)
	{
		return true;
	}

	std::string FileSystem::Rename(const std::filesystem::path &inPath, const std::filesystem::path &inNewPath)
	{
		return std::string();
	}

	bool FileSystem::Move(const std::filesystem::path &inFilePath, const std::filesystem::path &inDestPath)
	{
		return true;
	}

	bool FileSystem::Copy(const std::filesystem::path &inDestPath, const std::filesystem::path &inSrcPath, bool inOverrideIfExist)
	{
		return true;
	}

	bool FileSystem::CopyRecursive(const std::filesystem::path &inDestPath, const std::filesystem::path &inSrcPath)
	{
		return true;
	}
	
	int64_t FileSystem::GetFileSize(const std::filesystem::path &inPath)
	{
		return 0;
	}

	bool FileSystem::CreateFolder(const std::filesystem::path &inPath)
	{
		return true;
	}
	
	bool FileSystem::RemoveFolder(const std::filesystem::path &inPath, bool inForceDelete)
	{
		return true;
	}

	bool FileSystem::FolderExists(const std::filesystem::path &inPath)
	{
		return true;
	}

	std::filesystem::path FileSystem::GetPersistentStoragePath()
	{
		return std::filesystem::path();
	}
	
	std::filesystem::path FileSystem::GetUserDocumentsPath()
	{
		return std::filesystem::path();
	}

	std::filesystem::path FileSystem::GetUserDownloadsPath()
	{
		return std::filesystem::path();
	}

	std::filesystem::path FileSystem::GetUserDesktopPath()
	{
		return std::filesystem::path();
	}

	std::filesystem::path FileSystem::GetStartMenuPath()
	{
		return std::filesystem::path();
	}

	std::filesystem::path FileSystem::GetProgamsFilesPath()
	{
		return std::filesystem::path();
	}

	std::filesystem::path FileSystem::GetProgamsX86FilesPath()
	{
		return std::filesystem::path();
	}

	Byte *FileSystem::ReadFile(const std::filesystem::path &inPath, int64_t *outSize)
	{
		return nullptr;
	}

	std::string FileSystem::ReadTextFile(const std::filesystem::path &inPath)
	{
		return std::string();
	}

	bool FileSystem::WriteFile(const std::filesystem::path &inPath, Byte *inBuffer, int64_t inSize)
	{
		return true;
	}

	bool FileSystem::WriteTextFile(const std::filesystem::path &inPath, const std::string &inText)
	{
		return true;
	}

	void FileSystem::OpenInExplorer(const std::filesystem::path &inPath)
	{
	}

	void FileSystem::OpenInBrowser(const std::string &inURL)
	{
	}

	bool FileSystem::HasEnvironmentVariable(const std::string &inKey)
	{
		return true;
	}

	bool FileSystem::SetEnvironmentVariable(const std::string &inKey, const std::string &inValue)
	{
		return true;
	}

	std::string FileSystem::GetEnvironmentVariable(const std::string &inKey)
	{
		return std::string();
	}
}

#endif // SHADER_SYSTEM_PLATFORM_MACOS
