#include "Core/FileSystem.h"

#ifdef SHADER_SYSTEM_PLATFORM_WINDOWS

#include "Core/Assert.h"
#include "Core/Logger.h"

#include <Windows.h>
#include <shellapi.h>
#include <fileapi.h>
#include <Shlobj.h>

namespace ShaderSystem
{
	namespace utils
	{
		void CALLBACK FileIOCompleteRoutine(DWORD inErrorCode, DWORD inNumberOfBytesTransfered, LPOVERLAPPED inOverlapped)
		{
		}

		static HANDLE OpenFileInternal(const std::string& inPath)
		{
			return CreateFileA(inPath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
		}

		static int64_t GetFileSizeInternal(HANDLE inFile)
		{
			LARGE_INTEGER size;
			GetFileSizeEx(inFile, &size);
			return size.QuadPart;
		}

		static bool ReadFileInternal(HANDLE inFile, void* inBuffer, int64_t inSize)
		{
			OVERLAPPED ol = { 0 };
			return ReadFileEx(inFile, inBuffer, (DWORD)inSize, &ol, FileIOCompleteRoutine);
		}

		static std::filesystem::path GetFolderById(const KNOWNFOLDERID& inId)
		{
			PWSTR filePath;
			HRESULT result = SHGetKnownFolderPath(inId, KF_FLAG_DEFAULT, NULL, &filePath);
			SHADER_SYSTEM_ASSERT(result == S_OK);

			std::wstring filepathWstr = filePath;
			std::replace(filepathWstr.begin(), filepathWstr.end(), L'\\', L'/');
			return filepathWstr + L"/";
		}
	}

	bool FileSystem::SetCurrentWorkingDirectory(const std::filesystem::path& inPath)
	{
		return ::SetCurrentDirectoryA(inPath.string().c_str());
	}

	std::filesystem::path FileSystem::GetCurrentWorkingDirectory()
	{
		CHAR buffer[MAX_PATH];
		DWORD dwRet;

		dwRet = ::GetCurrentDirectoryA(MAX_PATH, buffer);

		if (dwRet == 0)
		{
			SHADER_SYSTEM_ERROR("GetCurrentDirectory failed {}", GetLastError());
			return std::filesystem::path();
		}

		if (dwRet >= MAX_PATH)
		{
			SHADER_SYSTEM_ERROR("Buffer too small; need {} characters", dwRet);
			return std::filesystem::path();
		}

		buffer[dwRet] = '\0';
		return std::filesystem::path(buffer);
	}

	bool FileSystem::FileExists(const std::filesystem::path& inPath)
	{
		if (inPath.empty())
			return false;

		DWORD result = GetFileAttributesA(inPath.string().c_str());
		return !(result == INVALID_FILE_ATTRIBUTES && GetLastError() == ERROR_FILE_NOT_FOUND);
	}

	bool FileSystem::RemoveFile(const std::filesystem::path& inPath)
	{
		return ::DeleteFileA(inPath.string().c_str());
	}

	std::string FileSystem::Rename(const std::filesystem::path& inPath, const std::filesystem::path& inNewPath)
	{
		std::string newFilePath;

		if (!inPath.parent_path().empty())
			newFilePath = inPath.parent_path().string() + "/" + inNewPath.string();
		else
			newFilePath = inNewPath.string();

		BOOL result = MoveFileA(inPath.string().c_str(), newFilePath.c_str());
		SHADER_SYSTEM_ASSERT(result);
		return newFilePath;
	}

	bool FileSystem::Move(const std::filesystem::path& inFilePath, const std::filesystem::path& inDestPath)
	{
		BOOL result = MoveFileA(inFilePath.string().c_str(), inDestPath.string().c_str());
		return result != 0;
	}

	bool FileSystem::Copy(const std::filesystem::path& inDestPath, const std::filesystem::path& inSrcPath, bool inOverrideIfExist)
	{
		return ::CopyFileA(inSrcPath.string().c_str(), inDestPath.string().c_str(), !inOverrideIfExist);
	}

	bool FileSystem::CopyRecursive(const std::filesystem::path& inDestPath, const std::filesystem::path& inSrcPath)
	{
		std::string source = inSrcPath.string();
		if (source[source.size() - 1] != '/')
		{
			source = source + "\\*";
		}

		SHFILEOPSTRUCTA s = { 0 };
		s.wFunc = FO_COPY;
		s.pTo = inDestPath.string().c_str();
		s.pFrom = source.c_str();
		s.fFlags = FOF_SILENT | FOF_NOCONFIRMMKDIR | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_NO_UI;
		int32_t result = SHFileOperationA(&s);
		return result == 0;
	}

	int64_t FileSystem::GetFileSize(const std::filesystem::path& inPath)
	{
		HANDLE file = utils::OpenFileInternal(inPath.string());
		if (file == INVALID_HANDLE_VALUE)
		{
			return -1;
		}

		int64_t size = utils::GetFileSizeInternal(file);
		CloseHandle(file);
		return size;
	}

	bool FileSystem::CreateFolder(const std::filesystem::path& inPath)
	{
		bool success = CreateDirectoryA(inPath.string().c_str(), NULL);
		if (!success)
			SHADER_SYSTEM_ERROR("WinAPI Error: {0}", GetLastError());

		return success;
	}

	bool FileSystem::RemoveFolder(const std::filesystem::path& inPath, bool inForceDelete)
	{
		if (!inForceDelete)
		{
			bool success = RemoveDirectoryA(inPath.string().c_str());
			if (!success)
				SHADER_SYSTEM_ERROR("WinAPI Error: {0}", GetLastError());

			return success;
		}
		else
		{
			// the user wants to force-delete the directory, without the limitation of only being able to remove an empty directory
			// therefore we use the Windows Shell instead of the Win32 API

			SHFILEOPSTRUCTA fileOperation;
			fileOperation.hwnd = NULL;
			fileOperation.wFunc = FO_DELETE;
			fileOperation.pFrom = inPath.string().c_str();
			fileOperation.pTo = "";
			fileOperation.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT;
			fileOperation.fAnyOperationsAborted = false;
			fileOperation.fFlags = 0;
			fileOperation.hNameMappings = L"";

			int32_t status = SHFileOperationA(&fileOperation);
			if (status != 0)
				SHADER_SYSTEM_ERROR("WinAPI Error: {0}", status);

			return status == 0;
		}
	}

	bool FileSystem::FolderExists(const std::filesystem::path& inPath)
	{
		if (inPath.empty())
			return false;

		DWORD result = GetFileAttributesA(inPath.string().c_str());
		return (result != INVALID_FILE_ATTRIBUTES && (result & FILE_ATTRIBUTE_DIRECTORY));
	}

	std::filesystem::path FileSystem::GetPersistentStoragePath()
	{
		return utils::GetFolderById(FOLDERID_RoamingAppData);
	}

	std::filesystem::path FileSystem::GetUserDocumentsPath()
	{
		return utils::GetFolderById(FOLDERID_Documents);
	}

	std::filesystem::path FileSystem::GetUserDownloadsPath()
	{
		return utils::GetFolderById(FOLDERID_Downloads);
	}

	std::filesystem::path FileSystem::GetUserDesktopPath()
	{
		return utils::GetFolderById(FOLDERID_Desktop);
	}

	std::filesystem::path FileSystem::GetStartMenuPath()
	{
		return utils::GetFolderById(FOLDERID_StartMenu);
	}

	std::filesystem::path FileSystem::GetProgamsFilesPath()
	{
		return utils::GetFolderById(FOLDERID_ProgramFiles);
	}

	std::filesystem::path FileSystem::GetProgamsX86FilesPath()
	{
		return utils::GetFolderById(FOLDERID_ProgramFilesX86);
	}

	Byte* FileSystem::ReadFile(const std::filesystem::path& inPath, int64_t* outSize)
	{
		HANDLE file = utils::OpenFileInternal(inPath.string());
		int64_t size = utils::GetFileSizeInternal(file);
		Byte* buffer = new Byte[size];
		bool result = utils::ReadFileInternal(file, buffer, size);
		CloseHandle(file);

		if (!result)
			delete[] buffer;

		if (outSize)
			*outSize = size;

		return result ? buffer : nullptr;
	}

	std::string FileSystem::ReadTextFile(const std::filesystem::path& inPath)
	{
		HANDLE file = utils::OpenFileInternal(inPath.string());
		if (file == INVALID_HANDLE_VALUE)
		{
			DWORD errorCode = GetLastError();
			SHADER_SYSTEM_ASSERT(false);
		}
		int64_t size = utils::GetFileSizeInternal(file);


		char* readBuffer = new char[size + 1];
		readBuffer[size] = '\0';
		bool success = utils::ReadFileInternal(file, &readBuffer[0], size);
		
		std::string result = std::string(readBuffer);
		
		delete[] readBuffer;
		CloseHandle(file);
		
		return success ? result : "";
	}

	bool FileSystem::WriteFile(const std::filesystem::path& inPath, Byte* inBuffer, int64_t inSize)
	{
		HANDLE file = CreateFileA(inPath.string().c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		if (file == INVALID_HANDLE_VALUE)
			return false;

		DWORD written;
		bool result = ::WriteFile(file, inBuffer, (DWORD)inSize, &written, NULL);
		CloseHandle(file);
		return result;
	}

	bool FileSystem::WriteTextFile(const std::filesystem::path& inPath, const std::string& inText)
	{
		return WriteFile(inPath, (Byte*)&inText[0], (int64_t)inText.size());
	}

	void FileSystem::OpenInExplorer(const std::filesystem::path& inPath)
	{
		ShellExecuteA(0, "open", inPath.string().c_str(), 0, 0, SW_SHOWDEFAULT);
	}

	void FileSystem::OpenInBrowser(const std::string& inURL)
	{
		ShellExecuteA(0, 0, inURL.c_str(), 0, 0, SW_SHOW);
	}

	bool FileSystem::HasEnvironmentVariable(const std::string& inKey)
	{
		HKEY hKey;
		LPCSTR keyPath = "Environment";
		LSTATUS lOpenStatus = RegOpenKeyExA(HKEY_CURRENT_USER, keyPath, 0, KEY_ALL_ACCESS, &hKey);

		if (lOpenStatus == ERROR_SUCCESS)
		{
			lOpenStatus = RegQueryValueExA(hKey, inKey.c_str(), 0, NULL, NULL, NULL);
			RegCloseKey(hKey);
		}

		return lOpenStatus == ERROR_SUCCESS;
	}

#undef SetEnvironmentVariable
	bool FileSystem::SetEnvironmentVariable(const std::string& inKey, const std::string& inValue)
	{
		HKEY hKey;
		LPCSTR keyPath = "Environment";
		DWORD createdNewKey;
		LSTATUS lOpenStatus = RegCreateKeyExA(HKEY_CURRENT_USER, keyPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &createdNewKey);

		if (lOpenStatus == ERROR_SUCCESS)
		{
			LSTATUS lSetStatus = RegSetValueExA(hKey, inKey.c_str(), 0, REG_SZ, (LPBYTE)inValue.c_str(), (DWORD)(inValue.size() + 1));
			RegCloseKey(hKey);

			if (lSetStatus == ERROR_SUCCESS)
			{
				SendMessageTimeoutA(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)"Environment", SMTO_BLOCK, 100, NULL);
				return true;
			}
		}

		return false;
	}

#undef GetEnvironmentVariable
	std::string FileSystem::GetEnvironmentVariable(const std::string& inKey)
	{
		HKEY hKey;
		LPCSTR keyPath = "Environment";
		DWORD createdNewKey;
		LSTATUS lOpenStatus = RegCreateKeyExA(HKEY_CURRENT_USER, keyPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &createdNewKey);

		if (lOpenStatus == ERROR_SUCCESS)
		{
			DWORD valueType;
			DWORD dataSize = 512;
			char* data = new char[dataSize];
			LSTATUS status = RegGetValueA(hKey, NULL, inKey.c_str(), RRF_RT_ANY, &valueType, (PVOID)data, &dataSize);

			RegCloseKey(hKey);

			if (status == ERROR_SUCCESS)
			{
				std::string result(data);
				delete[] data;
				return result;
			}
		}

		return {};
	}
}

#endif // SHADER_SYSTEM_PLATFORM_WINDOWS
