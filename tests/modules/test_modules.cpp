#include <string>
#include <vector>

import rfl;
import rfl.cli;
import rfl.generic;
import rfl.json;

#if REFLECTCPP_MODULE_TEST_AVRO
import rfl.avro;
#endif
#if REFLECTCPP_MODULE_TEST_BOOST_SERIALIZATION
import rfl.boost_serialization;
#endif
#if REFLECTCPP_MODULE_TEST_BSON
import rfl.bson;
#endif
#if REFLECTCPP_MODULE_TEST_CAPNPROTO
import rfl.capnproto;
#endif
#if REFLECTCPP_MODULE_TEST_CBOR
import rfl.cbor;
#endif
#if REFLECTCPP_MODULE_TEST_CEREAL
import rfl.cereal;
#endif
#if REFLECTCPP_MODULE_TEST_CSV
import rfl.csv;
#endif
#if REFLECTCPP_MODULE_TEST_FLEXBUFFERS
import rfl.flexbuf;
#endif
#if REFLECTCPP_MODULE_TEST_MSGPACK
import rfl.msgpack;
#endif
#if REFLECTCPP_MODULE_TEST_PARQUET
import rfl.parquet;
#endif
#if REFLECTCPP_MODULE_TEST_TOML
import rfl.toml;
#endif
#if REFLECTCPP_MODULE_TEST_UBJSON
import rfl.ubjson;
#endif
#if REFLECTCPP_MODULE_TEST_XML
import rfl.xml;
#endif
#if REFLECTCPP_MODULE_TEST_YAML
import rfl.yaml;
#endif
#if REFLECTCPP_MODULE_TEST_YAS
import rfl.yas;
#endif

namespace {

struct Person {
  std::string first_name;
  int age;
};

struct Config {
  std::string host_name;
  int port;
};

bool is_person(const Person& person, const std::string& first_name,
               const int age) {
  return person.first_name == first_name && person.age == age;
}

int test_core_module() {
  const auto field = rfl::make_field<"first_name">(std::string("Marge"));
  const auto named_tuple = field * rfl::make_field<"age">(42);
  const rfl::Result<int> result = rfl::get<"age">(named_tuple);

  if (!result) {
    return 10;
  }
  if (*result != 42) {
    return 11;
  }
  if (rfl::get<"first_name">(named_tuple) != "Marge") {
    return 12;
  }
  return 0;
}

int test_json_module() {
  const auto json = rfl::json::write(Person{.first_name = "Homer", .age = 45});
  const auto person = rfl::json::read<Person>(json);

  if (!person) {
    return 20;
  }
  if (!is_person(*person, "Homer", 45)) {
    return 21;
  }
  return 0;
}

int test_generic_module() {
  const auto generic =
      rfl::generic::write(Person{.first_name = "Lisa", .age = 8});
  const auto person = rfl::generic::read<Person>(generic);

  if (!person) {
    return 30;
  }
  if (!is_person(*person, "Lisa", 8)) {
    return 31;
  }
  return 0;
}

int test_cli_module() {
  char arg0[] = "reflect-cpp";
  char arg1[] = "--host-name=springfield";
  char arg2[] = "--port=742";
  char* argv[] = {arg0, arg1, arg2};

  const auto config = rfl::cli::read<Config>(3, argv);
  if (!config) {
    return 40;
  }
  if (config->host_name != "springfield") {
    return 41;
  }
  if (config->port != 742) {
    return 42;
  }
  return 0;
}

#if REFLECTCPP_MODULE_TEST_AVRO
int test_avro_module() {
  const auto bytes = rfl::avro::write(Person{.first_name = "Apu", .age = 36});
  const auto person = rfl::avro::read<Person>(bytes);
  return person && is_person(*person, "Apu", 36) ? 0 : 50;
}
#endif

#if REFLECTCPP_MODULE_TEST_BOOST_SERIALIZATION
int test_boost_serialization_module() {
  const auto bytes = rfl::boost_serialization::write(
      Person{.first_name = "Barney", .age = 40});
  const auto person = rfl::boost_serialization::read<Person>(bytes);
  return person && is_person(*person, "Barney", 40) ? 0 : 60;
}
#endif

#if REFLECTCPP_MODULE_TEST_BSON
int test_bson_module() {
  const auto bytes = rfl::bson::write(Person{.first_name = "Moe", .age = 48});
  const auto person = rfl::bson::read<Person>(bytes);
  return person && is_person(*person, "Moe", 48) ? 0 : 70;
}
#endif

#if REFLECTCPP_MODULE_TEST_CAPNPROTO
int test_capnproto_module() {
  const auto bytes =
      rfl::capnproto::write(Person{.first_name = "Patty", .age = 44});
  const auto person = rfl::capnproto::read<Person>(bytes);
  return person && is_person(*person, "Patty", 44) ? 0 : 80;
}
#endif

#if REFLECTCPP_MODULE_TEST_CBOR
int test_cbor_module() {
  const auto bytes = rfl::cbor::write(Person{.first_name = "Selma", .age = 44});
  const auto person = rfl::cbor::read<Person>(bytes);
  return person && is_person(*person, "Selma", 44) ? 0 : 90;
}
#endif

#if REFLECTCPP_MODULE_TEST_CEREAL
int test_cereal_module() {
  const auto bytes = rfl::cereal::write(Person{.first_name = "Ned", .age = 60});
  const auto person = rfl::cereal::read<Person>(bytes);
  return person && is_person(*person, "Ned", 60) ? 0 : 100;
}
#endif

#if REFLECTCPP_MODULE_TEST_CSV
int test_csv_module() {
  const auto csv = rfl::csv::write(std::vector<Person>{{"Lenny", 38}});
  const auto people = rfl::csv::read<std::vector<Person>>(csv);
  return people && people->size() == 1 &&
                 is_person(people->front(), "Lenny", 38)
             ? 0
             : 110;
}
#endif

#if REFLECTCPP_MODULE_TEST_FLEXBUFFERS
int test_flexbuf_module() {
  const auto bytes =
      rfl::flexbuf::write(Person{.first_name = "Carl", .age = 39});
  const auto person = rfl::flexbuf::read<Person>(bytes);
  return person && is_person(*person, "Carl", 39) ? 0 : 120;
}
#endif

#if REFLECTCPP_MODULE_TEST_MSGPACK
int test_msgpack_module() {
  const auto bytes =
      rfl::msgpack::write(Person{.first_name = "Milhouse", .age = 10});
  const auto person = rfl::msgpack::read<Person>(bytes);
  return person && is_person(*person, "Milhouse", 10) ? 0 : 130;
}
#endif

#if REFLECTCPP_MODULE_TEST_PARQUET
int test_parquet_module() {
  const auto bytes = rfl::parquet::write(std::vector<Person>{{"Ralph", 8}});
  const auto people = rfl::parquet::read<std::vector<Person>>(bytes);
  return people && people->size() == 1 && is_person(people->front(), "Ralph", 8)
             ? 0
             : 140;
}
#endif

#if REFLECTCPP_MODULE_TEST_TOML
int test_toml_module() {
  const auto toml =
      rfl::toml::write(Person{.first_name = "Skinner", .age = 45});
  const auto person = rfl::toml::read<Person>(toml);
  return person && is_person(*person, "Skinner", 45) ? 0 : 150;
}
#endif

#if REFLECTCPP_MODULE_TEST_UBJSON
int test_ubjson_module() {
  const auto bytes =
      rfl::ubjson::write(Person{.first_name = "Nelson", .age = 12});
  const auto person = rfl::ubjson::read<Person>(bytes);
  return person && is_person(*person, "Nelson", 12) ? 0 : 160;
}
#endif

#if REFLECTCPP_MODULE_TEST_XML
int test_xml_module() {
  const auto xml = rfl::xml::write(Person{.first_name = "Edna", .age = 41});
  const auto person = rfl::xml::read<Person>(xml);
  return person && is_person(*person, "Edna", 41) ? 0 : 170;
}
#endif

#if REFLECTCPP_MODULE_TEST_YAML
int test_yaml_module() {
  const auto yaml = rfl::yaml::write(Person{.first_name = "Agnes", .age = 75});
  const auto person = rfl::yaml::read<Person>(yaml);
  return person && is_person(*person, "Agnes", 75) ? 0 : 180;
}
#endif

#if REFLECTCPP_MODULE_TEST_YAS
int test_yas_module() {
  const auto bytes = rfl::yas::write(Person{.first_name = "Otto", .age = 29});
  const auto person = rfl::yas::read<Person>(bytes);
  return person && is_person(*person, "Otto", 29) ? 0 : 190;
}
#endif

}  // namespace

int main() {
  if (const auto rc = test_core_module(); rc != 0) {
    return rc;
  }
  if (const auto rc = test_json_module(); rc != 0) {
    return rc;
  }
  if (const auto rc = test_generic_module(); rc != 0) {
    return rc;
  }
  if (const auto rc = test_cli_module(); rc != 0) {
    return rc;
  }
#if REFLECTCPP_MODULE_TEST_AVRO
  if (const auto rc = test_avro_module(); rc != 0) {
    return rc;
  }
#endif
#if REFLECTCPP_MODULE_TEST_BOOST_SERIALIZATION
  if (const auto rc = test_boost_serialization_module(); rc != 0) {
    return rc;
  }
#endif
#if REFLECTCPP_MODULE_TEST_BSON
  if (const auto rc = test_bson_module(); rc != 0) {
    return rc;
  }
#endif
#if REFLECTCPP_MODULE_TEST_CAPNPROTO
  if (const auto rc = test_capnproto_module(); rc != 0) {
    return rc;
  }
#endif
#if REFLECTCPP_MODULE_TEST_CBOR
  if (const auto rc = test_cbor_module(); rc != 0) {
    return rc;
  }
#endif
#if REFLECTCPP_MODULE_TEST_CEREAL
  if (const auto rc = test_cereal_module(); rc != 0) {
    return rc;
  }
#endif
#if REFLECTCPP_MODULE_TEST_CSV
  if (const auto rc = test_csv_module(); rc != 0) {
    return rc;
  }
#endif
#if REFLECTCPP_MODULE_TEST_FLEXBUFFERS
  if (const auto rc = test_flexbuf_module(); rc != 0) {
    return rc;
  }
#endif
#if REFLECTCPP_MODULE_TEST_MSGPACK
  if (const auto rc = test_msgpack_module(); rc != 0) {
    return rc;
  }
#endif
#if REFLECTCPP_MODULE_TEST_PARQUET
  if (const auto rc = test_parquet_module(); rc != 0) {
    return rc;
  }
#endif
#if REFLECTCPP_MODULE_TEST_TOML
  if (const auto rc = test_toml_module(); rc != 0) {
    return rc;
  }
#endif
#if REFLECTCPP_MODULE_TEST_UBJSON
  if (const auto rc = test_ubjson_module(); rc != 0) {
    return rc;
  }
#endif
#if REFLECTCPP_MODULE_TEST_XML
  if (const auto rc = test_xml_module(); rc != 0) {
    return rc;
  }
#endif
#if REFLECTCPP_MODULE_TEST_YAML
  if (const auto rc = test_yaml_module(); rc != 0) {
    return rc;
  }
#endif
#if REFLECTCPP_MODULE_TEST_YAS
  if (const auto rc = test_yas_module(); rc != 0) {
    return rc;
  }
#endif
  return 0;
}
