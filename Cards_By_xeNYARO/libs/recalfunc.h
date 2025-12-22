#pragma once

#include "libs.h"

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

bool LoadWordsFromFile(const wchar_t* filePath)
{
    FILE* file = nullptr;
    _wfopen_s(&file, filePath, L"r, ccs=UTF-8");
    if (!file)
    {
        MessageBoxW(nullptr, L"Не удалось открыть файл!", L"Ошибка", MB_ICONERROR);
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
        MessageBoxW(nullptr, L"Недостаточно слов в файле", L"Ошибка", MB_ICONERROR);
        return false;
    }

    std::random_shuffle(allWords.begin(), allWords.end());

    for (int i = 0; i < 5; ++i)
        originalWords[i] = _wcsdup(allWords[i].c_str()); // Копируем в wchar_t*

    std::vector<std::wstring> shuffled(allWords.begin(), allWords.begin() + 5);
    std::random_shuffle(shuffled.begin(), shuffled.end());

    for (int i = 0; i < 5; ++i)
        shuffledWords[i] = _wcsdup(shuffled[i].c_str());

    return true;
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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
    srand((unsigned int)time(NULL));

    if (!LoadWordsFromFile(L"words.txt"))
        return 1;

    wchar_t* word1 = GetWordByIndex(1, L"original");
    wchar_t* word2 = GetWordByIndex(2, L"shuffled");

    MessageBoxW(nullptr, word1, L"original[1]", MB_OK);
    MessageBoxW(nullptr, word2, L"shuffled[2]", MB_OK);

    FreeWordArrays();
    return 0;
}

