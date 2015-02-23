#ifndef MAP_OBJECT_H
#define MAP_OBJECT_H

#include "map_object_global.h"
#include <string>

class MAP_OBJECTSHARED_EXPORT Map_Object
{

public:
    Map_Object();
    struct node;
    //enum node_type;

    enum node_type      //Enumeration type to track node type
    {
        track,
        two_switch,
        cross,
        dead_end,       //will only use connectionA
        null
    };

    struct node     //struct type to hold information for each node
    {
        std::string     node_name;
        node_type   type = null;
        int         length = 0;
                                            //linked node usage determined by current node_type
        node*       connectionA = NULL;     //normal track = inbound; two_switch = open; cross = path1
        node*       connectionB = NULL;     //normal track = null; two_switch = null; cross = path1
        node*       connectionC = NULL;     //normal track = outbound; two_switch = bypass; cross = path2
        node*       connectionD = NULL;     //normal track = null; two_switch = outbound; cross = path2
    };
};

#endif // MAP_OBJECT_H
