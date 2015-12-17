//
// Created by mklong on 15/12/13.
//

#ifndef __RAPIDJSON2STRUCT_H__
#define __RAPIDJSON2STRUCT_H__

#include <string>
#include <vector>

#include "rapidjson/document.h"

using namespace std;

namespace rapidjson2struct{

    typedef int                 type_int;
    typedef unsigned int        type_uint;
    typedef long long           type_int64;
    typedef unsigned long long  type_uint64;
    typedef double              type_double;


    enum{
        PARSE_OK                    = 0,
        PARSE_ERR_REQUIRED_NAME     = 1,
        PARSE_ERR_VALUE_TYPE        = 2,
        PARSE_ERR_UNKOWN_TYPE       = 3,
        PARSE_ERR_FORBIDDEN_TYPE    = 4,
        PARSE_ERR_NUMBER_TYPE       = 5,
        PARSE_ERR_NULL_ALLOCATOR    = 6,
    };

    enum member_type_e {
        TYPE_DEFAULT    = 0x0,
        TYPE_BOOL       = 0x1,
        TYPE_STRING     = 0x2,
        TYPE_STRUCT     = 0x3,
        TYPE_ARRAY      = 0x4,
        TYPE_NUMBER     = 0x5,
    };

    enum number_type_e {
        NUMBER_DEFAULT    = 0x0,
        NUMBER_INT        = 0x1,
        NUMBER_UINT       = 0x2,
        NUMBER_INT64      = 0x4,
        NUMBER_UINT64     = 0x8,
        NUMBER_DOUBLE     = 0x10,
    };

    typedef void * (*array_struct_allocator)();

    template <typename  _Ty >
    static void * alloc(){
        return new _Ty;
    }

    struct member_meta{

        member_meta():obj_(NULL),type_(TYPE_DEFAULT),required_(false),
                      alloc_func_(NULL),arr_type_(TYPE_DEFAULT),
                      num_type_(NUMBER_DEFAULT){}

        void *                  obj_;       //member
        member_type_e           type_;      //member type
        string                  name_;      //json name
        bool                    required_;  //whether required fields in json
        member_type_e           arr_type_;  //array data type
        array_struct_allocator  alloc_func_;//if struct,need allocator
        number_type_e           num_type_;  //number type
    };

    class base {



    public:
        base(){}
        virtual ~base(){}

        virtual void parser_add_members() = 0;

        const string & get_parse_error_name(){
            return this->parse_error_name_;
        }

    protected:
        /**
         *      bool
         * */
        void add_bool_member(const bool & b ,const char * name ,bool required = false){

            add_member_meta((void *)&b,TYPE_BOOL,name ,required);
        }

        /**
         *      string
         * */
        void add_string_member(const string & str,const char * name,bool required = false){

            add_member_meta((void *)&str,TYPE_STRING,name ,required);
        }

        /**
         *      number
         * */
        void add_number_member(const type_int & num,const char * name, bool required = false){

            add_member_meta((void *)&num,TYPE_NUMBER,name ,required,NUMBER_INT);
        }

        void add_number_member(const type_uint & num,const char * name, bool required = false){

            add_member_meta((void *)&num,TYPE_NUMBER,name ,required,NUMBER_UINT);
        }

        void add_number_member(const type_int64 & num,const char * name, bool required = false){

            add_member_meta((void *)&num,TYPE_NUMBER,name ,required,NUMBER_INT64);
        }

        void add_number_member(const type_uint64 & num,const char * name, bool required = false){

            add_member_meta((void *)&num,TYPE_NUMBER,name ,required,NUMBER_UINT64);
        }

        void add_number_member(const type_double & num,const char * name, bool required = false){

            add_member_meta((void *)&num,TYPE_NUMBER,name ,required,NUMBER_DOUBLE);
        }

        /**
         *      struct
         * */

        void add_struct_member(void * obj,const char * name ,bool required = false){

            add_member_meta(obj,TYPE_STRUCT,name ,required);
        }


        /**
         *      array bool      :   vector< bool >
         * */

        void add_array_bool_member(const vector<bool> & b ,const char * name ,bool required = false) {

            add_member_meta((void *)&b,TYPE_ARRAY,name ,required,NUMBER_DEFAULT,TYPE_BOOL,NULL);
        }

        /**
         *      array string    :   vector< string >
         * */

        void add_array_string_member(const vector<string> & str ,const char * name ,bool required = false) {

            add_member_meta((void *)&str,TYPE_ARRAY,name ,required,NUMBER_DEFAULT,TYPE_STRING,NULL);
        }

        /**
         *      array struct    :   vector< type * >
         * */

        void add_array_struct_member(void * arr ,const char * name ,array_struct_allocator func ,
                                     bool required = false) {

            add_member_meta(arr,TYPE_ARRAY,name ,required,NUMBER_DEFAULT,TYPE_STRUCT,func);
        }

        /**
         *      array number    :   vector< type_number >
         * */

        void add_array_number_member(const vector<type_int>& num,const char * name ,
                                     bool required = false ) {

            add_member_meta((void *)&num,TYPE_ARRAY,name ,required,NUMBER_INT,TYPE_NUMBER,NULL);
        }

        void add_array_number_member(const vector<type_uint>& num,const char * name ,
                                     bool required = false ) {

            add_member_meta((void *)&num,TYPE_ARRAY,name ,required,NUMBER_UINT,TYPE_NUMBER,NULL);
        }

        void add_array_number_member(const vector<type_int64>& num,const char * name ,
                                     bool required = false) {

            add_member_meta((void *)&num,TYPE_ARRAY,name ,required,NUMBER_INT64,TYPE_NUMBER,NULL);
        }

        void add_array_number_member(const vector<type_uint64>& num,const char * name ,
                                     bool required = false) {

            add_member_meta((void *)&num,TYPE_ARRAY,name ,required,NUMBER_UINT64,TYPE_NUMBER,NULL);
        }

        void add_array_number_member(const vector<type_double>& num,const char * name ,
                                     bool required = false) {

            add_member_meta((void *)&num,TYPE_ARRAY,name ,required,NUMBER_DOUBLE,TYPE_NUMBER,NULL);
        }

    private:

        void add_member_meta(void * obj,member_type_e type,const char * name ,bool required ,
                             number_type_e num_type = NUMBER_DEFAULT,
                             member_type_e arr_type = TYPE_DEFAULT,
                             array_struct_allocator func = NULL){
            member_meta m;
            
            m.obj_ = obj;
            m.type_ = type;
            m.name_ = name;
            m.required_ = required;
            m.arr_type_ = arr_type;
            m.alloc_func_ = func;
            m.num_type_ = num_type;

            members_.push_back(m);
        }

    public:

        //if add const ,error occur
        int parse(rapidjson::Document & doc){

            const rapidjson::Value & value = static_cast<rapidjson::Value& >(doc);
            return parse(value);
        }

        int parse(const rapidjson::Value & value){

            int ret = 0;
            parse_error_name_.clear();

            for (int i = 0; i < members_.size(); ++i) {
                const member_meta & m = members_[i];

                switch(m.type_){

                    case TYPE_BOOL:
                    {
                        const rapidjson::Value &bool_val = value[m.name_.c_str()];
                        if (bool_val.IsNull()) {
                            if (m.required_) {
                                parse_error_name_ = m.name_;
                                return PARSE_ERR_REQUIRED_NAME;
                            }
                            break;
                        }
                        if (!bool_val.IsBool()) {
                            parse_error_name_ = m.name_;
                            return PARSE_ERR_VALUE_TYPE;
                        }

                        *(bool *) m.obj_ = bool_val.GetBool();
                    }
                        break;

                    case TYPE_STRING:
                    {
                        const rapidjson::Value &string_val = value[m.name_.c_str()];
                        if (string_val.IsNull()) {
                            if (m.required_) {
                                parse_error_name_ = m.name_;
                                return PARSE_ERR_REQUIRED_NAME;
                            }
                            break;
                        }
                        if (!string_val.IsString()) {
                            parse_error_name_ = m.name_;
                            return PARSE_ERR_VALUE_TYPE;
                        }

                        static_cast<std::string * >(m.obj_)->append(string_val.GetString(),
                                                                    string_val.GetStringLength());
                    }
                        break;

                    case TYPE_STRUCT:
                    {
                        const rapidjson::Value &obj_val = value[m.name_.c_str()];
                        if (obj_val.IsNull()) {
                            if (m.required_) {
                                parse_error_name_ = m.name_;
                                return PARSE_ERR_REQUIRED_NAME;
                            }
                            break;
                        }
                        if (!obj_val.IsObject()) {
                            parse_error_name_ = m.name_;
                            return PARSE_ERR_VALUE_TYPE;
                        }

                        base * obj = static_cast<base * >(m.obj_);
                        obj->parser_add_members();
                        ret = obj->parse(obj_val);
                        if (ret) {
                            parse_error_name_ = static_cast<base * >(m.obj_)->get_parse_error_name();
                            return ret;
                        }
                    }
                        break;

                    case TYPE_ARRAY:
                    {
                        const rapidjson::Value &array_val = value[m.name_.c_str()];
                        if (array_val.IsNull()) {
                            if (m.required_) {
                                parse_error_name_ = m.name_;
                                return PARSE_ERR_REQUIRED_NAME;
                            }
                            break;
                        }
                        if (!array_val.IsArray()) {
                            parse_error_name_ = m.name_;
                            return PARSE_ERR_VALUE_TYPE;
                        }

                        switch (m.arr_type_) {

                            case TYPE_BOOL:
                            {
                                vector<bool> *bool_arr = static_cast<vector<bool> *>(m.obj_);
                                for (int i = 0; i < array_val.Size(); i++) {
                                    if (!array_val[i].IsBool()) {
                                        parse_error_name_ = m.name_;
                                        return PARSE_ERR_VALUE_TYPE;
                                    }
                                    bool b = array_val[i].GetBool();
                                    bool_arr->push_back(b);
                                }
                            }
                                break;
                            case TYPE_STRING:
                            {
                                vector<string> *string_arr = static_cast<vector<string> *>(m.obj_);
                                for (int i = 0; i < array_val.Size(); i++) {
                                    if (!array_val[i].IsString()) {
                                        parse_error_name_ = m.name_;
                                        return PARSE_ERR_VALUE_TYPE;
                                    }

                                    string_arr->push_back(
                                            string(array_val[i].GetString(), array_val[i].GetStringLength()));
                                }
                            }
                                break;
                            case TYPE_STRUCT:
                            {
                                vector<base *> *struct_arr = static_cast<vector<base *> *>(m.obj_);
                                for (int i = 0; i < array_val.Size(); i++) {
                                    if (!array_val[i].IsObject()) {
                                        parse_error_name_ = m.name_;
                                        return PARSE_ERR_VALUE_TYPE;
                                    }

                                    if (m.alloc_func_ == NULL){
                                        parse_error_name_ = m.name_;
                                        return PARSE_ERR_NULL_ALLOCATOR;
                                    }
                                    base *obj = static_cast<base *>( m.alloc_func_());
                                    //add before parse,to prevent memory leaks
                                    struct_arr->push_back(obj);
                                    obj->parser_add_members();
                                    ret = obj->parse(array_val[i]);
                                    if (ret) {

                                        parse_error_name_ = obj->get_parse_error_name();
                                        return ret;
                                    }
                                }
                            }
                                break;

                            case TYPE_ARRAY:
                                parse_error_name_ = m.name_;
                                return PARSE_ERR_FORBIDDEN_TYPE;

                            case TYPE_NUMBER:

                                switch (m.num_type_) {

                                    case NUMBER_INT:
                                    {
                                        vector<type_int> *int_arr = static_cast<vector<type_int> *>(m.obj_);
                                        for (int i = 0; i < array_val.Size(); i++) {
                                            if (!array_val[i].IsInt()) {
                                                parse_error_name_ = m.name_;
                                                return PARSE_ERR_NUMBER_TYPE;
                                            }

                                            int_arr->push_back(array_val[i].GetInt());
                                        }
                                    }
                                        break;

                                    case NUMBER_UINT:
                                    {
                                        vector<type_uint> *uint_arr = static_cast<vector<type_uint> *>(m.obj_);
                                        for (int i = 0; i < array_val.Size(); i++) {
                                            if (!array_val[i].IsUint()) {
                                                parse_error_name_ = m.name_;
                                                return PARSE_ERR_NUMBER_TYPE;
                                            }

                                            uint_arr->push_back(array_val[i].GetUint());
                                        }
                                    }
                                        break;

                                    case NUMBER_INT64:
                                    {
                                        vector<type_int64> *int64_arr = static_cast<vector<type_int64> *>(m.obj_);
                                        for (int i = 0; i < array_val.Size(); i++) {
                                            if (!array_val[i].IsInt64()) {
                                                parse_error_name_ = m.name_;
                                                return PARSE_ERR_NUMBER_TYPE;
                                            }

                                            int64_arr->push_back(array_val[i].GetInt64());
                                        }
                                    }
                                        break;

                                    case NUMBER_UINT64:
                                    {
                                        vector<type_uint64> *uint64_arr = static_cast<vector<type_uint64> *>(m.obj_);
                                        for (int i = 0; i < array_val.Size(); i++) {
                                            if (!array_val[i].IsUint64()) {
                                                parse_error_name_ = m.name_;
                                                return PARSE_ERR_NUMBER_TYPE;
                                            }

                                            uint64_arr->push_back(array_val[i].GetUint64());
                                        }
                                    }
                                        break;

                                    case NUMBER_DOUBLE:
                                    {
                                        vector<type_double> *double_arr = static_cast<vector<type_double> *>(m.obj_);
                                        for (int i = 0; i < array_val.Size(); i++) {
                                            if (!array_val[i].IsDouble()) {
                                                parse_error_name_ = m.name_;
                                                return PARSE_ERR_NUMBER_TYPE;
                                            }

                                            double_arr->push_back(array_val[i].GetDouble());
                                        }
                                    }
                                        break;

                                    default:
                                        parse_error_name_ = m.name_;
                                        return PARSE_ERR_UNKOWN_TYPE;
                                }

                                break;

                            default:
                                return PARSE_ERR_UNKOWN_TYPE;
                        }
                    }
                        break;

                    case TYPE_NUMBER: {
                        const rapidjson::Value &number_val = value[m.name_.c_str()];
                        if (number_val.IsNull()) {
                            if (m.required_) {
                                parse_error_name_ = m.name_;
                                return PARSE_ERR_REQUIRED_NAME;
                            }
                            break;
                        }
                        if (!number_val.IsNumber()) {
                            parse_error_name_ = m.name_;
                            return PARSE_ERR_VALUE_TYPE;
                        }

                        switch (m.num_type_) {
                            case NUMBER_INT:
                                if (number_val.IsInt()) {
                                    *(type_int *) m.obj_ = number_val.GetInt();
                                } else {
                                    parse_error_name_ = m.name_;
                                    return PARSE_ERR_NUMBER_TYPE;
                                }

                                break;
                            case NUMBER_UINT:
                                if (number_val.IsUint()) {
                                    *(type_uint *) m.obj_ = number_val.GetUint();
                                } else {
                                    parse_error_name_ = m.name_;
                                    return PARSE_ERR_NUMBER_TYPE;
                                }

                                break;
                            case NUMBER_INT64:
                                if (number_val.IsInt64()) {
                                    *(type_int64 *) m.obj_ = number_val.GetInt64();
                                } else {
                                    parse_error_name_ = m.name_;
                                    return PARSE_ERR_NUMBER_TYPE;
                                }

                                break;
                            case NUMBER_UINT64:
                                if (number_val.IsUint64()) {
                                    *(type_uint64 *) m.obj_ = number_val.GetUint64();
                                } else {
                                    parse_error_name_ = m.name_;
                                    return PARSE_ERR_NUMBER_TYPE;
                                }
                                break;
                            case NUMBER_DOUBLE:
                                if (number_val.IsDouble()) {
                                    *(type_double *) m.obj_ = number_val.GetDouble();
                                } else {
                                    parse_error_name_ = m.name_;
                                    return PARSE_ERR_NUMBER_TYPE;
                                }

                                break;

                            default:
                                parse_error_name_ = m.name_;
                                return PARSE_ERR_NUMBER_TYPE;
                        }
                    }
                        break;

                    default:
                        return PARSE_ERR_UNKOWN_TYPE;
                }
            }

            return 0;

        }

    private:

        string  parse_error_name_;
        vector<member_meta> members_;

    };

}



#endif //SERVER_RAPIDJSON2STRUCT_H
