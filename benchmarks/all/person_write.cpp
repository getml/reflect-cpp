#include <benchmark/benchmark.h>

#include <array>
#include <iostream>
#include <optional>
#include <rfl/avro.hpp>
#include <rfl/bson.hpp>
#include <rfl/capnproto.hpp>
#include <rfl/cbor.hpp>
#include <rfl/flexbuf.hpp>
#include <rfl/json.hpp>
#include <rfl/msgpack.hpp>
#include <rfl/toml.hpp>
#include <rfl/ubjson.hpp>
#include <rfl/xml.hpp>
#include <rfl/yaml.hpp>
#include <type_traits>
#include <vector>

namespace person_write {

// ----------------------------------------------------------------------------

struct Person {
  std::string first_name;
  std::string last_name = "Simpson";
  int age;
  std::string town = "Springfield";
  std::vector<Person> children;
};

// ----------------------------------------------------------------------------

static Person load_data() {
  const auto bart = Person{.first_name = "Bart", .age = 10};

  const auto lisa = Person{.first_name = "Lisa", .age = 8};

  const auto maggie = Person{.first_name = "Maggie", .age = 0};

  return Person{.first_name = "Homer",
                .age = 45,
                .children = std::vector<Person>({bart, lisa, maggie})};
}

// ----------------------------------------------------------------------------

static void BM_person_write_reflect_cpp_avro(benchmark::State &state) {
  const auto schema = rfl::avro::to_schema<Person>();
  const auto data = load_data();
  for (auto _ : state) {
    const auto output = rfl::avro::write(data, schema);
    if (output.size() == 0) {
      std::cout << "No output" << std::endl;
    }
  }
}
BENCHMARK(BM_person_write_reflect_cpp_avro);

static void BM_person_write_reflect_cpp_bson(benchmark::State &state) {
  const auto data = load_data();
  for (auto _ : state) {
    const auto output = rfl::bson::write(data);
    if (output.size() == 0) {
      std::cout << "No output" << std::endl;
    }
  }
}
BENCHMARK(BM_person_write_reflect_cpp_bson);

static void BM_person_write_reflect_cpp_capnproto(benchmark::State &state) {
  const auto schema = rfl::capnproto::to_schema<Person>();
  const auto data = load_data();
  for (auto _ : state) {
    const auto output = rfl::capnproto::write(data, schema);
    if (output.size() == 0) {
      std::cout << "No output" << std::endl;
    }
  }
}
BENCHMARK(BM_person_write_reflect_cpp_capnproto);

static void BM_person_write_reflect_cpp_cbor(benchmark::State &state) {
  const auto data = load_data();
  for (auto _ : state) {
    const auto output = rfl::cbor::write(data);
    if (output.size() == 0) {
      std::cout << "No output" << std::endl;
    }
  }
}
BENCHMARK(BM_person_write_reflect_cpp_cbor);

static void BM_person_write_reflect_cpp_cbor_without_field_names(
    benchmark::State &state) {
  const auto data = load_data();
  for (auto _ : state) {
    const auto output = rfl::cbor::write<rfl::NoFieldNames>(data);
    if (output.size() == 0) {
      std::cout << "No output" << std::endl;
    }
  }
}
BENCHMARK(BM_person_write_reflect_cpp_cbor_without_field_names);

static void BM_person_write_reflect_cpp_flexbuf(benchmark::State &state) {
  const auto data = load_data();
  for (auto _ : state) {
    const auto output = rfl::flexbuf::write(data);
    if (output.size() == 0) {
      std::cout << "No output" << std::endl;
    }
  }
}
BENCHMARK(BM_person_write_reflect_cpp_flexbuf);

static void BM_person_write_reflect_cpp_flexbuf_without_field_names(
    benchmark::State &state) {
  const auto data = load_data();
  for (auto _ : state) {
    const auto output = rfl::flexbuf::write<rfl::NoFieldNames>(data);
    if (output.size() == 0) {
      std::cout << "No output" << std::endl;
    }
  }
}
BENCHMARK(BM_person_write_reflect_cpp_flexbuf_without_field_names);

static void BM_person_write_reflect_cpp_json(benchmark::State &state) {
  const auto data = load_data();
  for (auto _ : state) {
    const auto output = rfl::json::write(data);
    if (output.size() == 0) {
      std::cout << "No output" << std::endl;
    }
  }
}
BENCHMARK(BM_person_write_reflect_cpp_json);

static void BM_person_write_reflect_cpp_json_without_field_names(
    benchmark::State &state) {
  const auto data = load_data();
  for (auto _ : state) {
    const auto output = rfl::json::write<rfl::NoFieldNames>(data);
    if (output.size() == 0) {
      std::cout << "No output" << std::endl;
    }
  }
}
BENCHMARK(BM_person_write_reflect_cpp_json_without_field_names);

static void BM_person_write_reflect_cpp_msgpack(benchmark::State &state) {
  const auto data = load_data();
  for (auto _ : state) {
    const auto output = rfl::msgpack::write(data);
    if (output.size() == 0) {
      std::cout << "No output" << std::endl;
    }
  }
}
BENCHMARK(BM_person_write_reflect_cpp_msgpack);

static void BM_person_write_reflect_cpp_msgpack_without_field_names(
    benchmark::State &state) {
  const auto data = load_data();
  for (auto _ : state) {
    const auto output = rfl::msgpack::write<rfl::NoFieldNames>(data);
    if (output.size() == 0) {
      std::cout << "No output" << std::endl;
    }
  }
}
BENCHMARK(BM_person_write_reflect_cpp_msgpack_without_field_names);

static void BM_person_write_reflect_cpp_toml(benchmark::State &state) {
  const auto data = load_data();
  for (auto _ : state) {
    const auto output = rfl::toml::write(data);
    if (output.size() == 0) {
      std::cout << "No output" << std::endl;
    }
  }
}
BENCHMARK(BM_person_write_reflect_cpp_toml);

static void BM_person_write_reflect_cpp_ubjson(benchmark::State &state) {
  const auto data = load_data();
  for (auto _ : state) {
    const auto output = rfl::ubjson::write(data);
    if (output.size() == 0) {
      std::cout << "No output" << std::endl;
    }
  }
}
BENCHMARK(BM_person_write_reflect_cpp_ubjson);

static void BM_person_write_reflect_cpp_ubjson_without_field_names(
    benchmark::State &state) {
  const auto data = load_data();
  for (auto _ : state) {
    const auto output = rfl::ubjson::write<rfl::NoFieldNames>(data);
    if (output.size() == 0) {
      std::cout << "No output" << std::endl;
    }
  }
}
BENCHMARK(BM_person_write_reflect_cpp_ubjson_without_field_names);

static void BM_person_write_reflect_cpp_xml(benchmark::State &state) {
  const auto data = load_data();
  for (auto _ : state) {
    const auto output = rfl::xml::write(data);
    if (output.size() == 0) {
      std::cout << "No output" << std::endl;
    }
  }
}
BENCHMARK(BM_person_write_reflect_cpp_xml);

static void BM_person_write_reflect_cpp_yaml(benchmark::State &state) {
  const auto data = load_data();
  for (auto _ : state) {
    const auto output = rfl::yaml::write(data);
    if (output.size() == 0) {
      std::cout << "No output" << std::endl;
    }
  }
}
BENCHMARK(BM_person_write_reflect_cpp_yaml);

// ----------------------------------------------------------------------------

}  // namespace person_write

