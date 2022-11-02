#pragma once
#include <Windows.h>

int fontWidth = 25;
int fontHeight = 120;

HFONT hfnt = CreateFont(fontHeight, // высота шрифта
    fontWidth, // средн€€ ширина символа
    0, // угол наклона
    0, // угол ориентации базисной линии
    FW_NORMAL, // толщина шрифта
    FALSE, // описатель параметра курсивного шрифта
    FALSE, // описатель параметра подчеркивани€
    FALSE,  // описатель параметра зачеркивани€
    DEFAULT_CHARSET, // идентификатор набора символов
    OUT_DEFAULT_PRECIS, // точность вывода
    CLIP_DEFAULT_PRECIS, // точность отсечени€
    DEFAULT_QUALITY, // качество вывода
    VARIABLE_PITCH, // шаг между символами шрифта и семейство
    L"Arial");
