#ifndef __BRO_STRINGIFYABLE__
#define __BRO_STRINGIFYABLE__
    #include<bro_global.h>
    class Stringifyable
    {
        public:
            virtual string stringify()=0;
    };
#endif