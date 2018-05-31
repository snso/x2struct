/*
* Copyright (C) 2017 YY Inc. All rights reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License"); 
* you may not use this file except in compliance with the License. 
* You may obtain a copy of the License at
*
*	http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, 
* software distributed under the License is distributed on an "AS IS" BASIS, 
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
* See the License for the specific language governing permissions and 
* limitations under the License.
*/


#pragma once

#include <string>
#include <set>
#include <stdexcept>

#include "util.h"
#include "xreader.h"

#include "json_reader.h"
#include "json_writer.h"

#include "bson_reader.h"
#include "bson_writer.h"

#include "config_reader.h"
#include "config_writer.h"

#include "xml_reader.h"
#include "xml_writer.h"


#ifdef XTOSTRUCT_GEN_GOLANG_CODE
#include <typeinfo>
#include "go_writer.h"
#endif
//#include <iostream>
//using namespace std;

namespace x2struct {

#define X2STRUCT_OPT_ME     "me"    // must exist

class X {
public:
    /* TO STRUCT */
    template <typename TYPE>
    static bool loadxml(const std::string&str, TYPE&t, bool isfile=true) {
        XmlReader reader(str, isfile);
        reader.convert(t);
        return true;
    }
    template <typename TYPE>
    static bool loadjson(const std::string&str, TYPE&t, bool isfile=true) {
        JsonReader reader(str, isfile);
        reader.convert(t);
        return true;
    }
    template <typename TYPE>
    static bool loadbson(const uint8_t*data, size_t length, TYPE&t, bool copy=true) { // if length==0, get len from data
        BsonReader reader(data, length, copy);
        reader.convert(t);
        return true;
    }
    template <typename TYPE>
    static bool loadbson(const std::string&data, TYPE&t, bool copy=true) { // if length==0, get len from data
        BsonReader reader(data, copy);
        reader.convert(t);
        return true;
    }
    template <typename TYPE>
    static bool loadconfig(const std::string&str, TYPE&t, bool isfile=true, const std::string&root="") {
        ConfigReader reader(str, isfile, root);
        try {
            reader.convert(t);
            return true;
        } catch (std::exception &e) {
            reader.exception(e);
        }
        return false;
    }

    /* TO stream */
    /*
      indentCount 表示缩进的数目，<0表示不换行不缩进，0表示换行但是不缩进
      indentChar  表示缩进用的字符，要么是' '要么是'\t'
    */
    template <typename TYPE>
    static std::string toxml(const TYPE&t, const std::string&root, int indentCount=-1, char indentChar=' ') {
        XmlWriter writer(indentCount, indentChar);
        writer.convert(root.c_str(), t);
        return writer.toStr();
    }
    template <typename TYPE>
    static std::string tojson(const TYPE&t, const std::string&root="", int indentCount=-1, char indentChar=' ') {
        JsonWriter writer(indentCount, indentChar);
        writer.convert(root.c_str(), t);
        return writer.toStr();
    }
    template <typename TYPE>
    static std::string tobson(const TYPE& t) {
        BsonWriter writer;
        writer.convert("", t);
        return writer.toStr();
    }
    template <typename TYPE>
    static std::string tocfg(const TYPE&t, const std::string&root, int indentCount=-1, char indentChar=' ') {
        ConfigWriter writer(indentCount, indentChar);
        writer.convert(root.c_str(), t);
        return writer.toStr();
    }

    /* gen golang code*/
#ifdef XTOSTRUCT_GEN_GOLANG_CODE
    template <typename TYPE>
    static std::string togocode(const TYPE&t, bool json, bool bson, bool xml) {
        std::set<std::string> alls;
        GoCode obj(alls, json, bson, xml);
        return t.__struct_to_go(obj);
    }
#endif
};

#define X_STRUCT_FUNC_TOX_BEGIN                                             \
private:                                                                    \
    std::set<std::string> __x_has_string;                                   \
public:                                                                     \
    bool xhas(const std::string& name) const {                              \
        return __x_has_string.find(name)!=__x_has_string.end();             \
    }                                                                       \
    template<typename DOC>                                                  \
    bool __x_condition(DOC& obj, const std::string&name) {                  \
        (void)obj;(void)name;                                               \
        return true;                                                        \
    }                                                                       \
    template<typename DOC>                                                  \
    void __x_to_struct(DOC& obj) {

#define X_STRUCT_ACT_TOX_O(M)                                               \
        if (obj.has(#M)) {                                                  \
            obj[#M].convert(M);                                             \
            __x_has_string.insert(#M);                                      \
        }

#define X_STRUCT_ACT_TOX_M(M)                                               \
        if (obj.has(#M)) {                                                  \
            obj[#M].convert(M);                                             \
            __x_has_string.insert(#M);                                      \
        } else {                                                            \
            obj.me_exception(#M);                                           \
        }

// aliase name
#define X_STRUCT_ACT_TOX_A(M, A_NAME)                                       \
    {                                                                       \
        bool me = false;                                                    \
        std::string __alias__name__ = obj.hasa(#M, A_NAME, &me);            \
        const char*__an = __alias__name__.c_str();                          \
        if (obj.has(__an)) {                                                \
            obj[__an].convert(M);                                           \
            __x_has_string.insert(#M);                                      \
        } else if (obj.has(#M)) {                                           \
            obj[#M].convert(M);                                             \
            __x_has_string.insert(#M);                                      \
        } else if (me) {                                                    \
            obj.me_exception(__an);                                         \
        }                                                                   \
    }

#define X_STRUCT_FUNC_TOX_END }


// struct to string
#define X_STRUCT_FUNC_TOS_BEGIN                                                     \
    template <class CLASS>                                                          \
    void __struct_to_str(CLASS& obj, const char *root) const {

#define X_STRUCT_ACT_TOS_O(M)                                                       \
        obj.convert(#M, M);

#define X_STRUCT_ACT_TOS_A(M, A_NAME)                                               \
        obj.convert(x2struct::alias_parse(#M, A_NAME, obj.type(), 0).c_str(), M);

#define X_STRUCT_FUNC_TOS_END                                                       \
    }

    
// struct to golang code
#define X_STRUCT_FUNC_TOG_BEGIN                                                     \
    std::string __struct_to_go(x2struct::GoCode &obj) const {                       \
        obj.set_type_id_name(typeid(*this).name());

#define X_STRUCT_ACT_TOG_O(M)                                                       \
        obj.convert(M, #M);

#define X_STRUCT_ACT_TOG_A(M, A_NAME)                                               \
        obj.convert(M, #M, A_NAME);

#define X_STRUCT_FUNC_TOG_END                                                       \
        return obj.str();                                                           \
    }


// lenght should equal X_STRUCT_N
#define X_DEC_LIST _32,_31,_30,_29,_28,_27,_26,_25,_24,_23,_22,_21,_20,_19,_18,_17,_16,_15,_14,_13,_12,_11,_10,_9,_8,_7,_6,_5,_4,_3,_2,_1
// expand macro depend on parameters number
#define X_STRUCT_N(ACTION,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,N,...) ACTION##N

/*
    work with X_STRUCT_N to expand macro
    struct MyX {
        int         a;
        std::string b;
        double      c;
        XTOSTRUCT(A(a,"_id"), O(b,c));
    };
    there'r two layer in this macro:
    l1: A(a,"_id"), O(b,c)
    l2: a,"_id"; b,c
    so we need expand twice
*/

// wrap X_STRUCT_N. so we can use X_DEC_LIST
#define X_STRUCT_WRAP_L1(ACT, LIST, ...) X_STRUCT_N(X_STRUCT_L1, __VA_ARGS__, LIST)(ACT, __VA_ARGS__)
#define X_STRUCT_WRAP_L2(ACT, LIST, ...) X_STRUCT_N(X_STRUCT_L2, __VA_ARGS__, LIST)(ACT, __VA_ARGS__)

#define X_STRUCT_L1_DEF(ACT, M)      X_STRUCT_L1_##ACT##M
#define X_STRUCT_L1_1(ACT, M)        X_STRUCT_L1_DEF(ACT, M)
#define X_STRUCT_L1_2(ACT, M,...)    X_STRUCT_L1_DEF(ACT, M)      X_STRUCT_L1_1(ACT, __VA_ARGS__)
#define X_STRUCT_L1_3(ACT, M,...)    X_STRUCT_L1_DEF(ACT, M)      X_STRUCT_L1_2(ACT, __VA_ARGS__)
#define X_STRUCT_L1_4(ACT, M,...)    X_STRUCT_L1_DEF(ACT, M)      X_STRUCT_L1_3(ACT, __VA_ARGS__)
#define X_STRUCT_L1_5(ACT, M,...)    X_STRUCT_L1_DEF(ACT, M)      X_STRUCT_L1_4(ACT, __VA_ARGS__)
#define X_STRUCT_L1_6(ACT, M,...)    X_STRUCT_L1_DEF(ACT, M)      X_STRUCT_L1_5(ACT, __VA_ARGS__)
#define X_STRUCT_L1_7(ACT, M,...)    X_STRUCT_L1_DEF(ACT, M)      X_STRUCT_L1_6(ACT, __VA_ARGS__)
#define X_STRUCT_L1_8(ACT, M,...)    X_STRUCT_L1_DEF(ACT, M)      X_STRUCT_L1_7(ACT, __VA_ARGS__)
#define X_STRUCT_L1_9(ACT, M,...)    X_STRUCT_L1_DEF(ACT, M)      X_STRUCT_L1_8(ACT, __VA_ARGS__)
#define X_STRUCT_L1_10(ACT, M,...)   X_STRUCT_L1_DEF(ACT, M)      X_STRUCT_L1_9(ACT, __VA_ARGS__)
#define X_STRUCT_L1_11(ACT, M,...)   X_STRUCT_L1_DEF(ACT, M)      X_STRUCT_L1_10(ACT, __VA_ARGS__)
#define X_STRUCT_L1_12(ACT, M,...)   X_STRUCT_L1_DEF(ACT, M)      X_STRUCT_L1_11(ACT, __VA_ARGS__)
#define X_STRUCT_L1_13(ACT, M,...)   X_STRUCT_L1_DEF(ACT, M)      X_STRUCT_L1_12(ACT, __VA_ARGS__)
#define X_STRUCT_L1_14(ACT, M,...)   X_STRUCT_L1_DEF(ACT, M)      X_STRUCT_L1_13(ACT, __VA_ARGS__)
#define X_STRUCT_L1_15(ACT, M,...)   X_STRUCT_L1_DEF(ACT, M)      X_STRUCT_L1_14(ACT, __VA_ARGS__)
#define X_STRUCT_L1_16(ACT, M,...)   X_STRUCT_L1_DEF(ACT, M)      X_STRUCT_L1_15(ACT, __VA_ARGS__)
#define X_STRUCT_L1_17(ACT, M,...)   X_STRUCT_L1_DEF(ACT, M)      X_STRUCT_L1_16(ACT, __VA_ARGS__)
#define X_STRUCT_L1_18(ACT, M,...)   X_STRUCT_L1_DEF(ACT, M)      X_STRUCT_L1_17(ACT, __VA_ARGS__)
#define X_STRUCT_L1_19(ACT, M,...)   X_STRUCT_L1_DEF(ACT, M)      X_STRUCT_L1_18(ACT, __VA_ARGS__)
#define X_STRUCT_L1_20(ACT, M,...)   X_STRUCT_L1_DEF(ACT, M)      X_STRUCT_L1_19(ACT, __VA_ARGS__)
#define X_STRUCT_L1_21(ACT, M,...)   X_STRUCT_L1_DEF(ACT, M)      X_STRUCT_L1_20(ACT, __VA_ARGS__)
#define X_STRUCT_L1_22(ACT, M,...)   X_STRUCT_L1_DEF(ACT, M)      X_STRUCT_L1_21(ACT, __VA_ARGS__)
#define X_STRUCT_L1_23(ACT, M,...)   X_STRUCT_L1_DEF(ACT, M)      X_STRUCT_L1_22(ACT, __VA_ARGS__)
#define X_STRUCT_L1_24(ACT, M,...)   X_STRUCT_L1_DEF(ACT, M)      X_STRUCT_L1_23(ACT, __VA_ARGS__)
#define X_STRUCT_L1_25(ACT, M,...)   X_STRUCT_L1_DEF(ACT, M)      X_STRUCT_L1_24(ACT, __VA_ARGS__)
#define X_STRUCT_L1_26(ACT, M,...)   X_STRUCT_L1_DEF(ACT, M)      X_STRUCT_L1_25(ACT, __VA_ARGS__)
#define X_STRUCT_L1_27(ACT, M,...)   X_STRUCT_L1_DEF(ACT, M)      X_STRUCT_L1_26(ACT, __VA_ARGS__)
#define X_STRUCT_L1_28(ACT, M,...)   X_STRUCT_L1_DEF(ACT, M)      X_STRUCT_L1_27(ACT, __VA_ARGS__)
#define X_STRUCT_L1_29(ACT, M,...)   X_STRUCT_L1_DEF(ACT, M)      X_STRUCT_L1_28(ACT, __VA_ARGS__)
#define X_STRUCT_L1_30(ACT, M,...)   X_STRUCT_L1_DEF(ACT, M)      X_STRUCT_L1_29(ACT, __VA_ARGS__)
#define X_STRUCT_L1_31(ACT, M,...)   X_STRUCT_L1_DEF(ACT, M)      X_STRUCT_L1_30(ACT, __VA_ARGS__)
#define X_STRUCT_L1_32(ACT, M,...)   X_STRUCT_L1_DEF(ACT, M)      X_STRUCT_L1_31(ACT, __VA_ARGS__)

#define X_STRUCT_L2_DEF(ACT, M)     X_STRUCT_ACT_##ACT(M)
#define X_STRUCT_L2_1(ACT, M)       X_STRUCT_L2_DEF(ACT, M)
#define X_STRUCT_L2_2(ACT, M,...)   X_STRUCT_L2_DEF(ACT, M)     X_STRUCT_L2_1(ACT, __VA_ARGS__)
#define X_STRUCT_L2_3(ACT, M,...)   X_STRUCT_L2_DEF(ACT, M)     X_STRUCT_L2_2(ACT, __VA_ARGS__)
#define X_STRUCT_L2_4(ACT, M,...)   X_STRUCT_L2_DEF(ACT, M)     X_STRUCT_L2_3(ACT, __VA_ARGS__)
#define X_STRUCT_L2_5(ACT, M,...)   X_STRUCT_L2_DEF(ACT, M)     X_STRUCT_L2_4(ACT, __VA_ARGS__)
#define X_STRUCT_L2_6(ACT, M,...)   X_STRUCT_L2_DEF(ACT, M)     X_STRUCT_L2_5(ACT, __VA_ARGS__)
#define X_STRUCT_L2_7(ACT, M,...)   X_STRUCT_L2_DEF(ACT, M)     X_STRUCT_L2_6(ACT, __VA_ARGS__)
#define X_STRUCT_L2_8(ACT, M,...)   X_STRUCT_L2_DEF(ACT, M)     X_STRUCT_L2_7(ACT, __VA_ARGS__)
#define X_STRUCT_L2_9(ACT, M,...)   X_STRUCT_L2_DEF(ACT, M)     X_STRUCT_L2_8(ACT, __VA_ARGS__)
#define X_STRUCT_L2_10(ACT, M,...)  X_STRUCT_L2_DEF(ACT, M)     X_STRUCT_L2_9(ACT, __VA_ARGS__)
#define X_STRUCT_L2_11(ACT, M,...)  X_STRUCT_L2_DEF(ACT, M)     X_STRUCT_L2_10(ACT, __VA_ARGS__)
#define X_STRUCT_L2_12(ACT, M,...)  X_STRUCT_L2_DEF(ACT, M)     X_STRUCT_L2_11(ACT, __VA_ARGS__)
#define X_STRUCT_L2_13(ACT, M,...)  X_STRUCT_L2_DEF(ACT, M)     X_STRUCT_L2_12(ACT, __VA_ARGS__)
#define X_STRUCT_L2_14(ACT, M,...)  X_STRUCT_L2_DEF(ACT, M)     X_STRUCT_L2_13(ACT, __VA_ARGS__)
#define X_STRUCT_L2_15(ACT, M,...)  X_STRUCT_L2_DEF(ACT, M)     X_STRUCT_L2_14(ACT, __VA_ARGS__)
#define X_STRUCT_L2_16(ACT, M,...)  X_STRUCT_L2_DEF(ACT, M)     X_STRUCT_L2_15(ACT, __VA_ARGS__)
#define X_STRUCT_L2_17(ACT, M,...)  X_STRUCT_L2_DEF(ACT, M)     X_STRUCT_L2_16(ACT, __VA_ARGS__)
#define X_STRUCT_L2_18(ACT, M,...)  X_STRUCT_L2_DEF(ACT, M)     X_STRUCT_L2_17(ACT, __VA_ARGS__)
#define X_STRUCT_L2_19(ACT, M,...)  X_STRUCT_L2_DEF(ACT, M)     X_STRUCT_L2_18(ACT, __VA_ARGS__)
#define X_STRUCT_L2_20(ACT, M,...)  X_STRUCT_L2_DEF(ACT, M)     X_STRUCT_L2_19(ACT, __VA_ARGS__)
#define X_STRUCT_L2_21(ACT, M,...)  X_STRUCT_L2_DEF(ACT, M)     X_STRUCT_L2_20(ACT, __VA_ARGS__)
#define X_STRUCT_L2_22(ACT, M,...)  X_STRUCT_L2_DEF(ACT, M)     X_STRUCT_L2_21(ACT, __VA_ARGS__)
#define X_STRUCT_L2_23(ACT, M,...)  X_STRUCT_L2_DEF(ACT, M)     X_STRUCT_L2_22(ACT, __VA_ARGS__)
#define X_STRUCT_L2_24(ACT, M,...)  X_STRUCT_L2_DEF(ACT, M)     X_STRUCT_L2_23(ACT, __VA_ARGS__)
#define X_STRUCT_L2_25(ACT, M,...)  X_STRUCT_L2_DEF(ACT, M)     X_STRUCT_L2_24(ACT, __VA_ARGS__)
#define X_STRUCT_L2_26(ACT, M,...)  X_STRUCT_L2_DEF(ACT, M)     X_STRUCT_L2_25(ACT, __VA_ARGS__)
#define X_STRUCT_L2_27(ACT, M,...)  X_STRUCT_L2_DEF(ACT, M)     X_STRUCT_L2_26(ACT, __VA_ARGS__)
#define X_STRUCT_L2_28(ACT, M,...)  X_STRUCT_L2_DEF(ACT, M)     X_STRUCT_L2_27(ACT, __VA_ARGS__)
#define X_STRUCT_L2_29(ACT, M,...)  X_STRUCT_L2_DEF(ACT, M)     X_STRUCT_L2_28(ACT, __VA_ARGS__)
#define X_STRUCT_L2_30(ACT, M,...)  X_STRUCT_L2_DEF(ACT, M)     X_STRUCT_L2_29(ACT, __VA_ARGS__)
#define X_STRUCT_L2_31(ACT, M,...)  X_STRUCT_L2_DEF(ACT, M)     X_STRUCT_L2_30(ACT, __VA_ARGS__)
#define X_STRUCT_L2_32(ACT, M,...)  X_STRUCT_L2_DEF(ACT, M)     X_STRUCT_L2_31(ACT, __VA_ARGS__)

// L1 action define. O M A
// string to struct
#define X_STRUCT_L1_TOX_O(...)  X_STRUCT_WRAP_L2(TOX_O, X_DEC_LIST, __VA_ARGS__)
#define X_STRUCT_L1_TOX_M(...)  X_STRUCT_WRAP_L2(TOX_M, X_DEC_LIST, __VA_ARGS__)
#define X_STRUCT_L1_TOX_A(M,A)  X_STRUCT_ACT_TOX_A(M,A)

// struct to string
#define X_STRUCT_L1_TOS_O(...)  X_STRUCT_WRAP_L2(TOS_O, X_DEC_LIST, __VA_ARGS__)
#define X_STRUCT_L1_TOS_M       X_STRUCT_L1_TOS_O
#define X_STRUCT_L1_TOS_A(M,A)  X_STRUCT_ACT_TOS_A(M,A)

// struct to golang code
#define X_STRUCT_L1_TOG_O(...)  X_STRUCT_WRAP_L2(TOG_O, X_DEC_LIST, __VA_ARGS__)
#define X_STRUCT_L1_TOG_M       X_STRUCT_L1_TOG_O
#define X_STRUCT_L1_TOG_A(M,A)  X_STRUCT_ACT_TOG_A(M,A)


#ifdef XTOSTRUCT_GEN_GOLANG_CODE
#define XTOSTRUCT(...)  \
    X_STRUCT_FUNC_TOX_BEGIN  X_STRUCT_WRAP_L1(TOX_, X_DEC_LIST, __VA_ARGS__) X_STRUCT_FUNC_TOX_END  \
    X_STRUCT_FUNC_TOS_BEGIN  X_STRUCT_WRAP_L1(TOS_, X_DEC_LIST, __VA_ARGS__) X_STRUCT_FUNC_TOS_END  \
    X_STRUCT_FUNC_TOG_BEGIN  X_STRUCT_WRAP_L1(TOG_, X_DEC_LIST, __VA_ARGS__) X_STRUCT_FUNC_TOG_END
#else
#define XTOSTRUCT(...)  \
    X_STRUCT_FUNC_TOX_BEGIN  X_STRUCT_WRAP_L1(TOX_, X_DEC_LIST, __VA_ARGS__) X_STRUCT_FUNC_TOX_END  \
    X_STRUCT_FUNC_TOS_BEGIN  X_STRUCT_WRAP_L1(TOS_, X_DEC_LIST, __VA_ARGS__) X_STRUCT_FUNC_TOS_END
#endif


#define XTOSTRUCT_CONDITION()   template<typename DOC> bool __x_condition(DOC& obj, const char* name)
#define XTOSTRUCT_CONDITION_EQ(attr1, attr2)                    \
    template<typename DOC>                                      \
    bool __x_condition(DOC& obj, const char* name) {            \
        return obj.attribute(attr1)==obj.attribute(attr2);      \
    }

}


