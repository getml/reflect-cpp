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

namespace licenses {

// ----------------------------------------------------------------------------

struct Identifier {
  std::string identifier;
  std::string scheme;
};

struct Link {
  std::string note;
  std::string url;
};

struct OtherName {
  std::string name;
  std::optional<std::string> note;
};

struct Text {
  std::string media_type;
  std::string title;
  std::string url;
};

struct License {
  std::string id;
  std::vector<Identifier> identifiers;
  std::vector<Link> links;
  std::string name;
  std::vector<OtherName> other_names;
  std::optional<std::string> superseded_by;
  std::vector<std::string> keywords;
  std::vector<Text> text;
};

struct Licenses {
  std::vector<License> licenses;
};

// ----------------------------------------------------------------------------

static std::string load_data() {
  return rfl::io::load_string("benchmarks/data/licenses.json").value();
}

// ----------------------------------------------------------------------------
// nlohmann/json

Identifier nlohmann_to_identifier(const nlohmann::json &_val);

Link nlohmann_to_link(const nlohmann::json &_val);

OtherName nlohmann_to_other_name(const nlohmann::json &_val);

Text nlohmann_to_text(const nlohmann::json &_val);

License nlohmann_to_license(const nlohmann::json &_val);

Licenses nlohmann_to_licenses(const nlohmann::json &_val);

Identifier nlohmann_to_identifier(const nlohmann::json &_val) {
  Identifier identifier;
  identifier.identifier = _val["identifier"].template get<std::string>();
  identifier.scheme = _val["scheme"].template get<std::string>();
  return identifier;
}

Link nlohmann_to_link(const nlohmann::json &_val) {
  Link link;
  link.note = _val["note"].template get<std::string>();
  link.url = _val["url"].template get<std::string>();
  return link;
}

OtherName nlohmann_to_other_name(const nlohmann::json &_val) {
  OtherName other_name;
  other_name.name = _val["name"].template get<std::string>();
  if (!_val["note"].is_null()) {
    other_name.note = _val["note"].template get<std::string>();
  }
  return other_name;
}

Text nlohmann_to_text(const nlohmann::json &_val) {
  Text text;
  text.media_type = _val["media_type"].template get<std::string>();
  text.title = _val["title"].template get<std::string>();
  text.url = _val["url"].template get<std::string>();
  return text;
}

License nlohmann_to_license(const nlohmann::json &_val) {
  License license;
  license.id = _val["id"].template get<std::string>();
  for (const auto &v : _val["identifiers"]) {
    license.identifiers.emplace_back(nlohmann_to_identifier(v));
  }
  for (const auto &v : _val["links"]) {
    license.links.emplace_back(nlohmann_to_link(v));
  }
  license.name = _val["name"].template get<std::string>();
  for (const auto &v : _val["other_names"]) {
    license.other_names.emplace_back(nlohmann_to_other_name(v));
  }
  if (!_val["superseded_by"].is_null()) {
    license.superseded_by = _val["superseded_by"].template get<std::string>();
  }
  for (const auto &v : _val["keywords"]) {
    license.keywords.emplace_back(v.template get<std::string>());
  }
  for (const auto &v : _val["text"]) {
    license.text.emplace_back(nlohmann_to_text(v));
  }
  return license;
}

Licenses nlohmann_to_licenses(const nlohmann::json &_val) {
  Licenses licenses;
  for (const auto &v : _val["licenses"]) {
    licenses.licenses.emplace_back(nlohmann_to_license(v));
  }
  return licenses;
}

static rfl::Result<Licenses> read_using_nlohmann(
    const std::string &_json_string) {
  try {
    auto val = nlohmann::json::parse(_json_string);
    return nlohmann_to_licenses(val);
  } catch (std::exception &e) {
    return rfl::Error(e.what());
  }
}

// ----------------------------------------------------------------------------
// rapidjson

Identifier rapidjson_to_identifier(const rapidjson::Value &_val);

Link rapidjson_to_link(const rapidjson::Value &_val);

OtherName rapidjson_to_other_name(const rapidjson::Value &_val);

Text rapidjson_to_text(const rapidjson::Value &_val);

License rapidjson_to_license(const rapidjson::Value &_val);

Licenses rapidjson_to_licenses(const rapidjson::Value &_val);

Identifier rapidjson_to_identifier(const rapidjson::Value &_val) {
  Identifier identifier;
  identifier.identifier = _val["identifier"].GetString();
  identifier.scheme = _val["scheme"].GetString();
  return identifier;
}

Link rapidjson_to_link(const rapidjson::Value &_val) {
  Link link;
  link.note = _val["note"].GetString();
  link.url = _val["url"].GetString();
  return link;
}

OtherName rapidjson_to_other_name(const rapidjson::Value &_val) {
  OtherName other_name;
  other_name.name = _val["name"].GetString();
  if (!_val["note"].IsNull()) {
    other_name.note = _val["note"].GetString();
  }
  return other_name;
}

Text rapidjson_to_text(const rapidjson::Value &_val) {
  Text text;
  text.media_type = _val["media_type"].GetString();
  text.title = _val["title"].GetString();
  text.url = _val["url"].GetString();
  return text;
}

License rapidjson_to_license(const rapidjson::Value &_val) {
  License license;
  license.id = _val["id"].GetString();
  const auto &arr1 = _val["identifiers"];
  for (auto it = arr1.Begin(); it != arr1.End(); ++it) {
    license.identifiers.emplace_back(rapidjson_to_identifier(*it));
  }
  const auto &arr2 = _val["links"];
  for (auto it = arr2.Begin(); it != arr2.End(); ++it) {
    license.links.emplace_back(rapidjson_to_link(*it));
  }
  license.name = _val["name"].GetString();
  const auto &arr3 = _val["other_names"];
  for (auto it = arr3.Begin(); it != arr3.End(); ++it) {
    license.other_names.emplace_back(rapidjson_to_other_name(*it));
  }
  if (!_val["superseded_by"].IsNull()) {
    license.superseded_by = _val["superseded_by"].GetString();
  }
  const auto &arr4 = _val["keywords"];
  for (auto it = arr4.Begin(); it != arr4.End(); ++it) {
    license.keywords.emplace_back(it->GetString());
  }
  const auto &arr5 = _val["text"];
  for (auto it = arr5.Begin(); it != arr5.End(); ++it) {
    license.text.emplace_back(rapidjson_to_text(*it));
  }
  return license;
}

Licenses rapidjson_to_licenses(const rapidjson::Value &_val) {
  Licenses licenses;
  const auto &arr = _val["licenses"];
  for (auto it = arr.Begin(); it != arr.End(); ++it) {
    licenses.licenses.emplace_back(rapidjson_to_license(*it));
  }
  return licenses;
}

static rfl::Result<Licenses> read_using_rapidjson(
    const std::string &_json_string) {
  try {
    rapidjson::Document d;
    d.Parse(_json_string.c_str());
    return rapidjson_to_licenses(d.GetObject());
  } catch (std::exception &e) {
    return rfl::Error(e.what());
  }
}

// ----------------------------------------------------------------------------
// simdjson

Identifier simdjson_to_identifier(simdjson::ondemand::object _val);

Link simdjson_to_link(simdjson::ondemand::object _val);

OtherName simdjson_to_other_name(simdjson::ondemand::object _val);

Text simdjson_to_text(simdjson::ondemand::object _val);

License simdjson_to_license(simdjson::ondemand::object _val);

Licenses simdjson_to_licenses(simdjson::ondemand::object _val);

Identifier simdjson_to_identifier(simdjson::ondemand::object _val) {
  Identifier identifier;
  identifier.identifier = _val["identifier"].get_string().value();
  identifier.scheme = _val["scheme"].get_string().value();
  return identifier;
}

Link simdjson_to_link(simdjson::ondemand::object _val) {
  Link link;
  link.note = _val["note"].get_string().value();
  link.url = _val["url"].get_string().value();
  return link;
}

OtherName simdjson_to_other_name(simdjson::ondemand::object _val) {
  OtherName other_name;
  other_name.name = _val["name"].get_string().value();
  if (!_val["note"].is_null()) {
    other_name.note = _val["note"].get_string().value();
  }
  return other_name;
}

Text simdjson_to_text(simdjson::ondemand::object _val) {
  Text text;
  text.media_type = _val["media_type"].get_string().value();
  text.title = _val["title"].get_string().value();
  text.url = _val["url"].get_string().value();
  return text;
}

License simdjson_to_license(simdjson::ondemand::object _val) {
  License license;
  license.id = _val["id"].get_string().value();
  for (auto v : _val["identifiers"]) {
    license.identifiers.emplace_back(simdjson_to_identifier(v));
  }
  for (auto v : _val["links"]) {
    license.links.emplace_back(simdjson_to_link(v));
  }
  license.name = _val["name"].get_string().value();
  for (auto v : _val["other_names"]) {
    license.other_names.emplace_back(simdjson_to_other_name(v));
  }
  if (!_val["superseded_by"].is_null()) {
    license.superseded_by = _val["superseded_by"].get_string().value();
  }
  for (auto v : _val["keywords"]) {
    license.keywords.emplace_back(v.get_string().value());
  }
  for (auto v : _val["text"]) {
    license.text.emplace_back(simdjson_to_text(v));
  }
  return license;
}

Licenses simdjson_to_licenses(simdjson::ondemand::object _val) {
  Licenses licenses;
  for (auto v : _val["licenses"]) {
    licenses.licenses.emplace_back(simdjson_to_license(v));
  }
  return licenses;
}

static rfl::Result<Licenses> read_using_simdjson(
    const std::string &_json_string) {
  try {
    simdjson::ondemand::parser parser;
    auto padded_str = simdjson::padded_string(_json_string);
    auto doc = parser.iterate(padded_str).value();
    return simdjson_to_licenses(doc.get_object());
  } catch (std::exception &e) {
    return rfl::Error(e.what());
  }
}

// ----------------------------------------------------------------------------
// yyjson
/*
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
*/
// ----------------------------------------------------------------------------

static void BM_licenses_nlohmann(benchmark::State &state) {
  const auto json_string = load_data();
  for (auto _ : state) {
    const auto res = read_using_nlohmann(json_string);
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_licenses_nlohmann);

static void BM_licenses_rapidjson(benchmark::State &state) {
  const auto json_string = load_data();
  for (auto _ : state) {
    const auto res = read_using_rapidjson(json_string);
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_licenses_rapidjson);

static void BM_licenses_simdjson(benchmark::State &state) {
  const auto json_string = load_data();
  for (auto _ : state) {
    const auto res = read_using_simdjson(json_string);
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_licenses_simdjson);

static void BM_licenses_reflect_cpp(benchmark::State &state) {
  const auto json_string = load_data();
  for (auto _ : state) {
    const auto res = rfl::json::read<Licenses>(json_string);
    if (!res) {
      std::cout << res.error()->what() << std::endl;
    }
  }
}
BENCHMARK(BM_licenses_reflect_cpp);

// ----------------------------------------------------------------------------

}  // namespace licenses

