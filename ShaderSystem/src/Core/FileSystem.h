#pragma once

#include "Core/Core.h"

#include <filesystem>
#include <string>

namespace ShaderSystem
{
	class FileSystem
	{
	public:

		virtual ~FileSystem() {}

		static bool SetCurrentWorkingDirectory(const std::filesystem::path& inPath);
		static std::filesystem::path GetCurrentWorkingDirectory();

		static bool FileExists(const std::filesystem::path& inPath);

		static bool RemoveFile(const std::filesystem::path& inPath);
		static std::string Rename(const std::filesystem::path& inPath, const std::filesystem::path& inNewPath);
		static bool Move(const std::filesystem::path& inFilePath, const std::filesystem::path& inDestPath);
		static bool Copy(const std::filesystem::path& inDestPath, const std::filesystem::path& inSrcPath, bool inOverrideIfExist = false);
		static bool CopyRecursive(const std::filesystem::path& inDestPath, const std::filesystem::path& inSrcPath);
		static int64_t GetFileSize(const std::filesystem::path& inPath);

		static bool CreateFolder(const std::filesystem::path& inPath);
		static bool RemoveFolder(const std::filesystem::path& inPath, bool inForceDelete = false);
		static bool FolderExists(const std::filesystem::path& inPath);

		static std::filesystem::path GetPersistentStoragePath();
		static std::filesystem::path GetUserDocumentsPath();
		static std::filesystem::path GetUserDownloadsPath();
		static std::filesystem::path GetUserDesktopPath();
		static std::filesystem::path GetStartMenuPath();
		static std::filesystem::path GetProgamsFilesPath();
		static std::filesystem::path GetProgamsX86FilesPath();

		static Byte* ReadFile(const std::filesystem::path& inPath, int64_t* outSize);
		static std::string ReadTextFile(const std::filesystem::path& inPath);

		static bool WriteFile(const std::filesystem::path& inPath, Byte* inBuffer, int64_t inSize);
		static bool WriteTextFile(const std::filesystem::path& inPath, const std::string& inText);

		static void OpenInExplorer(const std::filesystem::path& inPath);
		static void OpenInBrowser(const std::string& inURL);

		static bool HasEnvironmentVariable(const std::string& inKey);
		static bool SetEnvironmentVariable(const std::string& inKey, const std::string& inValue);
		static std::string GetEnvironmentVariable(const std::string& inKey);
	};
}
