#ifndef _CONV_H_
#define _CONV_H_

#include <string>
#include <vector>

/**
* This class is used for variety conversions between variables.
*/
class Conv
{
public:
	Conv() {}
	~Conv() {}
	static std::vector<unsigned char> AsciiToBin(std::string Ascii);
	static std::vector<unsigned char> AsciiToBin(std::wstring Ascii);
	static std::string BinToAscii(std::vector<unsigned char> BinData, bool comma = true);
	static std::string BinToAscii(unsigned char *BinData, int Length, bool comma = true);
	static std::string BinToAscii(unsigned char BinData);
	static std::string BinToText(unsigned char *BinData, size_t LengthOfArray);
	static std::string BinToText(std::vector<unsigned char> BinData);
	static unsigned int BinToInt(std::vector<unsigned char> BinData);
	static unsigned int BinToInt(unsigned char *BinData);
	static unsigned short BinToUShort(unsigned char *BinData, unsigned short Length);
	static std::vector<unsigned char> IntToBin(int Number);
	static std::string IntToStr(int Number);
	static int StrToInt(std::string Number);
	static int WStrToInt(std::wstring Number);
	static std::wstring StringToWstring(std::string Text);
	static std::string WstringToString(std::wstring Text);
	static std::wstring CentToEuro(std::string CentString);
	static std::vector<unsigned char> IntToMsbLsb(int Number);
	static int MsbLsbToInt(unsigned char Msb, unsigned char Lsb);
	static int MsbLsbToInt(std::vector<unsigned char> MsbLsb);
	static std::string CharArrayToString(char *CharArray);
	static std::string WcharToString(wchar_t *WcharArray);
	static std::string UnsignedLongToIpAdress(unsigned long IpAdress);
	static std::string ToLower(std::string Text);
	static std::string ToUpper(std::string Text);
};

#endif //_CONV_H_
