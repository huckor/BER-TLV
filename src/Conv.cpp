#include "Conv.h"
#include <stdlib.h>
#include <stdio.h>

/**
* Convert integer to string
*/
std::string Conv::IntToStr(int Number)
{
	char Buffer[15];

	sprintf(Buffer, "%i", Number);
	return std::string(Buffer);
}

/**
* Convert string to integer.
*/
int Conv::StrToInt(std::string Number)
{
	int Return = 0;

	Return = (int)strtol(Number.c_str(), NULL, 10);
	return Return;
}

/**
* Convert wstring to integer.
*/
int Conv::WStrToInt(std::wstring Number)
{
	int Return = 0;
	std::string Tmp = WcharToString((wchar_t *)Number.c_str());

	Return = (int)strtol(Tmp.c_str(), NULL, 10);
	return Return;
}

/**
* Convert byte array to string.
* Example: BinData = 0x03, 0x45, 0x35 
*	return string after conversion will contain 
* string = "034535" (if comma parameter is set to false)
* string = "03, 45, 35" (if comma parameter is set to true)
*/
std::string Conv::BinToAscii(std::vector<unsigned char> BinData, bool comma)
{
	char AsciiHexNo[5];
	std::string Return;

	for(int i = 0; i < (int)BinData.size(); i++)
	{
		sprintf(AsciiHexNo, "%02X", BinData[i]);
		Return += AsciiHexNo;
		if((i + 1) < (int)BinData.size() && comma)
			Return += ", ";
	}

	return Return;
}

/**
* Convert byte array to string.
* Example: BinData = 0x03, 0x45, 0x35 
*	return string after conversion will contain 
* string = "034535" (if comma parameter is set to false)
* string = "03, 45, 35" (if comma parameter is set to true)
*/
std::string Conv::BinToAscii(unsigned char *BinData, int Length, bool comma)
{
	char AsciiHexNo[5];
	std::string Return;

	for(int i = 0; i < Length; i++)
	{
		sprintf(AsciiHexNo, "%02X", BinData[i]);
		Return += AsciiHexNo;
		if((i + 1) < Length && comma)
			Return += ", ";
	}

	return Return;
}

/**
* Convert byte to string.
* Example: BinData = 0x03
*	return string after conversion will contain 
* string = "03"
*/
std::string Conv::BinToAscii(unsigned char BinData)
{
  char AsciiHexNo[5];
  std::string Return;

  sprintf(AsciiHexNo, "%02X", BinData);
  Return += AsciiHexNo;

  return Return;
}

/**
* Convert string which contains ASCI HEX values to byte array.
* Example: string = "034535", after conversion byte array will contain 0x03 0x45 0x35
*/
std::vector<unsigned char> Conv::AsciiToBin(std::string Ascii) 
{
	std::vector<unsigned char> Return;

	if((Ascii.length() % 2) != 0)
		return Return;

	for(size_t i = 0; i < Ascii.length(); i+=2) 
	{
		std::string Tmp = Ascii.substr(i, 2);
		Return.push_back((char)strtol(Tmp.c_str(), NULL, 16));
	}

	return Return;
}

/**
* Convert wstring which contains ASCI HEX values to byte array.
* Example: wstring = "034535", after conversion byte array will contain 0x03 0x45 0x35
*/
std::vector<unsigned char> Conv::AsciiToBin(std::wstring Ascii)
{
	std::vector<unsigned char> Return;

	if((Ascii.length() % 2) != 0)
		return Return;

	for(size_t i = 0; i < Ascii.length(); i+=2) 
	{
		std::string Tmp = WcharToString((wchar_t *)Ascii.substr(i, 2).c_str());
		Return.push_back((char)strtol(Tmp.c_str(), NULL, 16));
	}

	return Return;
}

/**
* Convert char array of bytes to text.
* Example: BinData = 0x30 0x41 0x5A after conversion string will contain "1AZ"
*/
std::string Conv::BinToText(unsigned char *BinData, size_t LengthOfArray)
{
	std::string Return;
	char Tmp[5];

	for(size_t i = 0; i < LengthOfArray; i++)
	{
		sprintf(Tmp, "%c", BinData[i]);
		Return += Tmp;
	}

	return Return;
}

/**
* Convert char array of bytes to text.
* Example: BinData = 0x30 0x41 0x5A after conversion string will contain "1AZ"
*/
std::string Conv::BinToText(std::vector<unsigned char> BinData)
{
	std::string Return;
	char Tmp[5];

	for(size_t i = 0; i < BinData.size(); i++)
	{
		sprintf(Tmp, "%c", BinData[i]);
		Return += Tmp;
	}

	return Return;
}

/**
* Convert string that represent amount to pay in cents to amount to pay in euro.
* Example: CentString = "00000002000" after conversion returned "20.00"
*/
std::wstring Conv::CentToEuro(std::string CentString)
{
	int Number = StrToInt(CentString);
	char *Tmp = new char[CentString.length() + 1];
	std::wstring Return;

	sprintf(Tmp, "%d.%02d", (int)(Number)/100, (int)(Number)%100); 

	Return = StringToWstring(std::string(Tmp));

	delete[] Tmp;

	return Return;
}

/**
* Convert string to wstring, actualy it's not converting any data, it's just copy 
* string array to wstring array.
*/
std::wstring Conv::StringToWstring(std::string Text)
{
	std::wstring Return(Text.length(), L' ');
	std::copy(Text.begin(), Text.end(), Return.begin());
	return Return;
}

/**
* Convert wstring to string, actualy it's not converting any data, it's just copy 
* wstring array to string array. (possible bad character reproduction)
*/
std::string Conv::WstringToString(std::wstring Text)
{
	std::string Return;

	for(unsigned int i = 0; i < Text.length(); i++)
		Return += (char)Text[i];

	return Return;
}

/**
* Convert MSB and LSB to number.
* @param Msb Most significant byte
* @param Lsb Least significant byte
*/
int Conv::MsbLsbToInt(unsigned char Msb, unsigned char Lsb)
{
	return ((Msb << 8) | Lsb);
}

/**
* Convert MSB and LSB to number.
* @param Msb Most significant byte
* @param Lsb Least significant byte
*/
int Conv::MsbLsbToInt(std::vector<unsigned char> MsbLsb)
{
	std::string TR = Conv::BinToAscii(MsbLsb, false);

	if(MsbLsb.size() >= 2)
		return ((MsbLsb[0] << 8) | MsbLsb[1]);
	else
		return 0;
}

/**
* Convert integer number to MSB and LSB.
* @param Number - number to convert
* @return vector contains MSB and LSB of gived number
*/
std::vector<unsigned char> Conv::IntToMsbLsb(int Number)
{
	std::vector<unsigned char> Return;

	if(Number < 0)
	{
		Return.push_back(0x00);
		Return.push_back(0x00);
	}
	else
	{
		Return.push_back((Number >> 8) & 0xff);
		Return.push_back(Number & 0xff);
	}

	return Return;
}

/**
* Copy char array to string. Char array must have last char equal '\0'
*/
std::string Conv::CharArrayToString(char *CharArray)
{
	int i = 0;
	std::string Return;

	if(CharArray == NULL) return Return;

	for(;;)
	{
    if(CharArray[i] == '\0') break;
		Return += CharArray[i];
		i++;
	}

	return Return;
}

/**
* Just copy all characters from wchar_t array to string.
* There is no Conv for special characters.
*/
std::string Conv::WcharToString(wchar_t *WcharArray)
{
	int i = 0;
	std::string Return;

	for(;;)
	{
    if(WcharArray[i] == '\0') break;
		Return += (char)WcharArray[i];
		i++;
	}
	return Return;
}

/**
* Convert unsigned long number to IP address.
*/
std::string Conv::UnsignedLongToIpAdress(unsigned long IpAdress)
{
	std::string Return;
	unsigned char a, b, c, d;  
  char Tmp[16];
    
	a = IpAdress & 0xFF;  
  b = (IpAdress >> 8) & 0xFF;
  c = (IpAdress >> 16) & 0xFF;
  d = (IpAdress >> 24) & 0xFF;

	sprintf(Tmp, "%u.%u.%u.%u", a, b, c, d); 

  Return = Tmp;
  
  return Return;
}

/**
* Convert 4 bytes data to unsigned integer.
*/
unsigned int Conv::BinToInt(std::vector<unsigned char> BinData)
{
	unsigned int Return;

	Return = BinData[3] | (BinData[2] << 8) | (BinData[1] << 16) | (BinData[0] << 24);

	return Return;
}

/**
* Convert 4 bytes data to unsigned integer
*/
unsigned int Conv::BinToInt(unsigned char *BinData)
{
	unsigned int Return;

	Return = BinData[3] | (BinData[2] << 8) | (BinData[1] << 16) | (BinData[0] << 24);

	return Return;
}

/**
* Convert 1 or 2 bytes of data to unsigned short integer.
*/
unsigned short Conv::BinToUShort(unsigned char *BinData, unsigned short Length)
{
	unsigned short Return;

	if(Length == 1)
		Return= BinData[0];
	else
		Return= BinData[1] | (BinData[0] << 8);

	return Return;
}

/**
* Convert integer to 4 bytes data
*/
std::vector<unsigned char> Conv::IntToBin(int Number)
{
	std::vector<unsigned char> ArrayOfByte(4);
	
	for(int i = 0; i < 4; i++)
		ArrayOfByte[3 - i] = (Number >> (i * 8));
	
	return ArrayOfByte;
}

/**
* Replace upper characters with thier lower equivalence. (only basic alfa characters from ASCII tabele is supported)
*/
std::string Conv::ToLower(std::string Text)
{
	char Upper[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '\0' };
	char Lower[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '\0' };
	int j = 0;
	
	while(Text[j] != '\0')
	{
		for(unsigned int i = 0; i < sizeof(Upper); i++)
		{
			if(Text[j] == Upper[i])
			{
				Text[j] = Lower[i];
				break;
			}	  
		}
		j++;
	}

	return Text;
}

/**
* Replace lower characters with thier upper equivalence. (only basic alfa characters from ASCII tabele is supported)
*/
std::string Conv::ToUpper(std::string Text)
{
  char Upper[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '\0' };
  char Lower[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '\0' };
  int j = 0;

  while(Text[j] != '\0')
  {
    for(unsigned int i = 0; i < sizeof(Upper); i++)
    {
      if(Text[j] == Lower[i])
      {
        Text[j] = Upper[i];
        break;
      }
    }
    j++;
  }

  return Text;
}
