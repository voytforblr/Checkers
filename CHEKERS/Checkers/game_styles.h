#pragma once
#include <Windows.h>

int fontWidthGame = 15;
int fontHeightGame = 10;

HFONT hfntGame = CreateFont(fontWidthGame, // ������ ������
    fontHeightGame, // ������� ������ �������
    0, // ���� �������
    0, // ���� ���������� �������� �����
    FW_NORMAL, // ������� ������
    FALSE, // ��������� ��������� ���������� ������
    FALSE, // ��������� ��������� �������������
    FALSE,  // ��������� ��������� ������������
    DEFAULT_CHARSET, // ������������� ������ ��������
    OUT_DEFAULT_PRECIS, // �������� ������
    CLIP_DEFAULT_PRECIS, // �������� ���������
    DEFAULT_QUALITY, // �������� ������
    VARIABLE_PITCH, // ��� ����� ��������� ������ � ���������
    L"Arial");
