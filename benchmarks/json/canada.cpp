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
  return rfl::io::load_string("benchmarks/data/canada.json").value();
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

simdjson_inline std::vector<std::vector<std::tuple<double, double>>> simdjson_to_coordinates(
    simdjson::ondemand::array _val);

simdjson_inline Property simdjson_to_property(simdjson::ondemand::object _val);

simdjson_inline Geometry simdjson_to_geometry(simdjson::ondemand::object _val);

simdjson_inline Feature simdjson_to_feature(simdjson::ondemand::object _val);

simdjson_inline FeatureCollection simdjson_to_feature_collection(
    simdjson::ondemand::object _val);

simdjson_inline std::vector<std::vector<std::tuple<double, double>>> simdjson_to_coordinates(
    simdjson::ondemand::array _val) {
  std::vector<std::vector<std::tuple<double, double>>> coordinates;
  for (auto arr1 : _val) {
    std::vector<std::tuple<double, double>> vec;
    for (auto val2 : arr1) {
      // Instead of indexing x = val2[0] and y = val2[1], we iterate through the two values.
      auto coord = val2.begin();
      double x = *coord;
      ++coord;
      double y = *coord;
      vec.emplace_back(x, y);
    }
    coordinates.emplace_back(std::move(vec));
  }
  return coordinates;
}

simdjson_inline std::string simdjson_to_string(simdjson::ondemand::value _val) {
  return std::string(std::string_view(_val));
}

simdjson_inline Property simdjson_to_property(simdjson::ondemand::object _val) {
  Property property;
  property.name = std::string_view(_val["name"]);
  return property;
}

simdjson_inline Geometry simdjson_to_geometry(simdjson::ondemand::object _val) {
  Geometry geometry;
  geometry.type = simdjson_to_string(_val["type"]);
  geometry.coordinates = simdjson_to_coordinates(_val["coordinates"]);
  return geometry;
}

simdjson_inline Feature simdjson_to_feature(simdjson::ondemand::object _val) {
  Feature feature;
  feature.type = simdjson_to_string(_val["type"]);
  feature.properties = simdjson_to_property(_val["properties"]);
  feature.geometry = simdjson_to_geometry(_val["geometry"]);
  return feature;
}

simdjson_inline FeatureCollection simdjson_to_feature_collection(
    simdjson::ondemand::object _val) {
  FeatureCollection feature_collection;
  feature_collection.type = simdjson_to_string(_val["type"]);
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
    auto doc = parser.iterate(padded_str);
    return simdjson_to_feature_collection(doc);
  } catch (std::exception &e) {
    return rfl::Error(e.what());
  }
}

// ----------------------------------------------------------------------------
// yyjson

std::vector<std::vector<std::tuple<double, double>>> yyjson_to_coordinates(
    yyjson_val *_val);

Property yyjson_to_property(yyjson_val *_val);

Geometry yyjson_to_geometry(yyjson_val *_val);

Feature yyjson_to_feature(yyjson_val *_val);

FeatureCollection yyjson_to_feature_collection(yyjson_val *_val);

std::vector<std::vector<std::tuple<double, double>>> yyjson_to_coordinates(
    yyjson_val *_val) {
  std::vector<std::vector<std::tuple<double, double>>> coordinates;
  yyjson_val *v1;
  yyjson_arr_iter iter1;
  yyjson_arr_iter_init(_val, &iter1);
  while ((v1 = yyjson_arr_iter_next(&iter1))) {
    std::vector<std::tuple<double, double>> vec;
    yyjson_val *arr2;
    yyjson_arr_iter iter2;
    yyjson_arr_iter_init(v1, &iter2);
    while ((arr2 = yyjson_arr_iter_next(&iter2))) {
      std::tuple<double, double> tup;
      std::get<0>(tup) =
          static_cast<double>(yyjson_get_num(yyjson_arr_get(arr2, 0)));
      std::get<1>(tup) =
          static_cast<double>(yyjson_get_num(yyjson_arr_get(arr2, 1)));
      vec.emplace_back(std::move(tup));
    }
    coordinates.emplace_back(std::move(vec));
  }
  return coordinates;
}

Property yyjson_to_property(yyjson_val *_val) {
  Property property;
  property.name = yyjson_get_str(yyjson_obj_get(_val, "name"));
  return property;
}

Geometry yyjson_to_geometry(yyjson_val *_val) {
  Geometry geometry;
  geometry.type = std::string(yyjson_get_str(yyjson_obj_get(_val, "type")));
  geometry.coordinates =
      yyjson_to_coordinates(yyjson_obj_get(_val, "coordinates"));
  return geometry;
}

Feature yyjson_to_feature(yyjson_val *_val) {
  Feature feature;
  feature.type = std::string(yyjson_get_str(yyjson_obj_get(_val, "type")));
  feature.properties = yyjson_to_property(yyjson_obj_get(_val, "properties"));
  feature.geometry = yyjson_to_geometry(yyjson_obj_get(_val, "geometry"));
  return feature;
}

FeatureCollection yyjson_to_feature_collection(yyjson_val *_val) {
  FeatureCollection feature_collection;
  feature_collection.type =
      std::string(yyjson_get_str(yyjson_obj_get(_val, "type")));
  auto features = yyjson_obj_get(_val, "features");
  yyjson_val *feat;
  yyjson_arr_iter iter;
  yyjson_arr_iter_init(features, &iter);
  while ((feat = yyjson_arr_iter_next(&iter))) {
    feature_collection.features.push_back(yyjson_to_feature(feat));
  }
  return feature_collection;
}

static rfl::Result<FeatureCollection> read_using_yyjson(
    const std::string &_json_string) {
  yyjson_doc *doc = yyjson_read(_json_string.c_str(), _json_string.size(), 0);
  if (!doc) {
    std::cout << "Could not parse document!" << std::endl;
    return rfl::Error("Could not parse document");
  }
  yyjson_val *root = yyjson_doc_get_root(doc);
  auto person = yyjson_to_feature_collection(root);
  yyjson_doc_free(doc);
  return person;

  try {
    auto val = nlohmann::json::parse(_json_string);
    return nlohmann_to_feature_collection(val);
  } catch (std::exception &e) {
    return rfl::Error(e.what());
  }
}

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

#ifndef __clang__

// simdjson has a bug in clang.
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

#endif

static void BM_canada_yyjson(benchmark::State &state) {
  const auto json_string = load_data();
  for (auto _ : state) {
    const auto res = read_using_yyjson(json_string);
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_canada_yyjson);

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

