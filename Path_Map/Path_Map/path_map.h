#ifndef PATH_MAP_H
#define PATH_MAP_H

#include "path_map_global.h"
#include <map>
#include <string>

class PATH_MAPSHARED_EXPORT Path_Map
{

public:
    Path_Map();
    enum node_type;
    struct  node;
    std::map <std::string, node> path_map;

/* The manner in which the key string is named will indicate the directional
 * limitations of a track node. The same preceding name ending in _A, _B, _C, or _D
 * indicate it is all the same track peice. Switches will have _A and _B nodes
 * so that they can either be one in two out, or two in, one out. Each normal track
 * piece will have _A and _B nodes to so the peice will be treated as unidirectional
 * i.e. one in, one out, to prevent the train from making an assumed uturn without
 * changing direction. A cross piece will have all _A, _B, _C, and _D. This is so it
 * can represent one node, with unidirectional paths, that won't allow phsyically
 * impossible turns. _A and _B should be the same physical part, and _C and _D are
 * representations of the other crossing part.
 */

    enum node_type      //Enumeration type to track node type
    {
        track,
        two_switch,
        cross,
        dead_end,
        null
    };

    struct node
    {
        node_type   type = null;
        int         length = 0;
                                              //linked node usage determined by current node_type
        std::string connectionA = "NULL";     //normal track = inbound; two_switch = open; cross = path1
        std::string connectionB = "NULL";     //normal track = null; two_switch = null; cross = path1
        std::string connectionC = "NULL";     //normal track = outbound; two_switch = bypass; cross = path2
        std::string connectionD = "NULL";     //normal track = null; two_switch = outbound; cross = path2
    };
};

#endif // PATH_MAP_H
