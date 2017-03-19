#include "BerTlv.h"
#include "Global.h"
#include "Conv.h"

//Add tag and value to TLV collection
short BerTlv::Add(std::string Tag, std::vector<unsigned char> Value)
{
    std::vector<unsigned char> BinTmp;
    
    if(Tag.length() <= 0)
        return TAG_IS_EMPTY;
    
    if(Tag.length() % 2 != 0)
        return TAG_ERROR;
    
    if(Value.size() <= 0)
        return VALUE_IS_EMPTY;
    
    //Convert and add TAG to TLV
    BinTmp = Conv::AsciiToBin(Tag);
    if(BinTmp.size() <= 0)
        return TAG_ERROR;
    
    for(size_t i = 0; i < BinTmp.size(); i++)
        _TlvStruct.push_back(BinTmp[i]);
    
    //Calculate Value length and add it to TLV
    BinTmp.clear();
    BinTmp = CalcSizeOfValue(Value);
    if(BinTmp.size() <= 0)
        return VALUE_ERROR;
    
    for(size_t i = 0; i < BinTmp.size(); i++)
        _TlvStruct.push_back(BinTmp[i]);
    
    //Add value
    for(size_t i = 0; i < Value.size(); i++)
        _TlvStruct.push_back(Value[i]);
    
    return OK;
}

//Retrieve value of tag from TLV collection and save it to 'ValueOfTag'
short BerTlv::GetValue(std::string Tag, std::vector<unsigned char> *ValueOfTag, bool CheckNestedTags)
{
    std::vector<unsigned char> BinTag;
    int ValueSize = 0;
    short LengthSize = 0;
    size_t StartPosition = 0;
    int TagSize = 0;
    
    if(Tag.length() <= 0)
        return TAG_IS_EMPTY;
    
    if(Tag.length() % 2 != 0)
        return TAG_ERROR;
    
    BinTag = Conv::AsciiToBin(Tag);
    
    //Start looking for tag
    for(size_t i = 0; i < _TlvStruct.size(); i++)
    {
        ValueSize = 0;
        LengthSize = 0;
        StartPosition = 0;
        
        //One byte tag
        if(BinTag.size() == 1 && _TlvStruct[i] == BinTag[0])
        {
            if(_TlvStruct.size() < i + 1)
                return TLV_SIZE_ERROR;
            
            //Get size of value and size of length
            LengthSize = GetSizeOfValue(i + 1, &ValueSize);
            if(LengthSize == TLV_SIZE_ERROR)
                return TLV_SIZE_ERROR;
            
            //Calculate start position of value
            StartPosition = i + 1 + LengthSize;
        }
        //Two bytes tag
        else if(BinTag.size() == 2 && _TlvStruct[i] == BinTag[0] &&
                _TlvStruct.size() > i + 1 && _TlvStruct[i + 1] == BinTag[1])
        {
            if(_TlvStruct.size() < i + 2)
                return TLV_SIZE_ERROR;
            
            //Get size of value and size of length
            LengthSize = GetSizeOfValue(i + 2, &ValueSize);
            if(LengthSize == TLV_SIZE_ERROR)
                return TLV_SIZE_ERROR;
            
            //Calculate start position of value
            StartPosition = i + 2 + LengthSize;
        }
        //Three bytes tag
        else if(BinTag.size() == 3 && _TlvStruct[i] == BinTag[0] &&
                _TlvStruct.size() > i + 1 && _TlvStruct[i + 1] == BinTag[1] &&
                _TlvStruct.size() > i + 2 && _TlvStruct[i + 2] == BinTag[2])
        {
            if(_TlvStruct.size() < i + 3)
                return TLV_SIZE_ERROR;
            
            //Get size of value and size of length
            LengthSize = GetSizeOfValue(i + 3, &ValueSize);
            if(LengthSize == TLV_SIZE_ERROR)
                return TLV_SIZE_ERROR;
            
            //Calculate start position of value
            StartPosition = i + 3 + LengthSize;
        }
        //Four bytes long tag
        else if(BinTag.size() == 4 && _TlvStruct[i] == BinTag[0] &&
                _TlvStruct.size() > i + 1 && _TlvStruct[i + 1] == BinTag[1] &&
                _TlvStruct.size() > i + 2 && _TlvStruct[i + 2] == BinTag[2] &&
                _TlvStruct.size() > i + 3 && _TlvStruct[i + 3] == BinTag[3])

        {
            if(_TlvStruct.size() < i + 4)
                return TLV_SIZE_ERROR;
            
            //Get size of value and size of length
            LengthSize = GetSizeOfValue(i + 4, &ValueSize);
            if(LengthSize == TLV_SIZE_ERROR)
                return TLV_SIZE_ERROR;
            
            //Calculate start position of value
            StartPosition = i + 4 + LengthSize;
        }
        
        //Tag not found?
        if(StartPosition <= 0)
        {
            TagSize = GetTagLength(i);
            
            //Get size of value and size of length
            LengthSize = GetSizeOfValue(i, &ValueSize);
            if(LengthSize == TLV_SIZE_ERROR)
                return TLV_SIZE_ERROR;
            
            if(CheckNestedTags && IsTagNested(i, TagSize))
            {
                if(GetValueFromTlvInsideTag(i, TagSize + LengthSize + ValueSize, BinTag, ValueOfTag) == OK)
                    return OK;
            }
               
            //Increment i + size of tag
            i += TagSize;
            if(_TlvStruct.size() < i)
                return TLV_SIZE_ERROR;
            
            //Increment i + size of value
            i += LengthSize + ValueSize - 1;
        }
        //We found tag so retrieve and return value
        else
        {
            if(StartPosition + ValueSize > _TlvStruct.size())
                return TLV_SIZE_ERROR;
            
            for(size_t j = StartPosition; j < StartPosition + ValueSize; j++)
                ValueOfTag->push_back(_TlvStruct[j]);
            
            return OK;
        }
    } //For
    
    return FAILED;
}

//Dump all data from TLV collection and save it to 'Output'
short BerTlv::DumpAllTagsAndValues(std::string *Output, bool ParseNestedTags)
{
    short TagSize = 0;
    int ValueSize = 0;
    short LengthSize = 0;
    size_t StartPosition = 0;
    size_t OutputActualPosition = 0;
    
    //Traverse through colection
    for(size_t i = 0; i < _TlvStruct.size(); i++)
    {
        StartPosition = i;
        
        //Get and increment i + size of tag
        TagSize = GetTagLength(i);
        i += TagSize;
        if(_TlvStruct.size() < i)
            return TLV_SIZE_ERROR;
        
        //Get size of value and size of length
        LengthSize = GetSizeOfValue(i, &ValueSize);
        if(LengthSize == TLV_SIZE_ERROR)
            return TLV_SIZE_ERROR;
        
        //Increment i + size of value
        i += LengthSize + ValueSize - 1;
        
        //Parse tags inside value?
        if(ParseNestedTags && IsTagNested(StartPosition, TagSize))
        {
            *Output += Conv::BinToAscii(&_TlvStruct[StartPosition], TagSize, false);
            *Output += "[";
            *Output += Conv::BinToAscii(&_TlvStruct[StartPosition + TagSize], LengthSize, false);
            *Output += "]";
            OutputActualPosition = Output->length();
            
            if(DumpTlvInsideTag(StartPosition + TagSize + LengthSize, ValueSize, Output) == OK)
            {
                Output->insert(OutputActualPosition, "\n{\n");
                *Output += "}";
            }
            else
            {
                *Output += " = ";
                *Output += Conv::BinToAscii(&_TlvStruct[StartPosition + TagSize + LengthSize], ValueSize, false);
            }
            
            *Output += "\n";
        }
        //Just parse value of tag
        else
        {
            *Output += Conv::BinToAscii(&_TlvStruct[StartPosition], TagSize, false);
            *Output += "[";
            *Output += Conv::BinToAscii(&_TlvStruct[StartPosition + TagSize], LengthSize, false);
            *Output += "] = ";
            *Output += Conv::BinToAscii(&_TlvStruct[StartPosition + TagSize + LengthSize], ValueSize, false);
            *Output += "\n";
        }
    } //For
    
    return OK;
}

short BerTlv::DumpTlvInsideTag(size_t StartPosition, int Length, std::string *Output)
{
    short TagSize = 0;
    int ValueSize = 0;
    short LengthSize = 0;
    size_t MaxSize = StartPosition + Length;
    size_t TmpStartPosition = 0;
    std::string TmpOutput = "";
    
    //Traverse through colection
    for(size_t i = StartPosition; i < MaxSize; i++)
    {
        TmpStartPosition = i;
        
        //Get and increment i + size of tag
        TagSize = GetTagLength(i);
        i += TagSize;
        if(MaxSize < i)
            return TLV_SIZE_ERROR;
        
        //Get size of value and size of length
        LengthSize = GetSizeOfValue(i, &ValueSize);
        if(LengthSize == TLV_SIZE_ERROR || (ValueSize + StartPosition + TagSize + LengthSize) > MaxSize)
            return TLV_SIZE_ERROR;
        
        //Increment i + size of value
        i += LengthSize + ValueSize - 1;
        
        TmpOutput += "  ";
        TmpOutput += Conv::BinToAscii(&_TlvStruct[TmpStartPosition], TagSize, false);
        TmpOutput += "[";
        TmpOutput += Conv::BinToAscii(&_TlvStruct[TmpStartPosition + TagSize], LengthSize, false);
        TmpOutput += "] = ";
        TmpOutput += Conv::BinToAscii(&_TlvStruct[TmpStartPosition + TagSize + LengthSize], ValueSize, false);
        TmpOutput += "\n";
    }
    
    *Output += TmpOutput;
    
    return OK;
}

short BerTlv::GetValueFromTlvInsideTag(size_t StartPosition, int Length, std::vector<unsigned char> Tag, std::vector<unsigned char> *Output)
{
    short TagSize = 0;
    int ValueSize = 0;
    short LengthSize = 0;
    size_t MaxSize = StartPosition + Length;
    std::vector<unsigned char> TmpTag;
    
    //Traverse through colection
    for(size_t i = StartPosition; i < MaxSize; i++)
    {
        TmpTag.clear();
        TagSize = GetTagLength(i);
        
        //Check if we found tag which we are looking for
        if(TagSize == Tag.size())
        {
            for(size_t j = i; j < i + TagSize; i++)
                TmpTag.push_back(_TlvStruct[j]);
            
            if(!std::equal(Tag.begin(), Tag.end(), TmpTag.begin()))
                continue;
        }
        else
            continue;
        
        //We are here so it means we have corect tag
        
        //Increment i + size of tag
        i += TagSize;
        if(MaxSize < i)
            return TLV_SIZE_ERROR;
        
        //Get size of value and size of length
        LengthSize = GetSizeOfValue(i, &ValueSize);
        if(LengthSize == TLV_SIZE_ERROR || (ValueSize + StartPosition + TagSize + LengthSize) > MaxSize)
            return TLV_SIZE_ERROR;
        
        //Copy value of tag to output
        for(size_t j = i + LengthSize; j < i + LengthSize + ValueSize; j++)
            Output->push_back(_TlvStruct[j]);

        return OK;
    }
    
    return FAILED;
}

//Check if the tag from TLV collection is in _NestedTags vector
bool BerTlv::IsTagNested(size_t StartPosition, short TagSize)
{
    std::vector<unsigned char> Tag;
    std::vector<unsigned char> NestedTag;
    
    for(size_t i = StartPosition; i < StartPosition + TagSize; i++)
        Tag.push_back(_TlvStruct[i]);
    
    for(size_t i = 0; i < _NestedTags.size(); i++)
    {
        if(Tag.size() == (_NestedTags[i].length() / 2))
        {
            NestedTag = Conv::AsciiToBin(_NestedTags[i]);
            
            if(std::equal(Tag.begin(), Tag.end(), NestedTag.begin()))
                return true;
        }
    }
    
    return false;
}

void BerTlv::SetTwoBytesTags(std::vector<std::string> Tags)
{
    for(size_t i = 0; i < Tags.size(); i++)
    {
        //Check if the input tag can be converted to 1 byte
        if(Tags[i].length() == 2 &&
           Tags[i][0] >= '0' && Tags[i][0] <= 'F' && Tags[i][1] >= '0' && Tags[i][1] <= 'F')
        {
            _TwoBytesTags.push_back(Conv::AsciiToBin(Tags[i])[0]);
        }
    }
}

void BerTlv::SetThreeBytesTags(std::vector<std::string> Tags)
{
    for(size_t i = 0; i < Tags.size(); i++)
    {
        //Check if the input tag can be converted to 2 bytes
        if(Tags[i].length() == 4 &&
           Tags[i][0] >= '0' && Tags[i][0] <= 'F' && Tags[i][1] >= '0' && Tags[i][1] <= 'F' &&
           Tags[i][2] >= '0' && Tags[i][2] <= 'F' && Tags[i][3] >= '0' && Tags[i][3] <= 'F')
        {
            _ThreeBytesTags.push_back(Conv::AsciiToBin(Tags[i])[0]);
            _ThreeBytesTags.push_back(Conv::AsciiToBin(Tags[i])[1]);
        }
    }
}

void BerTlv::SetFourBytesTags(std::vector<std::string> Tags)
{
    for(size_t i = 0; i < Tags.size(); i++)
    {
        //Check if the input tag can be converted to 3 bytes
        if(Tags[i].length() == 6 &&
           Tags[i][0] >= '0' && Tags[i][0] <= 'F' && Tags[i][1] >= '0' && Tags[i][1] <= 'F' &&
           Tags[i][2] >= '0' && Tags[i][2] <= 'F' && Tags[i][3] >= '0' && Tags[i][3] <= 'F' &&
           Tags[i][4] >= '0' && Tags[i][2] <= 'F' && Tags[i][5] >= '0' && Tags[i][3] <= 'F')
        {
            _FourBytesTags.push_back(Conv::AsciiToBin(Tags[i])[0]);
            _FourBytesTags.push_back(Conv::AsciiToBin(Tags[i])[1]);
            _FourBytesTags.push_back(Conv::AsciiToBin(Tags[i])[1]);
        }
    }
}

short BerTlv::GetSizeOfValue(size_t StartPosition, int *SizeOfValue)
{
    //Five bytes long size
    if(_TlvStruct[StartPosition] == 0x84)
    {
        if(_TlvStruct.size() >= StartPosition + 4)
        {
            *SizeOfValue = (_TlvStruct[StartPosition + 1] << 24) | (_TlvStruct[StartPosition + 2] << 16) | (_TlvStruct[StartPosition + 3] << 8) | _TlvStruct[StartPosition + 4];
            
            return 5;
        }
        else
            return TLV_SIZE_ERROR;
    }
    //Four bytes long size
    else if(_TlvStruct[StartPosition] == 0x83)
    {
        if(_TlvStruct.size() >= StartPosition + 3)
        {
            *SizeOfValue = (_TlvStruct[StartPosition + 1] << 16) | (_TlvStruct[StartPosition + 2] << 8) | _TlvStruct[StartPosition + 3];
            
            return 4;
        }
        else
            return TLV_SIZE_ERROR;
    }
    //Three bytes long size
    else if(_TlvStruct[StartPosition] == 0x82)
    {
        if(_TlvStruct.size() >= StartPosition + 2)
        {
            *SizeOfValue = (_TlvStruct[StartPosition + 1] << 8) | _TlvStruct[StartPosition + 2];
            
            return 3;
        }
        else
            return TLV_SIZE_ERROR;
    }
    //Two bytes long size
    else if(_TlvStruct[StartPosition] == 0x81)
    {
        if(_TlvStruct.size() >= StartPosition + 1)
        {
            *SizeOfValue = _TlvStruct[StartPosition + 1];
            
            return 2;
        }
        else
            return TLV_SIZE_ERROR;
    }
    //One byte long size
    else
    {
        *SizeOfValue = _TlvStruct[StartPosition];
        
        return 1;
    }
}

short BerTlv::GetTagLength(size_t StartPosition)
{
    for(size_t i = 0; i < _FourBytesTags.size(); i += 3)
    {
        if(_TlvStruct[StartPosition] == _FourBytesTags[i] &&
           _TlvStruct.size() >= StartPosition + 1 && _FourBytesTags.size() >= i + 1 &&
           _TlvStruct[StartPosition + 1] == _FourBytesTags[i + 1] &&
           _TlvStruct.size() >= StartPosition + 2 && _FourBytesTags.size() >= i + 2 &&
           _TlvStruct[StartPosition + 2] == _FourBytesTags[i + 2])
        {
            return 4;
        }
    }
    
    for(size_t i = 0; i < _ThreeBytesTags.size(); i += 2)
    {
        if(_TlvStruct[StartPosition] == _ThreeBytesTags[i] &&
           _TlvStruct.size() >= StartPosition + 1 && _ThreeBytesTags.size() >= i + 1 &&
           _TlvStruct[StartPosition + 1] == _ThreeBytesTags[i + 1])
        {
            return 3;
        }
    }
    
    for(size_t i = 0; i < _TwoBytesTags.size(); i++)
    {
        if(_TlvStruct[StartPosition] == _TwoBytesTags[i])
        {
            return 2;
        }
    }
    
    return 1;
}

std::vector<unsigned char> BerTlv::CalcSizeOfValue(std::vector<unsigned char> Value)
{
    std::vector<unsigned char> Return = std::vector<unsigned char>(0);
    
    //One byte long size
    if(Value.size() > 0 && Value.size() <= 127)
    {
        Return.push_back(Value.size());
    }
    //Two bytes long size
    else if(Value.size() > 127 && Value.size() <= 256)
    {
        Return.push_back(0x81);
        Return.push_back(Value.size());
    }
    //Three bytes long size
    else if(Value.size() > 256 && Value.size() <= 65535)
    {
        Return.push_back(0x82);
        Return.push_back((Value.size() >> 8) & 0xff);
        Return.push_back(Value.size() & 0xff);
    }
    //Four bytes long size
    else if(Value.size() > 65535 && Value.size() <= 16777215)
    {
        Return.push_back(0x83);
        Return.push_back((Value.size() >> 16) & 0xff);
        Return.push_back((Value.size() >> 8) & 0xff);
        Return.push_back(Value.size() & 0xff);
    }
    //Five bytes long size
    else if(Value.size() > 16777215 && Value.size() <= 4294967259)
    {
        Return.push_back(0x84);
        Return.push_back((Value.size() >> 24) & 0xff);
        Return.push_back((Value.size() >> 16) & 0xff);
        Return.push_back((Value.size() >> 8) & 0xff);
        Return.push_back(Value.size() & 0xff);
    }
    
    return Return;
}
