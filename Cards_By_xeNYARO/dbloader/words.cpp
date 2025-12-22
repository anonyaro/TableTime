#include "words.h"
#include "pch.h"
#include <algorithm>
#include <time.h>
#include "random"
#include <shellapi.h>
#include <commdlg.h> //tOpenFileName, OFN_FILEMUSTEXIST и OPENFILENAME

#include <Lmcons.h>  // UNLEN 

#include <fstream>
#include <mutex>

wchar_t* originalWords[5] = { nullptr };
wchar_t* shuffledWords[5] = { nullptr };

wchar_t* GetWordByIndex(int index, const wchar_t* listType)
{
    if (index < 0 || index >= 5)
        return (wchar_t*)L"";

    if (wcscmp(listType, L"original") == 0)
        return originalWords[index];
    else if (wcscmp(listType, L"shuffled") == 0)
        return shuffledWords[index];

    return (wchar_t*)L"";
}

bool LoadWordsIntoArray(const wchar_t* filePath, wchar_t* targetArray[5])
{
    FILE* file = nullptr;
    _wfopen_s(&file, filePath, L"r, ccs=UTF-8");
    if (!file)
    {
        MessageBoxW(nullptr, (std::wstring(L"Не удалось получить доступ к Базе Данных:\n") + filePath).c_str(), L"Ошибка", MB_ICONERROR);
        return false;
    }

    std::vector<std::wstring> allWords;
    wchar_t buffer[256];

    while (fgetws(buffer, sizeof(buffer) / sizeof(wchar_t), file))
    {
        buffer[wcscspn(buffer, L"\r\n")] = 0;
        if (wcslen(buffer) > 0)
            allWords.push_back(buffer);
        if (allWords.size() >= 200)
            break;
    }

    fclose(file);

    if (allWords.size() < 5)
    {
        MessageBoxW(nullptr, (std::wstring(L"Недостаточно слов в файле:\n") + filePath).c_str(), L"Ошибка", MB_ICONERROR);
        return false;
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(allWords.begin(), allWords.end(), g);

    for (int i = 0; i < 5; ++i)
        targetArray[i] = _wcsdup(allWords[i].c_str());

    return true;
}

bool LoadWordsFromTwoFiles(const wchar_t* maindb,const wchar_t* subdb)
{
    bool ok1 = LoadWordsIntoArray(maindb, originalWords);
    bool ok2 = LoadWordsIntoArray(subdb, shuffledWords);
    return ok1 && ok2;
}

void FreeWordArrays()
{
    for (int i = 0; i < 5; ++i)
    {
        free(originalWords[i]);
        free(shuffledWords[i]);
        originalWords[i] = nullptr;
        shuffledWords[i] = nullptr;
    }
}
