#include <iostream>
#include "BerTlv.h"
#include "Conv.h"
#include "Global.h"

//Add to vector starting byte of 2 bytes long tags
std::vector<std::string> GetTwoBytesTags()
{
    std::vector<std::string> Return;
    
    Return.push_back("9F");
    Return.push_back("5F");
    Return.push_back("BF");
    Return.push_back("7F");
    Return.push_back("DF");
    Return.push_back("FF");
    
    return Return;
}

//Add to vector first and second byte of 3 bytes long tags
std::vector<std::string> GetThreeBytesTags()
{
    std::vector<std::string> Return;
    
    Return.push_back("DF81");
    Return.push_back("FF81");
    
    return Return;
}

//Add to vector first and second and third byte of 4 bytes long tags
std::vector<std::string> GetFourBytesTags()
{
    std::vector<std::string> Return;
    
    Return.push_back("DF8281");
    Return.push_back("FF8281");
    
    return Return;
}

int main(int argc, const char * argv[])
{
    BerTlv Tlv;
    std::vector<unsigned char> Value;
    std::string Tag;
    std::string Output;
    
    //Setup TLV engine - THIS IS ALFA & OMEGA because withot knowing of which tags
    //are 2, 3 and 4 bytes long this engine won't working
    Tlv.SetTwoBytesTags(GetTwoBytesTags());
    Tlv.SetThreeBytesTags(GetThreeBytesTags());
    Tlv.SetFourBytesTags(GetFourBytesTags());
    
    //Example how to add tags and values to TLV
    Tlv.Add("9F40", Conv::AsciiToBin("31323334353637"));
    Tlv.Add("FF30", Conv::AsciiToBin("31323334353637"));
    Tlv.Add("1F", Conv::AsciiToBin("31323334353637"));
    Tlv.Add("81", Conv::AsciiToBin("31323334353637"));
    Tlv.Add("5A", Conv::AsciiToBin("31323334353637"));
    Tlv.Add("DF8126", Conv::AsciiToBin("31323334353637"));
    
    //Retrieval of created TLV collection
    std::cout << "Created TLV struct:\n";
    std::cout << Conv::BinToAscii(Tlv.GetTlv());
    
    //Clear TLV collection
    Tlv.GetTlv().clear();
    
    //Add example TLV collection
    Tlv.SetTlv(Conv::AsciiToBin("9f1a0207649f02060000000010005f2a0207649a031612209c0100950500000000009f37040095055b820220009f2608ab07b02018d87dc89f2701409f100706010a039000009f360200069f6604320040009f03060000000000009f34031f00009f45009f4c0200065a0847617390010100105f3401045f2403221231500b564953412043524544495457114761739001010010d2212201191894441f56009b0200005f201741445654205156534443205445535420434152442030345f28009f07009f0d009f0e009f0f009f51009f3901079f33030068c01f1901089f42008407a00000000310109f5d009f6c0210009f74009f12104361727465204465204372656469746f5f2d009f0607a00000000310105f25009f21031714228f01929f1e082d3433332d313930"));
    
    std::cout << "\n\nTLV struct:\n";
    std::cout << Conv::BinToAscii(Tlv.GetTlv());
    
    //Retrieve value of tag
    Tag = "FF8105";
    if(Tlv.GetValue(Tag, &Value) == OK)
    {
        std::cout << "\n\nValue of tag: " + Tag + "\n";
        std::cout << Conv::BinToAscii(Value);
    }
    
    std::cout << "\n\n";
    
    //Dump from TLV collection
    Tlv.DumpAllTagsAndValues(&Output);
    std::cout << Output;
    
    return 0;
}
