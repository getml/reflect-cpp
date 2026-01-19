module;

#ifdef REFLECTCPP_JSON
#include "rfl/json.hpp"
#endif

export module rfl:json;

export namespace rfl::json {
    #ifdef REFLECTCPP_JSON
    using rfl::json::Parser;
    using rfl::json::Reader;
    using rfl::json::Writer;
    using rfl::json::InputObjectType;
    using rfl::json::InputVarType;
    using rfl::json::TypeHelper;

    using rfl::json::pretty;

    using rfl::json::load;
    using rfl::json::read;
    using rfl::json::save;
    using rfl::json::to_schema;
    using rfl::json::write;

    namespace schema {
        using rfl::json::schema::JSONSchema;
        using rfl::json::schema::Type;
    }
    #endif
}