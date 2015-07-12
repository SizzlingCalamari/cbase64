
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
    const unsigned int TEST_SIZE_MAX = 1 << 10;
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

int OverwriteTest()
{
    const unsigned int TEST_SIZE_MAX = 1 << 10;
    int ret = 0;

    for (unsigned int i = 0; i < TEST_SIZE_MAX; ++i)
    {
        unsigned char* decodedString = (unsigned char*)malloc(i + 4);
        memset(decodedString + i, 0xfd, 4);

        const unsigned int encodedLength = cbase64_calc_encoded_length(i);
        char* encodedString = (char*)malloc(encodedLength);

        {
            char* encodeOut = encodedString;
            cbase64_encodestate encodeState;
            cbase64_init_encodestate(&encodeState);

            encodeOut += cbase64_encode_block(decodedString, i, encodeOut, &encodeState);
            encodeOut += cbase64_encode_blockend(encodeOut, &encodeState);
        }
        {
            cbase64_decodestate decodeState;
            cbase64_init_decodestate(&decodeState);
            cbase64_decode_block(encodedString, encodedLength, decodedString, &decodeState);
        }

        if (*(decodedString + i + 0) != 0xfd ||
            *(decodedString + i + 1) != 0xfd ||
            *(decodedString + i + 2) != 0xfd ||
            *(decodedString + i + 3) != 0xfd)
        {
            ret = -1;
        }

        free((void*)encodedString);
        free((void*)decodedString);
        
        if (ret == -1)
        {
            break;
        }
    }
    
    return ret;
}

int DecodeEncodeTest()
{
    int ret = 0;

    const char* encoded = "Awg=";
    unsigned int encodedLength = 4;

    unsigned char decoded[4];
    unsigned int decodedlength;

    char encoded2[5];
    unsigned int encodedLength2;

    {
        cbase64_decodestate decodeState;
        cbase64_init_decodestate(&decodeState);
        decodedlength = cbase64_decode_block(encoded, encodedLength, decoded, &decodeState);
    }
    {
        cbase64_encodestate encodeState;
        cbase64_init_encodestate(&encodeState);

        char* encodeOut = encoded2;
        encodeOut += cbase64_encode_block(decoded, decodedlength, encodeOut, &encodeState);
        encodeOut += cbase64_encode_blockend(encodeOut, &encodeState);
        encodedLength2 = (encodeOut - encoded2);
        encoded2[4] = '\0';
    }
    if (encodedLength != encodedLength2 || 0 != strcmp(encoded, encoded2))
    {
        ret = -1;
    }
    return ret;
}

int RFC4648Test()
{
    static const char* unencodedData[] =
    {
        "", "f", "fo", "foo", "foob", "fooba", "foobar"
    };
    static const char* encodedData[] =
    {
        "", "Zg==", "Zm8=", "Zm9v", "Zm9vYg==", "Zm9vYmE=", "Zm9vYmFy"
    };

    static const int numTests = sizeof(unencodedData) / sizeof(*unencodedData);

    int ret = 0;
    for (int i = 0; i < numTests; ++i)
    {
        const unsigned int unencodedLengthActual = strlen(unencodedData[i]);
        const unsigned int encodedLengthActual = strlen(encodedData[i]);
        const unsigned char* unencodedActual = (const unsigned char*)unencodedData[i];
        const char* encodedActual = encodedData[i];

        unsigned int unencodedLength;
        unsigned int encodedLength;
        unsigned char* unencoded = (unsigned char*)malloc(unencodedLengthActual);
        char* encoded = (char*)malloc(encodedLengthActual);

        {
            cbase64_encodestate encodeState;
            cbase64_init_encodestate(&encodeState);

            char* encodeOut = encoded;
            encodeOut += cbase64_encode_block(unencodedActual, unencodedLengthActual, encodeOut, &encodeState);
            encodeOut += cbase64_encode_blockend(encodeOut, &encodeState);
            encodedLength = (encodeOut - encoded);
        }
        {
            cbase64_decodestate decodeState;
            cbase64_init_decodestate(&decodeState);
            unencodedLength = cbase64_decode_block(encoded, encodedLength, unencoded, &decodeState);
        }
        if (unencodedLength != unencodedLengthActual ||
            encodedLength != encodedLengthActual ||
            0 != memcmp(unencoded, unencodedActual, unencodedLengthActual) ||
            0 != memcmp(encoded, encodedActual, encodedLengthActual))
        {
            ret = -1;
        }
        free(encoded);
        free(unencoded);
        if (ret != 0)
        {
            return ret;
        }
    }
    return ret;
}

int BitEncDecTest()
{
    static const char* encodedData = "awuDm+OCY/sSCyO7C6Mrk/uCk3v7srtxEZuDA3BQzuTC4NDmuODYvsTCyO7C6MrkvuDk3r7sblzC0twAAA==";
    static const int unencodedBits = 482;
    static const int unencodedBytes = 61;
    const int encodedBytes = strlen(encodedData);

    unsigned char* unencodedData = (unsigned char*)malloc(unencodedBytes);
    char* reencodedData = (char*)malloc(1 + encodedBytes);
    int reencodedLength = 0;

    {
        cbase64_decodestate decodeState;
        cbase64_init_decodestate(&decodeState);
        cbase64_decode_block(encodedData, encodedBytes, unencodedData, &decodeState);
    }
    unencodedData[unencodedBytes - 1] &= ~(0xFF >> (unencodedBits & 7));
    {
        cbase64_encodestate encodeState;
        cbase64_init_encodestate(&encodeState);

        char* encodeOut = reencodedData;
        encodeOut += cbase64_encode_block(unencodedData, unencodedBytes, encodeOut, &encodeState);
        encodeOut += cbase64_encode_blockend(encodeOut, &encodeState);
        reencodedLength = (encodeOut - reencodedData);
        *encodeOut = '\0';
    }
    int ret = 0;
    if (strcmp(reencodedData, encodedData))
    {
        ret = -1;
    }

    free(reencodedData);
    free(unencodedData);
    return ret;
}

int PartialTest()
{
    static const char* unencodedData[] =
    {
        "", "f", "fo", "foo", "foob", "fooba", "foobar"
    };
    static const char* encodedData[] =
    {
        "", "Zg==", "Zm8=", "Zm9v", "Zm9vYg==", "Zm9vYmE=", "Zm9vYmFy"
    };

    static const int numTests = sizeof(unencodedData) / sizeof(*unencodedData);

    int ret = 0;
    for (int i = 0; i < numTests; ++i)
    {
        const unsigned int unencodedLengthActual = strlen(unencodedData[i]);
        const unsigned int encodedLengthActual = strlen(encodedData[i]);
        const unsigned char* unencodedActual = (const unsigned char*)unencodedData[i];
        const char* encodedActual = encodedData[i];

        unsigned int unencodedLength = 0;
        unsigned int encodedLength = 0;
        unsigned char* unencoded = (unsigned char*)malloc(unencodedLengthActual);
        char* encoded = (char*)malloc(encodedLengthActual);

        {
            cbase64_encodestate encodeState;
            cbase64_init_encodestate(&encodeState);

            char* encodeOut = encoded;
            for (int j = 0; j < unencodedLengthActual; ++j)
            {
                encodeOut += cbase64_encode_block(unencodedActual + j, 1, encodeOut, &encodeState);
            }
            encodeOut += cbase64_encode_blockend(encodeOut, &encodeState);
            encodedLength = (encodeOut - encoded);
        }
        {
            cbase64_decodestate decodeState;
            cbase64_init_decodestate(&decodeState);
            char* unencodeOut = unencoded;
            for (int j = 0; j < encodedLength; ++j)
            {
                unencodeOut += cbase64_decode_block(encoded + j, 1, unencodeOut, &decodeState);
            }
            unencodedLength = (unencodeOut - unencoded);
        }
        if (unencodedLength != unencodedLengthActual ||
            encodedLength != encodedLengthActual ||
            0 != memcmp(unencoded, unencodedActual, unencodedLengthActual) ||
            0 != memcmp(encoded, encodedActual, encodedLengthActual))
        {
            ret = -1;
        }
        free(encoded);
        free(unencoded);
        if (ret != 0)
        {
            return ret;
        }
    }
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
    if (0 == OverwriteTest())
    {
        printf("OverwriteTest passed\n");
    }
    else
    {
        printf("OverwriteTest failed\n");
    }
    if (0 == DecodeEncodeTest())
    {
        printf("DecodeEncodeTest passed\n");
    }
    else
    {
        printf("DecodeEncodeTest failed\n");
    }
    if (0 == RFC4648Test())
    {
        printf("RFC4648Test passed\n");
    }
    else
    {
        printf("RFC4648Test failed\n");
    }
    if (0 == BitEncDecTest())
    {
        printf("BitEncDecTest passed\n");
    }
    else
    {
        printf("BitEncDecTest failed\n");
    }
    if (0 == PartialTest())
    {
        printf("PartialTest passed\n");
    }
    else
    {
        printf("PartialTest failed\n");
    }
    return 0;
}
