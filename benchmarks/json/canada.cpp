#include <benchmark/benchmark.h>
#include <rapidjson/document.h>
#include <simdjson.h>

#include <array>
#include <iostream>
#include <nlohmann/json.hpp>
#include <optional>
#include <rfl/json.hpp>
#include <type_traits>
#include <vector>

namespace canada {

// ----------------------------------------------------------------------------

struct Property {
  std::string name;
};

struct Geometry {
  rfl::Literal<"Polygon"> type;
  std::vector<std::vector<std::tuple<double, double>>> coordinates;
};

struct Feature {
  rfl::Literal<"Feature"> type;
  Property properties;
  Geometry geometry;
};

struct FeatureCollection {
  rfl::Literal<"FeatureCollection"> type;
  std::vector<Feature> features;
};

// ----------------------------------------------------------------------------

static std::string load_data() {
  return rfl::io::load_string("benchmarks/json/data/canada.json").value();
}

// ----------------------------------------------------------------------------
// nlohmann/json

std::vector<std::vector<std::tuple<double, double>>> nlohmann_to_coordinates(
    const nlohmann::json &_val);

Property nlohmann_to_property(const nlohmann::json &_val);

Geometry nlohmann_to_geometry(const nlohmann::json &_val);

Feature nlohmann_to_feature(const nlohmann::json &_val);

FeatureCollection nlohmann_to_feature_collection(const nlohmann::json &_val);

std::vector<std::vector<std::tuple<double, double>>> nlohmann_to_coordinates(
    const nlohmann::json &_val) {
  std::vector<std::vector<std::tuple<double, double>>> coordinates;
  for (const auto &arr1 : _val) {
    std::vector<std::tuple<double, double>> vec;
    for (const auto &arr2 : arr1) {
      std::tuple<double, double> tup;
      std::get<0>(tup) = arr2[0].template get<double>();
      std::get<1>(tup) = arr2[1].template get<double>();
      vec.emplace_back(std::move(tup));
    }
    coordinates.emplace_back(std::move(vec));
  }
  return coordinates;
}

Property nlohmann_to_property(const nlohmann::json &_val) {
  Property property;
  property.name = _val["name"].template get<std::string>();
  return property;
}

Geometry nlohmann_to_geometry(const nlohmann::json &_val) {
  Geometry geometry;
  geometry.type = _val["type"].template get<std::string>();
  geometry.coordinates = nlohmann_to_coordinates(_val["coordinates"]);
  return geometry;
}

Feature nlohmann_to_feature(const nlohmann::json &_val) {
  Feature feature;
  feature.type = _val["type"].template get<std::string>();
  feature.properties = nlohmann_to_property(_val["properties"]);
  feature.geometry = nlohmann_to_geometry(_val["geometry"]);
  return feature;
}

FeatureCollection nlohmann_to_feature_collection(const nlohmann::json &_val) {
  FeatureCollection feature_collection;
  feature_collection.type = _val["type"].template get<std::string>();
  for (const auto &val : _val["features"]) {
    feature_collection.features.push_back(nlohmann_to_feature(val));
  }
  return feature_collection;
}

static rfl::Result<FeatureCollection> read_using_nlohmann(
    const std::string &_json_string) {
  try {
    auto val = nlohmann::json::parse(_json_string);
    return nlohmann_to_feature_collection(val);
  } catch (std::exception &e) {
    return rfl::Error(e.what());
  }
}

// ----------------------------------------------------------------------------
// rapidjson

std::vector<std::vector<std::tuple<double, double>>> rapidjson_to_coordinates(
    const rapidjson::Value &_arr);

Property rapidjson_to_property(const rapidjson::Value &_val);

Geometry rapidjson_to_geometry(const rapidjson::Value &_val);

Feature rapidjson_to_feature(const rapidjson::Value &_val);

FeatureCollection rapidjson_to_feature_collection(const rapidjson::Value &_val);

std::vector<std::vector<std::tuple<double, double>>> rapidjson_to_coordinates(
    const rapidjson::Value &_arr) {
  std::vector<std::vector<std::tuple<double, double>>> coordinates;
  for (auto it1 = _arr.Begin(); it1 != _arr.End(); ++it1) {
    auto arr1 = it1->GetArray();
    std::vector<std::tuple<double, double>> vec;
    for (auto it2 = arr1.Begin(); it2 != arr1.End(); ++it2) {
      auto arr2 = it2->GetArray();
      std::tuple<double, double> tup;
      std::get<0>(tup) = arr2[0].GetDouble();
      std::get<1>(tup) = arr2[1].GetDouble();
      vec.emplace_back(std::move(tup));
    }
    coordinates.emplace_back(std::move(vec));
  }
  return coordinates;
}

Property rapidjson_to_property(const rapidjson::Value &_val) {
  Property property;
  property.name = _val["name"].GetString();
  return property;
}

Geometry rapidjson_to_geometry(const rapidjson::Value &_val) {
  Geometry geometry;
  geometry.type = _val["type"].GetString();
  geometry.coordinates =
      rapidjson_to_coordinates(_val["coordinates"].GetArray());
  return geometry;
}

Feature rapidjson_to_feature(const rapidjson::Value &_val) {
  Feature feature;
  feature.type = _val["type"].GetString();
  feature.properties = rapidjson_to_property(_val["properties"].GetObject());
  feature.geometry = rapidjson_to_geometry(_val["geometry"].GetObject());
  return feature;
}

FeatureCollection rapidjson_to_feature_collection(
    const rapidjson::Value &_val) {
  FeatureCollection feature_collection;
  feature_collection.type = _val["type"].GetString();
  auto arr = _val["features"].GetArray();
  for (auto it = arr.Begin(); it != arr.End(); ++it) {
    feature_collection.features.push_back(
        rapidjson_to_feature(it->GetObject()));
  }
  return feature_collection;
}

static rfl::Result<FeatureCollection> read_using_rapidjson(
    const std::string &_json_string) {
  try {
    rapidjson::Document d;
    d.Parse(_json_string.c_str());
    return rapidjson_to_feature_collection(d.GetObject());
  } catch (std::exception &e) {
    return rfl::Error(e.what());
  }
}

// ----------------------------------------------------------------------------
// simdjson

std::vector<std::vector<std::tuple<double, double>>> simdjson_to_coordinates(
    simdjson::ondemand::array _val);

Property simdjson_to_property(simdjson::ondemand::object _val);

Geometry simdjson_to_geometry(simdjson::ondemand::object _val);

Feature simdjson_to_feature(simdjson::ondemand::object _val);

FeatureCollection simdjson_to_feature_collection(
    simdjson::ondemand::object _val);

std::vector<std::vector<std::tuple<double, double>>> simdjson_to_coordinates(
    simdjson::ondemand::array _val) {
  std::vector<std::vector<std::tuple<double, double>>> coordinates;
  for (auto arr1 : _val) {
    std::vector<std::tuple<double, double>> vec;
    for (auto val2 : arr1.get_array()) {
      auto arr2 = val2.get_array();
      std::tuple<double, double> tup;
      std::get<0>(tup) = arr2.at(0).get_double();
      std::get<1>(tup) = arr2.at(1).get_double();
      vec.emplace_back(std::move(tup));
    }
    coordinates.emplace_back(std::move(vec));
  }
  return coordinates;
}

Property simdjson_to_property(simdjson::ondemand::object _val) {
  Property property;
  property.name = _val["name"].get_string().value();
  return property;
}

Geometry simdjson_to_geometry(simdjson::ondemand::object _val) {
  Geometry geometry;
  geometry.type = std::string(_val["type"].get_string().value());
  geometry.coordinates =
      simdjson_to_coordinates(_val["coordinates"].get_array());
  return geometry;
}

Feature simdjson_to_feature(simdjson::ondemand::object _val) {
  Feature feature;
  feature.type = std::string(_val["type"].get_string().value());
  feature.properties = simdjson_to_property(_val["properties"].get_object());
  feature.geometry = simdjson_to_geometry(_val["geometry"].get_object());
  return feature;
}

FeatureCollection simdjson_to_feature_collection(
    simdjson::ondemand::object _val) {
  FeatureCollection feature_collection;
  feature_collection.type = std::string(_val["type"].get_string().value());
  for (auto val : _val["features"].get_array()) {
    feature_collection.features.push_back(simdjson_to_feature(val));
  }
  return feature_collection;
}

static rfl::Result<FeatureCollection> read_using_simdjson(
    const std::string &_json_string) {
  try {
    simdjson::ondemand::parser parser;
    auto padded_str = simdjson::padded_string(_json_string);
    auto doc = parser.iterate(padded_str).value();
    return simdjson_to_feature_collection(doc.get_object());
  } catch (std::exception &e) {
    return rfl::Error(e.what());
  }
}

// ----------------------------------------------------------------------------
// yyjson
/*
rfl::Result<std::vector<Person>> yyjson_to_children(yyjson_val *_val);

rfl::Result<Person> yyjson_to_person(yyjson_val *_val);

rfl::Result<std::vector<Person>> yyjson_to_children(yyjson_val *_val) {
  std::vector<Person> children;
  yyjson_val *val;
  yyjson_arr_iter iter;
  yyjson_arr_iter_init(_val, &iter);
  while ((val = yyjson_arr_iter_next(&iter))) {
    auto r = yyjson_to_person(val);
    if (!r) {
      return *r.error();
    }
    children.emplace_back(std::move(*r));
  }
  return children;
}

rfl::Result<Person> yyjson_to_person(yyjson_val *_val) {
  Person person;

  std::vector<rfl::Error> errors;
  std::array<bool, 3> found;
  std::fill(found.begin(), found.end(), false);

  yyjson_obj_iter iter;
  yyjson_obj_iter_init(_val, &iter);
  yyjson_val *key;
  while ((key = yyjson_obj_iter_next(&iter))) {
    const auto name = std::string_view(yyjson_get_str(key));
    const auto v = yyjson_obj_iter_get_val(key);
    if (!std::get<0>(found) && name == "first_name") {
      auto first_name = yyjson_get_str(v);
      if (first_name == NULL) {
        errors.push_back(rfl::Error(
            "Error reading 'first_name': Could not cast to string."));
        continue;
      }
      person.first_name = first_name;
      std::get<0>(found) = true;
    } else if (!std::get<1>(found) && name == "last_name") {
      auto last_name = yyjson_get_str(v);
      if (last_name == NULL) {
        errors.push_back(
            rfl::Error("Error reading 'last_name': Could not cast to string."));
        continue;
      }
      person.last_name = last_name;
      std::get<1>(found) = true;
    } else if (!std::get<2>(found) && name == "children") {
      auto children = yyjson_to_children(v);
      if (!children) {
        errors.push_back(rfl::Error("Error reading 'children': " +
                                    children.error()->what()));
        continue;
      }
      person.children = std::move(*children);
      std::get<2>(found) = true;
    }
  }
  if (!std::get<0>(found)) {
    errors.push_back(rfl::Error("'first_name' not found"));
  }
  if (!std::get<1>(found)) {
    errors.push_back(rfl::Error("'last_name' not found"));
  }
  if (!std::get<2>(found)) {
    errors.push_back(rfl::Error("'children' not found"));
  }
  if (errors.size() != 0) {
    std::cout << "Some errors occurred:" << std::endl;
    for (const auto &err : errors) {
      std::cout << err.what() << std::endl;
    }
    return errors[0];
  }
  return person;
}

static rfl::Result<Person> read_using_yyjson() {
  yyjson_doc *doc = yyjson_read(json_string.c_str(), json_string.size(), 0);
  if (!doc) {
    std::cout << "Could not parse document!" << std::endl;
    return rfl::Error("Could not parse document");
  }
  yyjson_val *root = yyjson_doc_get_root(doc);
  auto person = yyjson_to_person(root);
  yyjson_doc_free(doc);
  return person;
}*/

// ----------------------------------------------------------------------------

static void BM_canada_nlohmann(benchmark::State &state) {
  const auto json_string = load_data();
  for (auto _ : state) {
    const auto res = read_using_nlohmann(json_string);
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_canada_nlohmann);

static void BM_canada_rapidjson(benchmark::State &state) {
  const auto json_string = load_data();
  for (auto _ : state) {
    const auto res = read_using_rapidjson(json_string);
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_canada_rapidjson);

static void BM_canada_simdjson(benchmark::State &state) {
  const auto json_string = load_data();
  for (auto _ : state) {
    const auto res = read_using_simdjson(json_string);
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_canada_simdjson);
/*
static void BM_simple_read_yyjson(benchmark::State &state) {
  for (auto _ : state) {
    const auto res = read_using_yyjson();
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_simple_read_yyjson);
*/

static void BM_canada_reflect_cpp(benchmark::State &state) {
  const auto json_string = load_data();
  for (auto _ : state) {
    const auto res = rfl::json::read<FeatureCollection>(json_string);
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_canada_reflect_cpp);

// ----------------------------------------------------------------------------

}  // namespace canada

