#include <ccxx/ccxx.h>



using namespace std;





//0x00XX0000
string f_datatypes[][2] = {
    { "int8"            , "char value;"                       },
    { "int16"           , "short value;"                      },
    { "int32"           , "int value;"                        },
    { "int64"           , "long long value;"                  },
    { "intText1"        , "char value[2];"                    },
    { "intText3"        , "char value[4];"                    },
    { "intText7"        , "char value[8];"                    },
    { "intText15"       , "char value[16];"                   },
    { "intText31"       , "char value[32];"                   },
    { "intText63"       , "char value[64];"                   },
    { "res"             , "long long value;"                  },
    { "res"             , "long long value;"                  },
    { "res"             , "long long value;"                  },
    { "res"             , "long long value;"                  },
    { "res"             , "long long value;"                  },
    { "res"             , "long long value;"                  },
    { "floatInt"        , "int32 value;"                      },
    { "floatIntConfig"  , "int32 value;"                      },
    { "floatRatio"      , "int32 value;"                      },
    { "floatBcd"        , "int32 value;"                      },
    { "floatBcdCon"     , "int32 value;"                      },
    { "float"           , "float value;"                      },
    { "doubleInt"       , "int64 value;"                      },
    { "doubleIntConfig" , "int64 value;"                      },
    { "doubleRatio"     , "int64 value;"                      },
    { "doubleBcd"       , "int64 value;"                      },
    { "doubleBcdCon"    , "int64 value;"                      },
    { "double"          , "double value;"                     },
    { "realText7"       , "char value[8];"                    },
    { "realText15"      , "char value[16];"                   },
    { "realText31"      , "char value[32];"                   },
    { "double"          , "double value;"                     },
    { "enum"            , "int value;"                        },
    { "timeLong"        , "int64 value;"                      },
    { "datetimeMsEpoch" , "int64 value;"                      },
    { "datetimeMsCn"    , "char value[8];"                    },
    { "datetimeDayCn"   , "char value[4];"                    },
    { "asciic"          , "char * value;"                     },
    { "string"          , "int iValueLength; char * value;"   },
    { "stream"          , "int iValueLength; char * value;"   },
    { "string2"         , "char value[2];"                   },
    { "string4"         , "char value[4];"                   },
    { "string8"         , "char value[8];"                   },
    { "string16"        , "char value[16];"                  },
    { "string32"        , "char value[32];"                  },
    { "string64"        , "char value[64];"                  },
    { "string128"       , "char value[128];"                 },
    { "string256"       , "char value[256];"                 },
    { "string512"       , "char value[512];"                 },
    { "string1024"      , "char value[1024];"                },
    { "string2048"      , "char value[2048];"                },
    { "string4096"      , "char value[4096];"                }
};

//0x0000xx00
string f_addresses[][2] = {
    { "address"       , "int address;" },
    { "firstAddress"  , "int addressBegin; int addressEnd;" },
    { "code"          , "int iCodeLength; char * pCode;" },
    { "path"          , "int iPathLength; char * pPath;" }
};

//0x000000xx
string f_values[][2] = {
    { "v"  , ""    },
    { "vqt"    , "int quality; long long datetime;"  }
};

int fn_datatypes()
{
    vector<string> ss;
    for (size_t i = 0; i < sizeof(f_addresses) / sizeof(f_addresses[0]); ++i)
    {
        for (size_t j = 0; j < sizeof(f_values) / sizeof(f_values[0]); ++j)
        {
            for (size_t k = 0; k < sizeof(f_datatypes) / sizeof(f_datatypes[0]); ++k)
            {
                int n = 0x01000000 + ((i+1) << 16) + ((j+1) << 8) + (k+1);
                string s(string("gcd_") + f_addresses[i][0] + "_" + f_values[j][0] + "_" + f_datatypes[k][0]);
                ss.push_back(CxString::toHexstring(n) + "\r\n" + s);
            }
        }
    }
    CxFile::save(CxFileSystem::mergeFilePath(CxApplication::applicationPath(), "datatype.txt"), ss);
    return 0;
}

int fn_structs()
{
    vector<string> ss;
    for (size_t i = 0; i < sizeof(f_addresses) / sizeof(f_addresses[0]); ++i)
    {
        for (size_t j = 0; j < sizeof(f_values) / sizeof(f_values[0]); ++j)
        {
            for (size_t k = 0; k < sizeof(f_datatypes) / sizeof(f_datatypes[0]); ++k)
            {
                int n = 0x01000000 + ((i+1) << 16) + ((j+1) << 8) + (k+1);
                string sDatatype(string("gcd_") + f_addresses[i][0] + "_" + f_datatypes[k][0] + "_" + f_values[j][0]);
                string sNumber = CxString::toHexstring(n);

                string sStruct;
                if (f_values[j][1].empty())
                {
                    sStruct = string("\"struct " + sDatatype + " {\n") + f_addresses[i][1] + "\n" + f_datatypes[k][1] + "\n};\"";
                }
                else
                {
                    sStruct = string("\"struct " + sDatatype + " {\n") + f_addresses[i][1] + "\n" + f_datatypes[k][1] + "\n" + f_values[j][1] + "\n};\"";
                }
                ss.push_back(sNumber + "," + sDatatype + "," + sStruct);
            }
        }
    }
    CxFile::save(CxFileSystem::mergeFilePath(CxApplication::applicationPath(), "struct.csv"), ss);
    return 0;
}

int fn_numbers()
{
    vector<string> ss;
    for (size_t i = 0; i < sizeof(f_addresses) / sizeof(f_addresses[0]); ++i)
    {
        for (size_t j = 0; j < sizeof(f_values) / sizeof(f_values[0]); ++j)
        {
            for (size_t k = 0; k < sizeof(f_datatypes) / sizeof(f_datatypes[0]); ++k)
            {
                int n = 0x01000000 + ((i+1) << 16) + ((j+1) << 8) + (k+1);
                ss.push_back(CxString::toHexstring(n));
            }
        }
    }
    CxFile::save(CxFileSystem::mergeFilePath(CxApplication::applicationPath(), "number.txt"), ss);
    return 0;
}

int main(int argc, const char * argv[])
{
    CxApplication::init(argc, argv);

    fn_datatypes();
    fn_structs();
    fn_numbers();

    return 0;
}

