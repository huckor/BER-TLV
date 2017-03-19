#ifndef BerTlv_h
#define BerTlv_h

#include <vector>
#include <string>

class BerTlv
{
public:
    BerTlv() { _TlvStruct = std::vector<unsigned char>(0); }
    ~BerTlv() { _TlvStruct.clear(); }
    std::vector<unsigned char> GetTlv() { return _TlvStruct; }
    void SetTlv(std::vector<unsigned char> TlvStruct) { _TlvStruct = TlvStruct; }
    short Add(std::string Tag, std::vector<unsigned char> Value);
    short GetValue(std::string Tag, std::vector<unsigned char> *ValueOfTag, bool CheckNestedTags);
    void SetTwoBytesTags(std::vector<std::string> Tags);
    void SetThreeBytesTags(std::vector<std::string> Tags);
    void SetFourBytesTags(std::vector<std::string> Tags);
    void SetNestedTags(std::vector<std::string> Tags) { _NestedTags = Tags; }
    short DumpAllTagsAndValues(std::string *Output, bool ParseNestedTags);
    
private:
    std::vector<unsigned char> _TlvStruct;
    std::vector<unsigned char> _TwoBytesTags;
    std::vector<unsigned char> _ThreeBytesTags;
    std::vector<unsigned char> _FourBytesTags;
    std::vector<std::string> _NestedTags;
    
    std::vector<unsigned char> CalcSizeOfValue(std::vector<unsigned char> Value);
    short GetSizeOfValue(size_t StartPosition, int *SizeOfValue);
    short GetTagLength(size_t StartPosition);
    short DumpTlvInsideTag(size_t StartPosition, int Length, std::string *Output);
    bool IsTagNested(size_t StartPosition, short TagSize);
    short GetValueFromTlvInsideTag(size_t StartPosition, int Length, std::vector<unsigned char> Tag, std::vector<unsigned char> *Output);
};

#endif /* BerTlv_h */
