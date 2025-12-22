#pragma once

extern wchar_t* originalWords[5];
extern wchar_t* shuffledWords[5];


wchar_t* GetWordByIndex(int index, const wchar_t* listType);
bool LoadWordsFromTwoFiles(const wchar_t* maindb, const wchar_t* subdb);
void FreeWordArrays();

