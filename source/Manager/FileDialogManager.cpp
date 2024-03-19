#include "FileDialogManager.hpp"

Resources::FileDialogManager::FileDialogManager()
{
}

Resources::FileDialogManager::~FileDialogManager()
{
}

void Resources::FileDialogManager::init()
{
}

std::string Resources::FileDialogManager::OpenFile()
{
    OPENFILENAME OFN;
    TCHAR filePathName[100] = L"";
    TCHAR lpstrFile[100] = L"";
    static TCHAR filter[] = L"모든 파일\0*.*\0텍스트 파일\0*.txt\0fbx 파일\0*.fbx";
 
    memset(&OFN, 0, sizeof(OPENFILENAME));
    OFN.lStructSize = sizeof(OPENFILENAME);
    OFN.hwndOwner = hWnd;
    OFN.lpstrFilter = filter;
    OFN.lpstrFile = lpstrFile;
    OFN.nMaxFile = 100;
    OFN.lpstrInitialDir = L".";

    if (GetOpenFileName(&OFN) != 0) {
        wsprintf(filePathName, L"%s 파일을 열겠습니까?", OFN.lpstrFile);
        MessageBox(hWnd, filePathName, L"열기 선택", MB_OK);
 
        string t = ToString(OFN.lpstrFile);
        
        int a = 0;
    }

}

std::string Resources::FileDialogManager::SaveFile()
{
    return std::string();
}
