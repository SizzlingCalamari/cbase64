
#define CBASE64_IMPLEMENTATION
#include <cbase64/cbase64.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static const char* g_TestString = \
    "voodoo god A.I. dome skyscraper sub-orbital face forwards construct pen camera " \
    "table narrative vehicle vehicle franchise cartel. savant augmented reality sign " \
    "j-pop knife ablative weathered refrigerator boat tattoo assassin systemic fetishism " \
    "car free-market. lights convenience store dome vehicle drone order-flow pistol " \
    "free-market kanji courier network assault courier boat systema. monofilament " \
    "decay car modem rifle lights face forwards monofilament office soul-delay city " \
    "otaku alcohol long-chain hydrocarbons hacker. refrigerator city tank-traps " \
    "augmented reality girl singularity rain towards dead futurity hotdog cardboard " \
    "plastic drone franchise. -ware monofilament boy convenience store grenade " \
    "monofilament marketing numinous warehouse euro-pop disposable sunglasses shrine "
    "pistol A.I..Kowloon j-pop alcohol alcohol boy industrial grade knife semiotics " \
    "silent kanji city stimulate soul-delay render-farm Kowloon. computer euro-pop " \
    "nano- cyber- nano- bridge disposable meta- sign network nodal point physical " \
    "augmented reality advert lights. claymore mine dead katana into bridge towards math- " \
    "media Tokyo -ware spook dolphin Kowloon sprawl dead. decay media spook artisanal " \
    "skyscraper physical math- semiotics ablative gang human DIY savant sprawl dead. " \
    "neon corrupted realism tanto San Francisco fetishism dead augmented reality human " \
    "dome cartel tank-traps faded Tokyo sensory. systemic -ware systema tower franchise " \
    "city refrigerator artisanal sub-orbital corporation market rebar smart- drugs woman." \
    "modem San Francisco office denim crypto- Shibuya sprawl human geodesic tower weathered " \
    "Tokyo systema faded artisanal. neon spook camera Tokyo boat weathered soul-delay " \
    "katana city military-grade sentient market wristwatch bicycle voodoo god. wonton " \
    "soup neon tiger-team A.I. tank-traps tiger-team boy receding refrigerator corrupted " \
    "marketing gang woman Tokyo tattoo. engine film -space modem smart- marketing face " \
    "forwards soul-delay Shibuya paranoid DIY military-grade vehicle nodality dolphin. " \
    "girl singularity franchise sprawl euro-pop film chrome fluidity soul-delay advert " \
    "render-farm narrative katana range-rover 3D-printed. plastic narrative cartel " \
    "geodesic j-pop BASE jump drugs chrome dolphin math- numinous realism nodal point " \
    "youtube shanty town.sentient j-pop military-grade sub-orbital fetishism tiger-team " \
    "urban denim soul-delay tiger-team office meta- towards euro-pop systemic. kanji " \
    "fluidity bridge grenade dead numinous augmented reality spook office denim concrete " \
    "corporation otaku gang voodoo god. physical towards pen chrome rebar -ware into " \
    "camera lights nodality paranoid boy geodesic Shibuya corrupted. construct paranoid " \
    "city film math- tower refrigerator lights wristwatch network advert into shrine " \
    "post- denim. lights neural voodoo god faded vehicle sentient papier-mache saturation " \
    "point range-rover uplink man futurity narrative faded artisanal. weathered tube bomb " \
    "j-pop spook range-rover fetishism gang vinyl neural A.I. papier-mache wonton soup sub-orbital car.";

char* EncodeData(const unsigned char* data_in, unsigned int length_in, unsigned int* length_out)
{
    const unsigned int encodedLength = cbase64_calc_encoded_length(length_in);
    char* codeOut = (char*)malloc(encodedLength);
    char* codeOutEnd = codeOut;

    cbase64_encodestate encodeState;
    cbase64_init_encodestate(&encodeState);
    codeOutEnd += cbase64_encode_block(data_in, length_in, codeOutEnd, &encodeState);
    codeOutEnd += cbase64_encode_blockend(codeOutEnd, &encodeState);

    *length_out = (codeOutEnd - codeOut);
    return codeOut;
}

unsigned char* DecodeData(const char* code_in, unsigned int length_in, unsigned int* length_out)
{
    const unsigned int decodedLength = cbase64_calc_decoded_length(code_in, length_in);
    unsigned char* dataOut = (unsigned char*)malloc(decodedLength);

    cbase64_decodestate decodeState;
    cbase64_init_decodestate(&decodeState);
    *length_out = cbase64_decode_block(code_in, length_in, dataOut, &decodeState);
    return dataOut;
}

int EncodeDecodeTest(const char* string)
{
    const char* data = string;
    const unsigned int dataLength = 1 + strlen(string);

    unsigned int encodedLength;
    const char* encodedData = EncodeData((const unsigned char*)data, dataLength, &encodedLength);

    unsigned int decodedLength;
    const char* decodedData = (const char*)DecodeData(encodedData, encodedLength, &decodedLength);

    const int result = strcmp(string, decodedData);

    free((void*)encodedData);
    free((void*)decodedData);
    return result;
}

int LengthEstimateTest()
{
    const unsigned int TEST_SIZE_MAX = 65535;
    int ret = 0;

    const unsigned char* string = (const unsigned char*)malloc(TEST_SIZE_MAX);
    char* encodedString = (char*)malloc(TEST_SIZE_MAX*2);
    for (unsigned int i = 0; i < TEST_SIZE_MAX; ++i)
    {
        const unsigned int predictedEncodedLength = cbase64_calc_encoded_length(i);
        char* codeOutEnd = encodedString;

        cbase64_encodestate encodeState;
        cbase64_init_encodestate(&encodeState);
        codeOutEnd += cbase64_encode_block(string, i, codeOutEnd, &encodeState);
        codeOutEnd += cbase64_encode_blockend(codeOutEnd, &encodeState);
        const unsigned int actualEncodedLength = (codeOutEnd - encodedString);

        if ((predictedEncodedLength != actualEncodedLength) ||
            (i != cbase64_calc_decoded_length(encodedString, actualEncodedLength)))
        {
            ret = -1;
            break;
        }
    }
    free((void*)string);
    free((void*)encodedString);
    return ret;
}

int main()
{
    if (0 == EncodeDecodeTest(g_TestString))
    {
        printf("EncodeDecodeTest passed\n");
    }
    else
    {
        printf("EncodeDecodeTest failed\n");
    }
    if (0 == LengthEstimateTest())
    {
        printf("LengthEstimateTest passed\n");
    }
    else
    {
        printf("LengthEstimateTest failed\n");
    }
    return 0;
}
