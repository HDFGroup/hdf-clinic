# A Machine-Readable File Format Specification for HDF5

## What

It's a formal description of a file format that is structured in a way that software tools (i.e. machines) can parse, analyze, and often use directly—for example, to read, write, validate, or generate files that conform to that format.

This contrasts with human-readable specs (like prose in a PDF or web page), which require manual interpretation by a developer.

Key Features of Machine-Readable Specifications:
  
  - **Structured syntax:** Usually written in a data serialization language (e.g., JSON, XML, YAML) or a domain-specific language (DSL).
  - **Unambiguous:** Defines the file format with precision—fields, sizes, types, byte order, constraints, and sometimes relationships between fields.
  - **Tool-friendly:** Can be used to automatically generate parsers, serializers, documentation, and test cases.

## Why

- **Trust:** Owners/maintainers don't play footsie with the spec.
- **Portability:** Makes format specs portable across languages, systems, and teams.
- **Maintainability:** (Ideally) Changes to the spec propagate automatically to tooling.
Validation & Debugging:** It is Easier to verify whether a file conforms to the spec and whether an implementation makes assumptions beyond the spec.
- **Reverse Engineering:** Helps document legacy or proprietary formats.

## How

- Common Lisp: Read chapters 24 and 25 in [Practical Common Lisp](https://gigamonkeys.com/book/) by Peter Seibel
- YAML fans enjoy [Kaitai](https://kaitai.io/)
- I like [GNU poke](https://www.jemarch.net/poke)

So far, nothing new here. (I talked about this a few years ago, but I won't have the time to "sweat the details.")

How about asking [ChatGPT](https://chatgpt.com/) for a helping hand? Worst case: Come back another time...

## Dramatis Personae

- [HDF5 File Format Specification Version 2.0](https://support.hdfgroup.org/documentation/hdf5/latest/_f_m_t2.html)
- [GNU poke 4.3](https://www.jemarch.net/poke-4.3-manual/poke.html)
- [ChatGPT GPT-4o](https://openai.com/index/gpt-4o-system-card/)

## Sample Problems

### Problem 1: Parsing the Superblock

### Problem 2: Parsing Datatype Messages

### Problem 3: ???
