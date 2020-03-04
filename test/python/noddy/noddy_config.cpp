
#include "noddy_config.h"

#include "noddy1.h"
#include "noddy2.h"
#include "noddy3.h"
#include "noddy4.h"

InitTypeTable gInitTableNoddy[] =
    {
        { "Noddy1", Noddy1::getPyType},
        { "Noddy2", Noddy2::getPyType },
        { "Noddy3", Noddy3::getPyType },
        { "Noddy3", Noddy3::getPyType },
        { NULL, NULL },
    };

