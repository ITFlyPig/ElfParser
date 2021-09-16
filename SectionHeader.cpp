//
// Created by yuelin on 2021/9/15.
//

#include "SectionHeader.h"

Json::Value SectionHeader::to_json() {
    Json::Value json;
    json["index"] = index;
    json["name"] = name;
    json["type"] = type;
    json["addr"] = addr;
    json["offset"] = offset;
    json["size"] = sh_size;
    return json;
}
