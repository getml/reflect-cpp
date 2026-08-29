# Supported formats

reflect-cpp provides a unified reflection-based interface across different serialization formats. They are listed below:

## Comparing formats

When choosing a format for your use case, consider the following factors:

1. **Readability**: JSON, YAML, TOML, and CSV are human-readable. Binary formats (BSON, CBOR, UBJSON, msgpack, Avro, Cap'n Proto, yas, etc.) are not.
2. **Performance**: For raw speed, yas, msgpack, and flexbuffers are typically the fastest. Note that due to a highly efficient implementation, JSON is a lot faster than you might think.
3. **Backwards compatibility**: Schemaless formats (JSON, YAML, TOML, BSON, CBOR, UBJSON, msgpack, Flexbuffers) tolerate structural changes better than schemaful formats.
4. **Data structure**: Nested, hierarchical data works best with JSON, YAML, or XML. Flat, tabular data works best with CSV or Parquet.
5. **External dependencies**: Some formats require linking to external libraries (Avro, Cap'n Proto, CBOR, msgpack, YAML, TOML, XML, BSON, Boost.Serialization, Cereal, Flexbuffers). JSON and environment variables have no external dependencies.

## Textual formats

Human-readable formats for configuration, data exchange, and legacy integration.

{{< cards >}}
  {{< card link="json" title="JSON" icon="book-open" subtitle="Out-of-the-box support via yyjson. Standard for web APIs and data interchange." >}}
  {{< card link="yaml" title="YAML" icon="book-open" subtitle="Textual format with an emphasis on readability. Widely used in Docker, Kubernetes, and CI/CD configuration." >}}
  {{< card link="toml" title="TOML" icon="book-open" subtitle="Textual format with an emphasis on readability. Popular for application configuration in the Rust ecosystem." >}}
  {{< card link="csv" title="CSV" icon="book-open" subtitle="Tabular textual format. The universal interchange format for spreadsheets, databases, and ETL pipelines." >}}
  {{< card link="xml" title="XML" icon="book-open" subtitle="Textual format used in many legacy projects. Dominates enterprise SOAP APIs and document formats." >}}
{{< /cards >}}

## JSON-like binary formats

Compact binary formats designed for small payload sizes and interoperability.

{{< cards >}}
  {{< card link="bson" title="BSON" icon="book-open" subtitle="JSON-like binary format. Native storage format for MongoDB." >}}
  {{< card link="cbor" title="CBOR" icon="book-open" subtitle="JSON-like binary format. IETF-standardized (RFC 7049); widely used in IoT and JWT tokens." >}}
  {{< card link="msgpack" title="msgpack" icon="book-open" subtitle="JSON-like binary format. Optimized for speed; popular in RPC frameworks like gRPC. Very fast." >}}
  {{< card link="ubjson" title="UBJSON" icon="book-open" subtitle="JSON-like binary format. A more efficient binary replacement for JSON in network protocols." >}}
{{< /cards >}}

## Schemaful binary formats

Binary formats that require a predefined schema, enabling strict type safety and efficient serialization.

{{< cards >}}
  {{< card link="avro" title="Avro" icon="book-open" subtitle="Schemaful binary format. Widely used in big data pipelines (Apache Hadoop, Kafka) for schema evolution." >}}
  {{< card link="capnproto" title="Cap'n Proto" icon="book-open" subtitle="Schemaful binary format. Designed for game networking and IPC." >}}
{{< /cards >}}

## Tabular formats

Formats designed for storing collections of records, optimized for analytical workloads and data exchange.

{{< cards >}}
  {{< card link="csv" title="CSV" icon="book-open" subtitle="Tabular textual format. The universal format for spreadsheets, databases, and ETL pipelines." >}}
  {{< card link="parquet" title="Parquet" icon="book-open" subtitle="Tabular binary format. Columnar storage of choice for data warehousing and Apache Spark." >}}
{{< /cards >}}

## C++ serialization libraries

C++-native serialization libraries offering compact, high-performance binary output.

{{< cards >}}
  {{< card link="cereal" title="Cereal" icon="book-open" subtitle="C++ serialization library with multiple formats. Commonly used in game development and scientific computing for checkpoint/restart workflows." >}}
  {{< card link="flexbuffers" title="Flexbuffers" icon="book-open" subtitle="Schema-less version of flatbuffers, binary format. Ideal for game assets and dynamic data without code generation. Very fast." >}}
  {{< card link="yas" title="yas" icon="book-open" subtitle="Very fast and compact serialization library. Targets applications where speed and memory matter. Very fast." >}}
  {{< card link="boost_serialization" title="Boost.Serialization" icon="book-open" subtitle="Streaming binary format with archive interop. Integrates with the broader Boost ecosystem for object graph serialization." >}}
{{< /cards >}}

## Other

Non-serialization interfaces for configuration and command-line integration.

{{< cards >}}
  {{< card link="cli" title="Command Line Interface" icon="book-open" subtitle="Parse command-line arguments into a reflectable struct." >}}
  {{< card link="env" title="Environment Variables" icon="book-open" subtitle="Flat, hierarchical key-value store for configuration. Standard for containerized and cloud-native applications." >}}
  {{< card link="supporting_your_own_format" title="Supporting Your Own Format" icon="book-open" subtitle="Extend reflect-cpp to support custom serialization formats." >}}
{{< /cards >}}
