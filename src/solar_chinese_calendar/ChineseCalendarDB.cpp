/**
 * ChineseCalendarDB.cpp
 * @Author   Tu Yongce <yongce@126.com>
 * @Created  2008-12-13 
 * @Modified 2008-12-13   
 * @Version  0.1
 */

#include "ChineseCalendarDB.h"

// 数组中每一个元素存放1901~2050期间每一年的闰月月份，取值范围0~12（0表示该年没有闰月）
// （注：其实每个字节可以存放2年的数据，可以节约75个字节，但运行起来会慢点）
const uint8_t ChineseCalendarDB::sm_leapMonth[YEAR_NUM] = {
    0x00, 0x00, 0x05, 0x00, 0x00, 0x04, 0x00, 0x00, 0x02, 0x00, //1910
    0x06, 0x00, 0x00, 0x05, 0x00, 0x00, 0x02, 0x00, 0x07, 0x00, //1920
    0x00, 0x05, 0x00, 0x00, 0x04, 0x00, 0x00, 0x02, 0x00, 0x06, //1930
    0x00, 0x00, 0x05, 0x00, 0x00, 0x03, 0x00, 0x07, 0x00, 0x00, //1940
    0x06, 0x00, 0x00, 0x04, 0x00, 0x00, 0x02, 0x00, 0x07, 0x00, //1950
    0x00, 0x05, 0x00, 0x00, 0x03, 0x00, 0x08, 0x00, 0x00, 0x06, //1960
    0x00, 0x00, 0x04, 0x00, 0x00, 0x03, 0x00, 0x07, 0x00, 0x00, //1970
    0x05, 0x00, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x06, 0x00, //1980
    0x00, 0x04, 0x00, 0x0A, 0x00, 0x00, 0x06, 0x00, 0x00, 0x05, //1990
    0x00, 0x00, 0x03, 0x00, 0x08, 0x00, 0x00, 0x05, 0x00, 0x00, //2000
    0x04, 0x00, 0x00, 0x02, 0x00, 0x07, 0x00, 0x00, 0x05, 0x00, //2010
    0x00, 0x04, 0x00, 0x09, 0x00, 0x00, 0x06, 0x00, 0x00, 0x04, //2020
    0x00, 0x00, 0x02, 0x00, 0x06, 0x00, 0x00, 0x05, 0x00, 0x00, //2030
    0x03, 0x00, 0x0B, 0x00, 0x00, 0x06, 0x00, 0x00, 0x05, 0x00, //2040
    0x00, 0x02, 0x00, 0x07, 0x00, 0x00, 0x05, 0x00, 0x00, 0x03, //2050
};

// 数组中每一个元素存放1901~2050期间每一年的12个月或13个月（有闰月）的月天数
// 数组元素的低12位或13位（有闰月）分别对应着这12个月或13个月（有闰月），最低位对应着最小月（1月）
// 如果月份对应的位为1则表示该月有30天，否则表示该月有29天。
// （注：农历中每个月的天数只有29天或者30天）
const uint16_t ChineseCalendarDB::sm_monthDay[YEAR_NUM] = {
    0x0752, 0x0EA5, 0x164A, 0x064B, 0x0A9B, 0x1556, 0x056A, 0x0B59, 0x1752, 0x0752, //1910
    0x1B25, 0x0B25, 0x0A4B, 0x12AB, 0x0AAD, 0x056A, 0x0B69, 0x0DA9, 0x1D92, 0x0D92, //1920
    0x0D25, 0x1A4D, 0x0A56, 0x02B6, 0x15B5, 0x06D4, 0x0EA9, 0x1E92, 0x0E92, 0x0D26, //1930
    0x052B, 0x0A57, 0x12B6, 0x0B5A, 0x06D4, 0x0EC9, 0x0749, 0x1693, 0x0A93, 0x052B, //1940
    0x0A5B, 0x0AAD, 0x056A, 0x1B55, 0x0BA4, 0x0B49, 0x1A93, 0x0A95, 0x152D, 0x0536, //1950
    0x0AAD, 0x15AA, 0x05B2, 0x0DA5, 0x1D4A, 0x0D4A, 0x0A95, 0x0A97, 0x0556, 0x0AB5, //1960
    0x0AD5, 0x06D2, 0x0EA5, 0x0EA5, 0x064A, 0x0C97, 0x0A9B, 0x155A, 0x056A, 0x0B69, //1970
    0x1752, 0x0B52, 0x0B25, 0x164B, 0x0A4B, 0x14AB, 0x02AD, 0x056D, 0x0B69, 0x0DA9, //1980
    0x0D92, 0x1D25, 0x0D25, 0x1A4D, 0x0A56, 0x02B6, 0x05B5, 0x06D5, 0x0EC9, 0x1E92, //1990
    0x0E92, 0x0D26, 0x0A56, 0x0A57, 0x14D6, 0x035A, 0x06D5, 0x16C9, 0x0749, 0x0693, //2000
    0x152B, 0x052B, 0x0A5B, 0x155A, 0x056A, 0x1B55, 0x0BA4, 0x0B49, 0x1A93, 0x0A95, //2010
    0x052D, 0x0AAD, 0x0AAD, 0x15AA, 0x05D2, 0x0DA5, 0x1D4A, 0x0D4A, 0x0C95, 0x152E, //2020
    0x0556, 0x0AB5, 0x15B2, 0x06D2, 0x0EA9, 0x0725, 0x064B, 0x0C97, 0x0CAB, 0x055A, //2030
    0x0AD6, 0x0B69, 0x1752, 0x0B52, 0x0B25, 0x1A4B, 0x0A4B, 0x04AB, 0x055B, 0x05AD, //2040
    0x0B6A, 0x1B52, 0x0D92, 0x1D25, 0x0D25, 0x0A55, 0x14AD, 0x04B6, 0x05B5, 0x0DAA, //2050
};

// 二十四节气数据（注：这里是公历日期）
// 1901~2050期间的二十四节气的公历日期满足如下规律：
// 1月  小寒（5~7） 大寒（19~21）
// 2月  立春（3~5） 雨水（18~20）
// 3月  惊蛰（5~7） 春分（20~22）
// 4月  清明（4~6） 谷雨（19~21）
// 5月  立夏（5~7） 小满（20~22）
// 6月  芒种（5~7） 夏至（20~22）
// 7月  小暑（6~8） 大暑（22~24）
// 8月  立秋（7~9） 处暑（22~24）
// 9月  白露（7~9） 秋分（22~24）
// 10月 寒露（7~9） 霜降（23~24）
// 11月 立冬（7~8） 小雪（21~23）
// 12月 大雪（6~8） 冬至（21~23）
// 可以看出，如果把每月的第二个节气的日期减去15将得到与第一个节气相近的日期，且都在3~9范围内
// 因此，可以使用一个字节来保存每个月两个节气的日期
// 另外，也可以看出，每个节气都在3天的波动范围内，因此还可以近一步把四个节气压缩在一个字节内
// 但那样做使用起来太复杂，因此这里还是把两个节气保存在一个字节中
//
// 数据格式：每个字节的高四位存放每个月的第一个节气的日期，低四位存放该月的第二个
// 节气日期减去15之后的日期
const uint8_t ChineseCalendarDB::sm_solarTerm[YEAR_NUM][12] = {
    0x66, 0x44, 0x66, 0x56, 0x67, 0x67, 0x88, 0x89, 0x89, 0x99, 0x88, 0x87, //1901
    0x66, 0x54, 0x66, 0x66, 0x67, 0x77, 0x89, 0x89, 0x89, 0x99, 0x88, 0x88, //1902
    0x66, 0x55, 0x77, 0x66, 0x77, 0x77, 0x89, 0x99, 0x99, 0x99, 0x88, 0x88, //1903
    0x76, 0x55, 0x66, 0x55, 0x66, 0x67, 0x78, 0x88, 0x88, 0x89, 0x88, 0x77, //1904
    0x66, 0x44, 0x66, 0x56, 0x67, 0x67, 0x88, 0x89, 0x89, 0x99, 0x88, 0x87, //1905
    0x66, 0x54, 0x66, 0x66, 0x67, 0x67, 0x89, 0x89, 0x89, 0x99, 0x88, 0x88, //1906
    0x66, 0x55, 0x77, 0x66, 0x77, 0x77, 0x89, 0x99, 0x99, 0x99, 0x88, 0x88, //1907
    0x76, 0x55, 0x66, 0x55, 0x66, 0x67, 0x78, 0x88, 0x88, 0x99, 0x88, 0x77, //1908
    0x66, 0x44, 0x66, 0x56, 0x67, 0x67, 0x88, 0x89, 0x89, 0x99, 0x88, 0x87, //1909
    0x66, 0x54, 0x66, 0x66, 0x67, 0x67, 0x89, 0x89, 0x89, 0x99, 0x88, 0x88, //1910
    0x66, 0x55, 0x77, 0x66, 0x77, 0x77, 0x89, 0x99, 0x99, 0x99, 0x88, 0x88, //1911
    0x76, 0x55, 0x66, 0x55, 0x66, 0x67, 0x78, 0x88, 0x88, 0x99, 0x88, 0x77, //1912
    0x65, 0x44, 0x66, 0x56, 0x67, 0x67, 0x88, 0x89, 0x89, 0x99, 0x88, 0x87, //1913
    0x66, 0x44, 0x66, 0x56, 0x67, 0x67, 0x89, 0x89, 0x89, 0x99, 0x88, 0x88, //1914
    0x66, 0x55, 0x67, 0x66, 0x67, 0x77, 0x89, 0x89, 0x99, 0x99, 0x88, 0x88, //1915
    0x66, 0x55, 0x66, 0x55, 0x66, 0x67, 0x78, 0x88, 0x88, 0x89, 0x87, 0x77, //1916
    0x65, 0x44, 0x66, 0x56, 0x66, 0x67, 0x88, 0x89, 0x88, 0x99, 0x88, 0x77, //1917
    0x66, 0x44, 0x66, 0x56, 0x67, 0x67, 0x89, 0x89, 0x89, 0x99, 0x88, 0x87, //1918
    0x66, 0x55, 0x67, 0x66, 0x67, 0x77, 0x89, 0x89, 0x99, 0x99, 0x88, 0x88, //1919
    0x66, 0x55, 0x66, 0x55, 0x66, 0x67, 0x78, 0x88, 0x88, 0x89, 0x87, 0x77, //1920
    0x65, 0x44, 0x66, 0x55, 0x66, 0x67, 0x88, 0x89, 0x88, 0x99, 0x88, 0x77, //1921
    0x66, 0x44, 0x66, 0x56, 0x67, 0x67, 0x89, 0x89, 0x89, 0x99, 0x88, 0x87, //1922
    0x66, 0x54, 0x66, 0x66, 0x67, 0x77, 0x89, 0x89, 0x99, 0x99, 0x88, 0x88, //1923
    0x66, 0x55, 0x66, 0x55, 0x66, 0x67, 0x78, 0x88, 0x88, 0x89, 0x87, 0x77, //1924
    0x65, 0x44, 0x66, 0x55, 0x66, 0x67, 0x88, 0x89, 0x88, 0x99, 0x88, 0x77, //1925
    0x66, 0x44, 0x66, 0x56, 0x67, 0x67, 0x88, 0x89, 0x89, 0x99, 0x88, 0x87, //1926
    0x66, 0x54, 0x66, 0x66, 0x67, 0x77, 0x89, 0x89, 0x89, 0x99, 0x88, 0x88, //1927
    0x66, 0x55, 0x66, 0x55, 0x66, 0x66, 0x78, 0x88, 0x88, 0x88, 0x77, 0x77, //1928
    0x65, 0x44, 0x66, 0x55, 0x66, 0x67, 0x78, 0x88, 0x88, 0x89, 0x87, 0x77, //1929
    0x66, 0x44, 0x66, 0x56, 0x67, 0x67, 0x88, 0x89, 0x89, 0x99, 0x88, 0x87, //1930
    0x66, 0x54, 0x66, 0x66, 0x67, 0x77, 0x89, 0x89, 0x89, 0x99, 0x88, 0x88, //1931
    0x66, 0x55, 0x66, 0x55, 0x66, 0x66, 0x78, 0x88, 0x88, 0x88, 0x77, 0x77, //1932
    0x65, 0x44, 0x66, 0x55, 0x66, 0x67, 0x78, 0x88, 0x88, 0x99, 0x88, 0x77, //1933
    0x66, 0x44, 0x66, 0x56, 0x67, 0x67, 0x88, 0x89, 0x89, 0x99, 0x88, 0x87, //1934
    0x66, 0x54, 0x66, 0x66, 0x67, 0x67, 0x89, 0x89, 0x89, 0x99, 0x88, 0x88, //1935
    0x66, 0x55, 0x66, 0x55, 0x66, 0x66, 0x78, 0x88, 0x88, 0x88, 0x77, 0x77, //1936
    0x65, 0x44, 0x66, 0x55, 0x66, 0x67, 0x78, 0x88, 0x88, 0x99, 0x88, 0x77, //1937
    0x66, 0x44, 0x66, 0x56, 0x67, 0x67, 0x88, 0x89, 0x89, 0x99, 0x88, 0x87, //1938
    0x66, 0x54, 0x66, 0x66, 0x67, 0x67, 0x89, 0x89, 0x89, 0x99, 0x88, 0x88, //1939
    0x66, 0x55, 0x66, 0x55, 0x66, 0x66, 0x78, 0x88, 0x88, 0x88, 0x77, 0x77, //1940
    0x65, 0x44, 0x66, 0x55, 0x66, 0x67, 0x78, 0x88, 0x88, 0x99, 0x88, 0x77, //1941
    0x66, 0x44, 0x66, 0x56, 0x67, 0x67, 0x88, 0x89, 0x89, 0x99, 0x88, 0x87, //1942
    0x66, 0x54, 0x66, 0x66, 0x67, 0x67, 0x89, 0x89, 0x89, 0x99, 0x88, 0x88, //1943
    0x66, 0x55, 0x66, 0x55, 0x56, 0x66, 0x78, 0x88, 0x88, 0x88, 0x77, 0x77, //1944
    0x65, 0x44, 0x66, 0x55, 0x66, 0x67, 0x78, 0x88, 0x88, 0x89, 0x87, 0x77, //1945
    0x65, 0x44, 0x66, 0x56, 0x67, 0x67, 0x88, 0x89, 0x88, 0x99, 0x88, 0x87, //1946
    0x66, 0x44, 0x66, 0x56, 0x67, 0x67, 0x89, 0x89, 0x89, 0x99, 0x88, 0x88, //1947
    0x66, 0x55, 0x56, 0x55, 0x56, 0x66, 0x78, 0x78, 0x88, 0x88, 0x77, 0x77, //1948
    0x55, 0x44, 0x66, 0x55, 0x66, 0x67, 0x78, 0x89, 0x88, 0x89, 0x87, 0x77, //1949
    0x65, 0x44, 0x66, 0x55, 0x66, 0x67, 0x88, 0x89, 0x88, 0x99, 0x88, 0x87, //1950
    0x66, 0x44, 0x66, 0x56, 0x67, 0x67, 0x89, 0x89, 0x89, 0x99, 0x88, 0x88, //1951
    0x66, 0x55, 0x56, 0x55, 0x56, 0x66, 0x78, 0x78, 0x88, 0x88, 0x77, 0x77, //1952
    0x55, 0x44, 0x66, 0x55, 0x66, 0x67, 0x78, 0x88, 0x88, 0x89, 0x87, 0x77, //1953
    0x65, 0x44, 0x66, 0x55, 0x66, 0x67, 0x88, 0x89, 0x88, 0x98, 0x88, 0x77, //1954
    0x66, 0x44, 0x66, 0x56, 0x67, 0x67, 0x88, 0x89, 0x89, 0x99, 0x88, 0x87, //1955
    0x66, 0x55, 0x55, 0x55, 0x56, 0x66, 0x78, 0x78, 0x88, 0x88, 0x77, 0x77, //1956
    0x55, 0x44, 0x66, 0x55, 0x66, 0x67, 0x78, 0x88, 0x88, 0x89, 0x87, 0x77, //1957
    0x65, 0x44, 0x66, 0x55, 0x66, 0x67, 0x78, 0x88, 0x88, 0x99, 0x88, 0x77, //1958
    0x66, 0x44, 0x66, 0x56, 0x67, 0x67, 0x88, 0x89, 0x89, 0x99, 0x88, 0x87, //1959
    0x66, 0x54, 0x55, 0x55, 0x56, 0x66, 0x78, 0x78, 0x78, 0x88, 0x77, 0x77, //1960
    0x55, 0x44, 0x66, 0x55, 0x66, 0x66, 0x78, 0x88, 0x88, 0x88, 0x77, 0x77, //1961
    0x66, 0x44, 0x66, 0x55, 0x66, 0x67, 0x78, 0x88, 0x88, 0x99, 0x88, 0x77, //1962
    0x66, 0x44, 0x66, 0x56, 0x67, 0x67, 0x88, 0x89, 0x89, 0x99, 0x88, 0x87, //1963
    0x66, 0x54, 0x55, 0x55, 0x56, 0x66, 0x78, 0x78, 0x78, 0x88, 0x77, 0x77, //1964
    0x55, 0x44, 0x66, 0x55, 0x66, 0x66, 0x78, 0x88, 0x88, 0x88, 0x77, 0x77, //1965
    0x65, 0x44, 0x66, 0x55, 0x66, 0x67, 0x78, 0x88, 0x88, 0x99, 0x88, 0x77, //1966
    0x66, 0x44, 0x66, 0x56, 0x67, 0x67, 0x88, 0x89, 0x89, 0x99, 0x88, 0x87, //1967
    0x66, 0x54, 0x55, 0x55, 0x56, 0x56, 0x78, 0x78, 0x78, 0x88, 0x77, 0x77, //1968
    0x55, 0x44, 0x66, 0x55, 0x66, 0x66, 0x78, 0x88, 0x88, 0x88, 0x77, 0x77, //1969
    0x65, 0x44, 0x66, 0x55, 0x66, 0x67, 0x78, 0x88, 0x88, 0x99, 0x88, 0x77, //1970
    0x66, 0x44, 0x66, 0x56, 0x67, 0x67, 0x88, 0x89, 0x89, 0x99, 0x88, 0x87, //1971
    0x66, 0x54, 0x55, 0x55, 0x56, 0x56, 0x78, 0x78, 0x78, 0x88, 0x77, 0x77, //1972
    0x55, 0x45, 0x66, 0x55, 0x56, 0x66, 0x78, 0x88, 0x88, 0x88, 0x77, 0x77, //1973
    0x65, 0x44, 0x66, 0x55, 0x66, 0x67, 0x78, 0x88, 0x88, 0x99, 0x88, 0x77, //1974
    0x66, 0x44, 0x66, 0x56, 0x67, 0x67, 0x88, 0x89, 0x88, 0x99, 0x88, 0x87, //1975
    0x66, 0x54, 0x55, 0x45, 0x56, 0x56, 0x78, 0x79, 0x78, 0x88, 0x77, 0x77, //1976
    0x55, 0x44, 0x66, 0x55, 0x66, 0x66, 0x78, 0x78, 0x88, 0x88, 0x77, 0x77, //1977
    0x65, 0x44, 0x66, 0x55, 0x66, 0x67, 0x78, 0x88, 0x88, 0x89, 0x88, 0x77, //1978
    0x66, 0x44, 0x66, 0x56, 0x66, 0x67, 0x88, 0x89, 0x88, 0x99, 0x88, 0x87, //1979
    0x66, 0x54, 0x55, 0x45, 0x56, 0x56, 0x78, 0x78, 0x78, 0x88, 0x77, 0x77, //1980
    0x55, 0x44, 0x66, 0x55, 0x56, 0x66, 0x78, 0x78, 0x88, 0x88, 0x87, 0x77, //1981
    0x65, 0x44, 0x66, 0x55, 0x66, 0x67, 0x78, 0x88, 0x88, 0x89, 0x87, 0x77, //1982
    0x65, 0x44, 0x66, 0x55, 0x66, 0x67, 0x88, 0x89, 0x88, 0x99, 0x88, 0x87, //1983
    0x66, 0x44, 0x55, 0x45, 0x56, 0x56, 0x77, 0x78, 0x78, 0x88, 0x77, 0x77, //1984
    0x55, 0x44, 0x56, 0x55, 0x56, 0x66, 0x78, 0x78, 0x88, 0x88, 0x77, 0x77, //1985
    0x55, 0x44, 0x66, 0x55, 0x66, 0x67, 0x78, 0x88, 0x88, 0x89, 0x87, 0x77, //1986
    0x65, 0x44, 0x66, 0x55, 0x66, 0x67, 0x78, 0x89, 0x88, 0x99, 0x88, 0x77, //1987
    0x66, 0x44, 0x55, 0x45, 0x56, 0x56, 0x77, 0x78, 0x78, 0x88, 0x77, 0x76, //1988
    0x55, 0x44, 0x55, 0x55, 0x56, 0x66, 0x78, 0x78, 0x78, 0x88, 0x77, 0x77, //1989
    0x55, 0x44, 0x66, 0x55, 0x66, 0x66, 0x78, 0x88, 0x88, 0x89, 0x87, 0x77, //1990
    0x65, 0x44, 0x66, 0x55, 0x76, 0x67, 0x78, 0x88, 0x88, 0x99, 0x88, 0x77, //1991
    0x66, 0x44, 0x55, 0x45, 0x56, 0x56, 0x77, 0x78, 0x78, 0x88, 0x77, 0x76, //1992
    0x55, 0x43, 0x55, 0x55, 0x56, 0x66, 0x78, 0x78, 0x78, 0x88, 0x77, 0x77, //1993
    0x55, 0x44, 0x66, 0x55, 0x66, 0x66, 0x78, 0x88, 0x88, 0x88, 0x77, 0x77, //1994
    0x65, 0x44, 0x66, 0x55, 0x66, 0x67, 0x78, 0x86, 0x88, 0x99, 0x88, 0x77, //1995
    0x66, 0x44, 0x55, 0x45, 0x56, 0x56, 0x77, 0x78, 0x78, 0x88, 0x77, 0x76, //1996
    0x55, 0x43, 0x55, 0x55, 0x56, 0x56, 0x78, 0x78, 0x78, 0x88, 0x77, 0x77, //1997
    0x55, 0x44, 0x66, 0x55, 0x66, 0x66, 0x78, 0x88, 0x88, 0x88, 0x77, 0x77, //1998
    0x65, 0x44, 0x66, 0x55, 0x66, 0x67, 0x78, 0x88, 0x88, 0x99, 0x88, 0x77, //1999
    0x66, 0x44, 0x55, 0x45, 0x56, 0x56, 0x77, 0x78, 0x78, 0x88, 0x77, 0x76, //2000
    0x55, 0x43, 0x55, 0x55, 0x56, 0x56, 0x78, 0x78, 0x78, 0x88, 0x77, 0x77, //2001
    0x55, 0x44, 0x66, 0x55, 0x66, 0x66, 0x78, 0x88, 0x88, 0x88, 0x77, 0x77, //2002
    0x65, 0x44, 0x66, 0x55, 0x66, 0x67, 0x78, 0x88, 0x88, 0x99, 0x88, 0x77, //2003
    0x66, 0x44, 0x55, 0x45, 0x56, 0x56, 0x77, 0x78, 0x78, 0x88, 0x77, 0x76, //2004
    0x55, 0x43, 0x55, 0x55, 0x56, 0x56, 0x78, 0x78, 0x78, 0x88, 0x77, 0x77, //2005
    0x55, 0x44, 0x66, 0x55, 0x56, 0x66, 0x78, 0x78, 0x88, 0x88, 0x77, 0x77, //2006
    0x65, 0x44, 0x66, 0x55, 0x66, 0x67, 0x78, 0x88, 0x88, 0x99, 0x88, 0x77, //2007
    0x66, 0x44, 0x55, 0x45, 0x56, 0x56, 0x77, 0x78, 0x77, 0x88, 0x77, 0x76, //2008
    0x55, 0x43, 0x55, 0x45, 0x56, 0x56, 0x78, 0x78, 0x78, 0x88, 0x77, 0x77, //2009
    0x55, 0x44, 0x66, 0x55, 0x56, 0x66, 0x78, 0x78, 0x88, 0x88, 0x77, 0x77, //2010
    0x65, 0x44, 0x66, 0x55, 0x66, 0x67, 0x78, 0x88, 0x88, 0x89, 0x88, 0x77, //2011
    0x66, 0x44, 0x55, 0x45, 0x55, 0x56, 0x77, 0x78, 0x77, 0x88, 0x77, 0x76, //2012
    0x55, 0x43, 0x55, 0x45, 0x56, 0x56, 0x77, 0x78, 0x78, 0x88, 0x77, 0x77, //2013
    0x55, 0x44, 0x66, 0x55, 0x56, 0x66, 0x78, 0x78, 0x88, 0x88, 0x77, 0x77, //2014
    0x65, 0x44, 0x66, 0x55, 0x66, 0x67, 0x78, 0x88, 0x88, 0x89, 0x87, 0x77, //2015
    0x65, 0x44, 0x55, 0x44, 0x55, 0x56, 0x77, 0x78, 0x77, 0x88, 0x77, 0x76, //2016
    0x55, 0x33, 0x55, 0x45, 0x56, 0x56, 0x77, 0x78, 0x78, 0x88, 0x77, 0x77, //2017
    0x55, 0x44, 0x56, 0x55, 0x56, 0x66, 0x78, 0x78, 0x88, 0x88, 0x77, 0x77, //2018
    0x55, 0x44, 0x66, 0x55, 0x66, 0x66, 0x78, 0x88, 0x88, 0x89, 0x87, 0x77, //2019
    0x65, 0x44, 0x55, 0x44, 0x55, 0x56, 0x67, 0x77, 0x77, 0x88, 0x77, 0x76, //2020
    0x55, 0x33, 0x55, 0x45, 0x56, 0x56, 0x77, 0x78, 0x78, 0x88, 0x77, 0x76, //2021
    0x55, 0x44, 0x55, 0x55, 0x56, 0x66, 0x78, 0x78, 0x78, 0x88, 0x77, 0x77, //2022
    0x55, 0x44, 0x66, 0x55, 0x66, 0x66, 0x78, 0x88, 0x88, 0x89, 0x87, 0x77, //2023
    0x65, 0x44, 0x55, 0x44, 0x55, 0x56, 0x67, 0x77, 0x77, 0x88, 0x77, 0x66, //2024
    0x55, 0x33, 0x55, 0x45, 0x56, 0x56, 0x77, 0x78, 0x78, 0x88, 0x77, 0x76, //2025
    0x55, 0x43, 0x55, 0x55, 0x56, 0x56, 0x78, 0x78, 0x78, 0x88, 0x77, 0x77, //2026
    0x55, 0x44, 0x66, 0x55, 0x66, 0x66, 0x78, 0x88, 0x88, 0x88, 0x77, 0x77, //2027
    0x65, 0x44, 0x55, 0x44, 0x55, 0x56, 0x67, 0x77, 0x77, 0x88, 0x77, 0x66, //2028
    0x55, 0x33, 0x55, 0x45, 0x56, 0x56, 0x77, 0x78, 0x78, 0x88, 0x77, 0x76, //2029
    0x55, 0x43, 0x55, 0x55, 0x56, 0x56, 0x78, 0x78, 0x78, 0x88, 0x77, 0x77, //2030
    0x55, 0x44, 0x66, 0x55, 0x66, 0x66, 0x78, 0x88, 0x88, 0x88, 0x77, 0x77, //2031
    0x65, 0x44, 0x55, 0x44, 0x55, 0x56, 0x67, 0x77, 0x77, 0x88, 0x77, 0x66, //2032
    0x55, 0x33, 0x55, 0x45, 0x56, 0x56, 0x78, 0x78, 0x78, 0x88, 0x77, 0x76, //2033
    0x55, 0x43, 0x55, 0x55, 0x56, 0x56, 0x78, 0x88, 0x78, 0x88, 0x77, 0x77, //2034
    0x55, 0x44, 0x66, 0x55, 0x56, 0x66, 0x78, 0x78, 0x88, 0x88, 0x77, 0x77, //2035
    0x65, 0x44, 0x55, 0x44, 0x55, 0x56, 0x67, 0x77, 0x77, 0x88, 0x77, 0x66, //2036
    0x55, 0x33, 0x55, 0x45, 0x56, 0x56, 0x77, 0x78, 0x78, 0x88, 0x77, 0x76, //2037
    0x55, 0x43, 0x55, 0x55, 0x56, 0x56, 0x78, 0x78, 0x78, 0x88, 0x77, 0x77, //2038
    0x55, 0x44, 0x66, 0x55, 0x56, 0x66, 0x78, 0x78, 0x88, 0x88, 0x77, 0x77, //2039
    0x65, 0x44, 0x55, 0x44, 0x55, 0x56, 0x67, 0x77, 0x77, 0x88, 0x77, 0x66, //2040
    0x55, 0x33, 0x55, 0x45, 0x55, 0x56, 0x77, 0x78, 0x77, 0x88, 0x77, 0x76, //2041
    0x55, 0x43, 0x55, 0x45, 0x56, 0x56, 0x78, 0x78, 0x78, 0x88, 0x77, 0x77, //2042
    0x55, 0x44, 0x66, 0x55, 0x56, 0x66, 0x78, 0x78, 0x88, 0x88, 0x77, 0x77, //2043
    0x65, 0x44, 0x55, 0x44, 0x55, 0x56, 0x67, 0x77, 0x77, 0x78, 0x77, 0x66, //2044
    0x55, 0x33, 0x55, 0x44, 0x55, 0x56, 0x77, 0x78, 0x77, 0x88, 0x77, 0x76, //2045
    0x55, 0x43, 0x55, 0x45, 0x56, 0x56, 0x77, 0x78, 0x78, 0x88, 0x77, 0x77, //2046
    0x55, 0x44, 0x66, 0x55, 0x56, 0x66, 0x78, 0x78, 0x88, 0x88, 0x77, 0x77, //2047
    0x65, 0x44, 0x55, 0x44, 0x55, 0x55, 0x67, 0x77, 0x77, 0x78, 0x76, 0x66, //2048
    0x54, 0x33, 0x55, 0x55, 0x55, 0x56, 0x67, 0x77, 0x77, 0x88, 0x77, 0x76, //2049
    0x55, 0x33, 0x55, 0x45, 0x56, 0x56, 0x77, 0x78, 0x88, 0x88, 0x77, 0x77, //2050
};
