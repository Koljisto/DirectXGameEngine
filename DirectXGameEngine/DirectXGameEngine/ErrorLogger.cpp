#include "ErrorLogger.h"
#include <comdef.h>

void ErrorLogger::Log(const std::string& message)
{
	const std::string error_message = "Error: " + message;
	MessageBoxA(NULL, error_message.c_str(), "Error", MB_ICONERROR);
}

void ErrorLogger::Log(const HRESULT hr, const std::string& message)
{
	const _com_error error(hr);
	const std::wstring error_message = L"Error: " + StringConverter::StringToWide(message) + L"\n" + error.ErrorMessage();
	MessageBoxW(NULL, error_message.c_str(), L"Error", MB_ICONERROR);
}

void ErrorLogger::Log(HRESULT hr, const std::wstring& message)
{
	const _com_error error(hr);
	const std::wstring error_message = L"Error: " + message + L"\n" + error.ErrorMessage();
	MessageBoxW(NULL, error_message.c_str(), L"Error", MB_ICONERROR);
}
