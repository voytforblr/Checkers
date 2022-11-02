#pragma once
#include <Windows.h>

int fontWidth = 25;
int fontHeight = 120;

HFONT hfnt = CreateFont(fontHeight, // ������ ������
    fontWidth, // ������� ������ �������
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
